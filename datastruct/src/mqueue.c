/**
 * @file mqueue.c
 * @brief 线程安全消息队列实现（跨平台：Windows / Linux）
 *
 * 实现策略：
 * - 固定大小环形缓冲区，预分配，无运行时动态内存分配
 * - Windows 使用 CRITICAL_SECTION + CONDITION_VARIABLE
 * - Linux 使用 pthread_mutex_t + pthread_cond_t
 * - 支持 send（队尾）和 send_urgent（队首）
 * - 支持超时机制（毫秒级精度）
 *
 * 线程安全保证：
 * - 所有公共方法在进入时加锁
 * - 使用条件变量实现生产者-消费者同步
 * - 单次 read/write 全部在锁内完成，无数据竞争
 */

#include "mqueue.h"

#include <stdlib.h>  /* malloc, free */
#include <string.h>  /* memcpy */
#include <errno.h>   /* ETIMEDOUT */

/* ================================================================== */
/*  平台相关的线程同步抽象                                              */
/* ================================================================== */
#if defined(_WIN32) || defined(_WIN64)
/* ------------------------------------------------------------------ */
/*  Windows 实现：CRITICAL_SECTION + CONDITION_VARIABLE                */
/* ------------------------------------------------------------------ */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#define MQ_MUTEX_T            CRITICAL_SECTION
#define MQ_COND_T             CONDITION_VARIABLE
#define MQ_MUTEX_INIT(m)      InitializeCriticalSection(m)
#define MQ_MUTEX_LOCK(m)      EnterCriticalSection(m)
#define MQ_MUTEX_UNLOCK(m)    LeaveCriticalSection(m)
#define MQ_MUTEX_DESTROY(m)   DeleteCriticalSection(m)
#define MQ_COND_INIT(c)       InitializeConditionVariable(c)
#define MQ_COND_SIGNAL(c)     WakeConditionVariable(c)
#define MQ_COND_BROADCAST(c)  WakeAllConditionVariable(c)

/* Windows 条件变量等待（相对毫秒超时） */
static int mq_cond_wait(MQ_COND_T* cond, MQ_MUTEX_T* lock, int timeout_ms)
{
    BOOL ok;
    if (timeout_ms < 0) {
        ok = SleepConditionVariableCS(cond, lock, INFINITE);
    } else {
        ok = SleepConditionVariableCS(cond, lock, (DWORD)timeout_ms);
    }
    return ok ? 0 : -1;
}

#else
/* ------------------------------------------------------------------ */
/*  Linux/POSIX 实现：pthread_mutex_t + pthread_cond_t                 */
/* ------------------------------------------------------------------ */
#include <pthread.h>
#include <time.h>    /* clock_gettime, timespec */

#define MQ_MUTEX_T            pthread_mutex_t
#define MQ_COND_T             pthread_cond_t
#define MQ_MUTEX_INIT(m)      pthread_mutex_init(m, NULL)
#define MQ_MUTEX_LOCK(m)      pthread_mutex_lock(m)
#define MQ_MUTEX_UNLOCK(m)    pthread_mutex_unlock(m)
#define MQ_MUTEX_DESTROY(m)   pthread_mutex_destroy(m)
#define MQ_COND_INIT(c)       pthread_cond_init(c, NULL)
#define MQ_COND_SIGNAL(c)     pthread_cond_signal(c)
#define MQ_COND_BROADCAST(c)  pthread_cond_broadcast(c)

/* Linux 条件变量等待（将相对毫秒转换为绝对时间） */
static int mq_cond_wait(MQ_COND_T* cond, MQ_MUTEX_T* lock, int timeout_ms)
{
    if (timeout_ms < 0) {
        /* 无限等待 */
        return pthread_cond_wait(cond, lock);
    }

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec  += timeout_ms / 1000;
    ts.tv_nsec += (timeout_ms % 1000) * 1000000L;
    /* 处理纳秒溢出 */
    if (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec  += 1;
        ts.tv_nsec -= 1000000000L;
    }

    int ret = pthread_cond_timedwait(cond, lock, &ts);
    return (ret == 0) ? 0 : -1;
}

#endif

/* ================================================================== */
/*  消息队列结构体                                                     */
/* ================================================================== */
struct ds_mqueue {
    unsigned char* buffer;       /**< 环形缓冲区 */
    size_t max_msgs;             /**< 容量（最大消息数） */
    size_t msg_size;             /**< 每条消息大小 */

    volatile size_t head;        /**< 读指针（队首） */
    volatile size_t tail;        /**< 写指针（队尾） */
    volatile size_t count;       /**< 当前消息数 */

    MQ_MUTEX_T lock;             /**< 互斥锁 */
    MQ_COND_T  not_full;         /**< 队列未满条件（通知生产者） */
    MQ_COND_T  not_empty;        /**< 队列非空条件（通知消费者） */

    volatile int destroyed;      /**< 销毁标志，唤醒所有等待线程 */
};

/* ================================================================== */
/*  生命周期管理                                                       */
/* ================================================================== */

ds_mqueue_t* ds_mqueue_create(size_t max_msgs, size_t msg_size)
{
    if (max_msgs == 0 || msg_size == 0) {
        return NULL;
    }

    ds_mqueue_t* mq = (ds_mqueue_t*)malloc(sizeof(ds_mqueue_t));
    if (!mq) {
        return NULL;
    }

    mq->buffer = (unsigned char*)malloc(max_msgs * msg_size);
    if (!mq->buffer) {
        free(mq);
        return NULL;
    }

    mq->max_msgs = max_msgs;
    mq->msg_size = msg_size;
    mq->head = 0;
    mq->tail = 0;
    mq->count = 0;
    mq->destroyed = 0;

    MQ_MUTEX_INIT(&mq->lock);
    MQ_COND_INIT(&mq->not_full);
    MQ_COND_INIT(&mq->not_empty);

    return mq;
}

void ds_mqueue_destroy(ds_mqueue_t* mq)
{
    if (!mq) {
        return;
    }

    /* 设置销毁标志并唤醒所有等待线程 */
    MQ_MUTEX_LOCK(&mq->lock);
    mq->destroyed = 1;
    MQ_MUTEX_UNLOCK(&mq->lock);

    MQ_COND_BROADCAST(&mq->not_full);
    MQ_COND_BROADCAST(&mq->not_empty);

    MQ_MUTEX_DESTROY(&mq->lock);
    free(mq->buffer);
    free(mq);
}

/* ================================================================== */
/*  消息发送与接收                                                     */
/* ================================================================== */

int ds_mqueue_send(ds_mqueue_t* mq, const void* msg, int timeout_ms)
{
    if (!mq || !msg) {
        return -1;
    }

    MQ_MUTEX_LOCK(&mq->lock);

    /* 等待直到队列非满或超时/销毁 */
    while (mq->count >= mq->max_msgs && !mq->destroyed) {
        if (timeout_ms == DS_MQUEUE_NO_WAIT) {
            MQ_MUTEX_UNLOCK(&mq->lock);
            return -1;
        }
        if (mq_cond_wait(&mq->not_full, &mq->lock, timeout_ms) != 0) {
            MQ_MUTEX_UNLOCK(&mq->lock);
            return -1;  /* 超时 */
        }
    }

    if (mq->destroyed) {
        MQ_MUTEX_UNLOCK(&mq->lock);
        return -1;
    }

    /* 写入队尾 */
    memcpy(mq->buffer + mq->tail * mq->msg_size, msg, mq->msg_size);
    mq->tail = (mq->tail + 1) % mq->max_msgs;
    mq->count++;

    MQ_MUTEX_UNLOCK(&mq->lock);
    MQ_COND_SIGNAL(&mq->not_empty);
    return 0;
}

int ds_mqueue_send_urgent(ds_mqueue_t* mq, const void* msg, int timeout_ms)
{
    if (!mq || !msg) {
        return -1;
    }

    MQ_MUTEX_LOCK(&mq->lock);

    while (mq->count >= mq->max_msgs && !mq->destroyed) {
        if (timeout_ms == DS_MQUEUE_NO_WAIT) {
            MQ_MUTEX_UNLOCK(&mq->lock);
            return -1;
        }
        if (mq_cond_wait(&mq->not_full, &mq->lock, timeout_ms) != 0) {
            MQ_MUTEX_UNLOCK(&mq->lock);
            return -1;
        }
    }

    if (mq->destroyed) {
        MQ_MUTEX_UNLOCK(&mq->lock);
        return -1;
    }

    /* 写入队首（head 前一个位置） */
    mq->head = (mq->head == 0) ? mq->max_msgs - 1 : mq->head - 1;
    memcpy(mq->buffer + mq->head * mq->msg_size, msg, mq->msg_size);
    mq->count++;

    MQ_MUTEX_UNLOCK(&mq->lock);
    MQ_COND_SIGNAL(&mq->not_empty);
    return 0;
}

int ds_mqueue_recv(ds_mqueue_t* mq, void* buf, int timeout_ms)
{
    if (!mq || !buf) {
        return -1;
    }

    MQ_MUTEX_LOCK(&mq->lock);

    /* 等待直到队列非空或超时/销毁 */
    while (mq->count == 0 && !mq->destroyed) {
        if (timeout_ms == DS_MQUEUE_NO_WAIT) {
            MQ_MUTEX_UNLOCK(&mq->lock);
            return -1;
        }
        if (mq_cond_wait(&mq->not_empty, &mq->lock, timeout_ms) != 0) {
            MQ_MUTEX_UNLOCK(&mq->lock);
            return -1;  /* 超时 */
        }
    }

    if (mq->destroyed) {
        MQ_MUTEX_UNLOCK(&mq->lock);
        return -1;
    }

    /* 从队首读取 */
    memcpy(buf, mq->buffer + mq->head * mq->msg_size, mq->msg_size);
    mq->head = (mq->head + 1) % mq->max_msgs;
    mq->count--;

    MQ_MUTEX_UNLOCK(&mq->lock);
    MQ_COND_SIGNAL(&mq->not_full);
    return 0;
}

int ds_mqueue_try_recv(ds_mqueue_t* mq, void* buf)
{
    return ds_mqueue_recv(mq, buf, DS_MQUEUE_NO_WAIT);
}

/* ================================================================== */
/*  状态查询                                                           */
/* ================================================================== */

size_t ds_mqueue_count(const ds_mqueue_t* mq)
{
    return mq ? mq->count : 0;
}

size_t ds_mqueue_capacity(const ds_mqueue_t* mq)
{
    return mq ? mq->max_msgs : 0;
}

int ds_mqueue_full(const ds_mqueue_t* mq)
{
    return (mq && mq->count >= mq->max_msgs) ? 1 : 0;
}

int ds_mqueue_empty(const ds_mqueue_t* mq)
{
    return (mq == NULL || mq->count == 0) ? 1 : 0;
}

void ds_mqueue_flush(ds_mqueue_t* mq)
{
    if (!mq) {
        return;
    }
    MQ_MUTEX_LOCK(&mq->lock);
    mq->head = 0;
    mq->tail = 0;
    mq->count = 0;
    MQ_MUTEX_UNLOCK(&mq->lock);
    MQ_COND_BROADCAST(&mq->not_full);
}
