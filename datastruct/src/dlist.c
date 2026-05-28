/**
 * @file dlist.c
 * @brief 双向链表实现
 *
 * 实现策略：
 * - 哨兵节点环形结构：head->next 指向第一个节点，head->prev 指向最后一个节点
 * - 空链表时 sentinel->next == sentinel->prev == sentinel
 * - 这种设计使插入和删除无需判断边界条件
 */

#include "dlist.h"
#include <stdlib.h>  /* malloc, free */
#include <string.h>  /* memcpy */

/* ------------------------------------------------------------------ */
/*  链表节点结构                                                       */
/* ------------------------------------------------------------------ */
struct ds_dlist_node {
    ds_dlist_node_t* prev;  /**< 前驱 */
    ds_dlist_node_t* next;  /**< 后继 */
    /* 节点数据紧跟在指针之后 */
};

/** @brief 获取节点数据区域的宏 */
#define NODE_DATA(node) ((unsigned char*)(node) + sizeof(ds_dlist_node_t))

/* ------------------------------------------------------------------ */
/*  链表结构体                                                         */
/* ------------------------------------------------------------------ */
struct ds_dlist {
    ds_dlist_node_t* sentinel;  /**< 哨兵节点 */
    size_t  elem_size;          /**< 元素大小 */
    size_t  size;               /**< 元素个数 */
    ds_dlist_free_fn free_fn;   /**< 析构回调 */
};

/* ------------------------------------------------------------------ */
/*  内部辅助函数                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 分配一个新节点
 * @param dlist 链表指针
 * @param data  元素数据
 * @return 成功返回节点指针，失败返回 NULL
 */
static ds_dlist_node_t* dlist_alloc_node(ds_dlist_t* dlist, const void* data)
{
    ds_dlist_node_t* node = (ds_dlist_node_t*)malloc(sizeof(ds_dlist_node_t) + dlist->elem_size);
    if (!node) {
        return NULL;
    }
    memcpy(NODE_DATA(node), data, dlist->elem_size);
    return node;
}

/**
 * @brief 释放节点的元素和节点本身
 * @param dlist 链表指针
 * @param node  要释放的节点
 */
static void dlist_free_node(ds_dlist_t* dlist, ds_dlist_node_t* node)
{
    if (dlist->free_fn) {
        dlist->free_fn(NODE_DATA(node));
    }
    free(node);
}

/**
 * @brief 在指定节点之后插入新节点
 * @param dlist 链表指针
 * @param pos   新节点将插入到此节点之后
 * @param data  元素数据
 * @return 0 成功，-1 失败
 */
static int dlist_insert_after_node(ds_dlist_t* dlist,
                                    ds_dlist_node_t* pos,
                                    const void* data)
{
    ds_dlist_node_t* new_node = dlist_alloc_node(dlist, data);
    if (!new_node) {
        return -1;
    }
    new_node->prev = pos;
    new_node->next = pos->next;
    pos->next->prev = new_node;
    pos->next = new_node;
    dlist->size++;
    return 0;
}

/**
 * @brief 删除指定节点
 * @param dlist 链表指针
 * @param node  要删除的节点
 * @param out   输出缓冲区（可为 NULL）
 */
static void dlist_remove_node(ds_dlist_t* dlist,
                               ds_dlist_node_t* node,
                               void* out)
{
    if (out) {
        memcpy(out, NODE_DATA(node), dlist->elem_size);
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    dlist_free_node(dlist, node);
    dlist->size--;
}

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

ds_dlist_t* ds_dlist_create(size_t elem_size, ds_dlist_free_fn free_fn)
{
    if (elem_size == 0) {
        return NULL;
    }

    ds_dlist_t* list = (ds_dlist_t*)malloc(sizeof(ds_dlist_t));
    if (!list) {
        return NULL;
    }

    list->sentinel = (ds_dlist_node_t*)malloc(sizeof(ds_dlist_node_t));
    if (!list->sentinel) {
        free(list);
        return NULL;
    }

    /* 环形：哨兵指向自身 */
    list->sentinel->prev = list->sentinel;
    list->sentinel->next = list->sentinel;
    list->elem_size = elem_size;
    list->size = 0;
    list->free_fn = free_fn;
    return list;
}

void ds_dlist_destroy(ds_dlist_t* list)
{
    if (!list) {
        return;
    }
    ds_dlist_clear(list);
    free(list->sentinel);
    free(list);
}

/* ------------------------------------------------------------------ */
/*  容量与状态                                                         */
/* ------------------------------------------------------------------ */

size_t ds_dlist_size(const ds_dlist_t* list)
{
    return list ? list->size : 0;
}

int ds_dlist_empty(const ds_dlist_t* list)
{
    return (list == NULL || list->size == 0) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/*  元素访问                                                           */
/* ------------------------------------------------------------------ */

int ds_dlist_front(const ds_dlist_t* list, void* out)
{
    if (!list || !out || list->size == 0) {
        return -1;
    }
    memcpy(out, NODE_DATA(list->sentinel->next), list->elem_size);
    return 0;
}

int ds_dlist_back(const ds_dlist_t* list, void* out)
{
    if (!list || !out || list->size == 0) {
        return -1;
    }
    memcpy(out, NODE_DATA(list->sentinel->prev), list->elem_size);
    return 0;
}

/* ------------------------------------------------------------------ */
/*  修改操作                                                           */
/* ------------------------------------------------------------------ */

int ds_dlist_push_front(ds_dlist_t* list, const void* data)
{
    if (!list || !data) {
        return -1;
    }
    /* 哨兵之后插入，即首节点前 */
    return dlist_insert_after_node(list, list->sentinel, data);
}

int ds_dlist_push_back(ds_dlist_t* list, const void* data)
{
    if (!list || !data) {
        return -1;
    }
    /* 哨兵之前插入，即尾节点后 */
    return dlist_insert_after_node(list, list->sentinel->prev, data);
}

int ds_dlist_pop_front(ds_dlist_t* list, void* out)
{
    if (!list || list->size == 0) {
        return -1;
    }
    dlist_remove_node(list, list->sentinel->next, out);
    return 0;
}

int ds_dlist_pop_back(ds_dlist_t* list, void* out)
{
    if (!list || list->size == 0) {
        return -1;
    }
    dlist_remove_node(list, list->sentinel->prev, out);
    return 0;
}

void ds_dlist_clear(ds_dlist_t* list)
{
    if (!list || list->size == 0) {
        return;
    }
    ds_dlist_node_t* curr = list->sentinel->next;
    while (curr != list->sentinel) {
        ds_dlist_node_t* next = curr->next;
        dlist_free_node(list, curr);
        curr = next;
    }
    list->sentinel->prev = list->sentinel;
    list->sentinel->next = list->sentinel;
    list->size = 0;
}

/* ------------------------------------------------------------------ */
/*  迭代器操作                                                         */
/* ------------------------------------------------------------------ */

ds_dlist_iter_t ds_dlist_begin(ds_dlist_t* list)
{
    ds_dlist_iter_t iter = { list, list ? list->sentinel->next : NULL };
    return iter;
}

ds_dlist_iter_t ds_dlist_end(ds_dlist_t* list)
{
    ds_dlist_iter_t iter = { list, list ? list->sentinel : NULL };
    return iter;
}

int ds_dlist_iter_next(ds_dlist_iter_t* iter)
{
    if (!iter || !iter->list || !iter->node ||
        iter->node == iter->list->sentinel) {
        return -1;
    }
    iter->node = iter->node->next;
    return (iter->node == iter->list->sentinel) ? -1 : 0;
}

int ds_dlist_iter_prev(ds_dlist_iter_t* iter)
{
    if (!iter || !iter->list || !iter->node ||
        iter->node->prev == iter->list->sentinel) {
        return -1;
    }
    iter->node = iter->node->prev;
    return 0;
}

void* ds_dlist_iter_data(const ds_dlist_iter_t* iter)
{
    if (!iter || !iter->node || !iter->list ||
        iter->node == iter->list->sentinel) {
        return NULL;
    }
    return NODE_DATA(iter->node);
}

int ds_dlist_iter_equal(const ds_dlist_iter_t* a, const ds_dlist_iter_t* b)
{
    if (!a || !b) {
        return 0;
    }
    return (a->list == b->list && a->node == b->node) ? 1 : 0;
}

int ds_dlist_insert_after(ds_dlist_iter_t* iter, const void* data)
{
    if (!iter || !iter->list || !iter->node || !data) {
        return -1;
    }
    return dlist_insert_after_node(iter->list, iter->node, data);
}

int ds_dlist_insert_before(ds_dlist_iter_t* iter, const void* data)
{
    if (!iter || !iter->list || !iter->node || !data) {
        return -1;
    }
    return dlist_insert_after_node(iter->list, iter->node->prev, data);
}

int ds_dlist_erase(ds_dlist_iter_t* iter, void* out)
{
    if (!iter || !iter->list || !iter->node ||
        iter->node == iter->list->sentinel) {
        return -1;
    }
    ds_dlist_node_t* to_remove = iter->node;
    iter->node = to_remove->next;  /* 迭代器指向下一个有效节点 */
    dlist_remove_node(iter->list, to_remove, out);
    return 0;
}
