/**
 * @file mqueue.h
 * @brief 线程安全消息队列（Message Queue）
 *
 * 特性：
 * - 多生产者/多消费者（MPMC）安全
 * - 支持阻塞等待和超时机制
 * - 固定大小环形缓冲区，无动态内存分配
 * - 基于 Windows CRITICAL_SECTION + CONDITION_VARIABLE
 * - 支持紧急消息（插队到队首）
 *
 * @author 飞机大战 EX Team
 * @version 1.0.0
 * @date 2026-05-27
 */

#ifndef DS_MQUEUE_H
#define DS_MQUEUE_H

#include <stddef.h>  /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  常量定义                                                           */
/* ------------------------------------------------------------------ */

/** @brief 无限等待（阻塞直到有消息） */
#define DS_MQUEUE_WAIT_FOREVER (-1)

/** @brief 不等待（立即返回） */
#define DS_MQUEUE_NO_WAIT      (0)

/* ------------------------------------------------------------------ */
/*  类型定义                                                           */
/* ------------------------------------------------------------------ */

/** @brief 消息队列结构体（不透明） */
typedef struct ds_mqueue ds_mqueue_t;

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 创建消息队列
 * @param max_msgs 最大消息数（必须 > 0）
 * @param msg_size 每条消息的大小（字节，必须 > 0）
 * @return 成功返回队列指针，失败返回 NULL
 *
 * @note 消息队列在创建时预分配固定大小的环形缓冲区，
 *       运行过程中不会动态分配内存。
 */
ds_mqueue_t* ds_mqueue_create(size_t max_msgs, size_t msg_size);

/**
 * @brief 销毁消息队列
 * @param mq 队列指针（允许 NULL）
 *
 * @note 销毁时唤醒所有等待的线程，
 *       后续对已销毁队列的操作行为未定义。
 */
void ds_mqueue_destroy(ds_mqueue_t* mq);

/* ------------------------------------------------------------------ */
/*  消息发送与接收                                                     */
/* ------------------------------------------------------------------ */

/**
 * @brief 发送消息（队列满时阻塞）
 * @param mq   队列指针
 * @param msg  消息内容指针
 * @param timeout_ms 超时时间（毫秒）
 *                   DS_MQUEUE_WAIT_FOREVER 无限等待
 *                   DS_MQUEUE_NO_WAIT 不等待
 *                   其他值为等待的毫秒数
 * @return 0 成功，-1 超时或队列被销毁
 */
int ds_mqueue_send(ds_mqueue_t* mq, const void* msg, int timeout_ms);

/**
 * @brief 发送紧急消息（插队到队首）
 * @param mq   队列指针
 * @param msg  消息内容
 * @param timeout_ms 超时时间
 * @return 0 成功，-1 超时
 */
int ds_mqueue_send_urgent(ds_mqueue_t* mq, const void* msg, int timeout_ms);

/**
 * @brief 接收消息（队列空时阻塞）
 * @param mq   队列指针
 * @param buf  接收缓冲区
 * @param timeout_ms 超时时间
 * @return 0 成功，-1 超时或队列被销毁
 */
int ds_mqueue_recv(ds_mqueue_t* mq, void* buf, int timeout_ms);

/**
 * @brief 尝试接收消息（不阻塞）
 * @param mq  队列指针
 * @param buf 接收缓冲区
 * @return 0 成功，-1 队列为空
 */
int ds_mqueue_try_recv(ds_mqueue_t* mq, void* buf);

/* ------------------------------------------------------------------ */
/*  状态查询                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取当前消息数
 */
size_t ds_mqueue_count(const ds_mqueue_t* mq);

/**
 * @brief 获取队列容量
 */
size_t ds_mqueue_capacity(const ds_mqueue_t* mq);

/**
 * @brief 判断队列是否已满
 * @return 非 0 表示满，0 表示未满
 */
int ds_mqueue_full(const ds_mqueue_t* mq);

/**
 * @brief 判断队列是否为空
 * @return 非 0 表示为空，0 表示非空
 */
int ds_mqueue_empty(const ds_mqueue_t* mq);

/**
 * @brief 清空队列中所有消息
 * @param mq 队列指针
 */
void ds_mqueue_flush(ds_mqueue_t* mq);

#ifdef __cplusplus
}
#endif

#endif /* DS_MQUEUE_H */
