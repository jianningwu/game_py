/**
 * @file dlist.h
 * @brief 通用双向链表（Doubly Linked List）
 *
 * 特性：
 * - 哨兵节点（sentinel）环形实现，简化边界操作
 * - 存储任意类型元素（memcpy 拷贝语义）
 * - 支持前向/后向遍历迭代器
 * - 允许注册元素析构回调
 * - 所有操作返回状态码
 *
 * @author 飞机大战 EX Team
 * @version 1.0.0
 * @date 2026-05-27
 */

#ifndef DS_DLIST_H
#define DS_DLIST_H

#include <stddef.h>  /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  类型定义                                                           */
/* ------------------------------------------------------------------ */

/** @brief 链表结构体（不透明） */
typedef struct ds_dlist ds_dlist_t;

/** @brief 链表节点（不透明） */
typedef struct ds_dlist_node ds_dlist_node_t;

/**
 * @brief 链表迭代器
 */
typedef struct {
    ds_dlist_t*      list;  /**< 所属链表 */
    ds_dlist_node_t* node;  /**< 当前节点 */
} ds_dlist_iter_t;

/**
 * @brief 元素析构函数类型
 * @param data 指向元素数据的指针
 */
typedef void (*ds_dlist_free_fn)(void* data);

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 创建双向链表
 * @param elem_size 元素大小（字节），必须 > 0
 * @param free_fn   元素析构回调（可为 NULL）
 * @return 成功返回链表指针，失败返回 NULL
 */
ds_dlist_t* ds_dlist_create(size_t elem_size, ds_dlist_free_fn free_fn);

/**
 * @brief 销毁链表，释放所有内存
 * @param list 链表指针（允许 NULL）
 *
 * @note 销毁时会遍历所有节点，对每个元素调用 free_fn（若注册）。
 */
void ds_dlist_destroy(ds_dlist_t* list);

/* ------------------------------------------------------------------ */
/*  容量与状态                                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取链表中的元素个数
 */
size_t ds_dlist_size(const ds_dlist_t* list);

/**
 * @brief 判断链表是否为空
 * @return 非 0 表示为空，0 表示非空
 */
int ds_dlist_empty(const ds_dlist_t* list);

/* ------------------------------------------------------------------ */
/*  元素访问                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取第一个元素
 * @param list 链表指针
 * @param out  输出缓冲区
 * @return 0 成功，-1 链表为空
 */
int ds_dlist_front(const ds_dlist_t* list, void* out);

/**
 * @brief 获取最后一个元素
 * @param list 链表指针
 * @param out  输出缓冲区
 * @return 0 成功，-1 链表为空
 */
int ds_dlist_back(const ds_dlist_t* list, void* out);

/* ------------------------------------------------------------------ */
/*  修改操作                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 头部插入元素
 * @param list 链表指针
 * @param data 元素指针
 * @return 0 成功，-1 失败
 */
int ds_dlist_push_front(ds_dlist_t* list, const void* data);

/**
 * @brief 尾部插入元素
 * @param list 链表指针
 * @param data 元素指针
 * @return 0 成功，-1 失败
 */
int ds_dlist_push_back(ds_dlist_t* list, const void* data);

/**
 * @brief 删除头部元素
 * @param list 链表指针
 * @param out  输出缓冲区（可为 NULL，表示丢弃）
 * @return 0 成功，-1 链表为空
 */
int ds_dlist_pop_front(ds_dlist_t* list, void* out);

/**
 * @brief 删除尾部元素
 * @param list 链表指针
 * @param out  输出缓冲区（可为 NULL）
 * @return 0 成功，-1 链表为空
 */
int ds_dlist_pop_back(ds_dlist_t* list, void* out);

/**
 * @brief 清空链表
 * @param list 链表指针
 */
void ds_dlist_clear(ds_dlist_t* list);

/* ------------------------------------------------------------------ */
/*  迭代器操作                                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取指向第一个元素的迭代器
 * @param list 链表指针
 * @return 迭代器。若链表为空，返回的迭代器等于 end()
 */
ds_dlist_iter_t ds_dlist_begin(ds_dlist_t* list);

/**
 * @brief 获取末尾哨兵迭代器
 * @param list 链表指针
 * @return 迭代器
 */
ds_dlist_iter_t ds_dlist_end(ds_dlist_t* list);

/**
 * @brief 迭代器前进到下一个元素
 * @param iter 迭代器指针
 * @return 0 成功，-1 已到达链表末尾
 */
int ds_dlist_iter_next(ds_dlist_iter_t* iter);

/**
 * @brief 迭代器后退到上一个元素
 * @param iter 迭代器指针
 * @return 0 成功，-1 已到达链表开头
 */
int ds_dlist_iter_prev(ds_dlist_iter_t* iter);

/**
 * @brief 获取迭代器指向的数据指针
 * @param iter 迭代器指针
 * @return 数据指针，无效迭代器返回 NULL
 */
void* ds_dlist_iter_data(const ds_dlist_iter_t* iter);

/**
 * @brief 判断两个迭代器是否相等
 */
int ds_dlist_iter_equal(const ds_dlist_iter_t* a, const ds_dlist_iter_t* b);

/**
 * @brief 在迭代器位置之后插入元素
 * @param iter 迭代器指针
 * @param data 元素指针
 * @return 0 成功，-1 失败
 */
int ds_dlist_insert_after(ds_dlist_iter_t* iter, const void* data);

/**
 * @brief 在迭代器位置之前插入元素
 * @param iter 迭代器指针
 * @param data 元素指针
 * @return 0 成功，-1 失败
 */
int ds_dlist_insert_before(ds_dlist_iter_t* iter, const void* data);

/**
 * @brief 删除迭代器指向的元素
 * @param iter 迭代器指针（删除后指向下一节点或 end()）
 * @param out  输出缓冲区（可为 NULL）
 * @return 0 成功，-1 迭代器无效
 */
int ds_dlist_erase(ds_dlist_iter_t* iter, void* out);

#ifdef __cplusplus
}
#endif

#endif /* DS_DLIST_H */
