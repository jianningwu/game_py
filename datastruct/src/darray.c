/**
 * @file darray.c
 * @brief 动态数组实现
 *
 * 实现策略：
 * - 倍增扩容（扩容因子 1.5），均摊 O(1) 尾部插入
 * - 元素通过 memcpy 拷贝，适用于 POD 类型
 * - 所有错误通过返回值报告，不调用 abort/exit
 */

#include "darray.h"
#include <stdlib.h>   /* malloc, realloc, free */
#include <string.h>   /* memcpy, memmove, memset */

/* ------------------------------------------------------------------ */
/*  内部常量                                                           */
/* ------------------------------------------------------------------ */

/** @brief 默认初始容量（元素个数） */
#define DARRAY_DEFAULT_CAPACITY 16

/** @brief 扩容因子（1.5 倍） */
#define DARRAY_GROWTH_FACTOR    1.5

/* ------------------------------------------------------------------ */
/*  动态数组结构体                                                     */
/* ------------------------------------------------------------------ */
struct ds_darray {
    void*  data;       /**< 连续内存缓冲区 */
    size_t elem_size;  /**< 每个元素占用的字节数 */
    size_t size;       /**< 当前元素个数 */
    size_t capacity;   /**< 当前容量（最多容纳多少个元素） */
};

/* ------------------------------------------------------------------ */
/*  内部辅助函数                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 计算扩容后的新容量
 * @param current 当前容量
 * @return 新容量（至少比当前大 1）
 */
static size_t darray_calc_growth(size_t current)
{
    if (current == 0) {
        return DARRAY_DEFAULT_CAPACITY;
    }
    /* 容量 * 1.5，但至少要能多放一个 */
    size_t new_cap = (size_t)(current * DARRAY_GROWTH_FACTOR);
    if (new_cap <= current) {
        /* 防止溢出时原地踏步 */
        new_cap = current + 1;
    }
    return new_cap;
}

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

ds_darray_t* ds_darray_create(size_t elem_size, size_t capacity)
{
    if (elem_size == 0) {
        return NULL;  /* 元素大小不能为 0 */
    }

    ds_darray_t* arr = (ds_darray_t*)malloc(sizeof(ds_darray_t));
    if (!arr) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = DARRAY_DEFAULT_CAPACITY;
    }

    arr->data = malloc(elem_size * capacity);
    if (!arr->data) {
        free(arr);
        return NULL;
    }

    arr->elem_size = elem_size;
    arr->size = 0;
    arr->capacity = capacity;
    return arr;
}

void ds_darray_destroy(ds_darray_t* arr)
{
    if (!arr) {
        return;
    }
    free(arr->data);
    free(arr);
}

/* ------------------------------------------------------------------ */
/*  容量操作                                                           */
/* ------------------------------------------------------------------ */

int ds_darray_reserve(ds_darray_t* arr, size_t capacity)
{
    if (!arr) {
        return -1;
    }
    if (capacity <= arr->capacity) {
        return 0;  /* 容量足够，无需操作 */
    }
    void* new_data = realloc(arr->data, arr->elem_size * capacity);
    if (!new_data) {
        return -1;  /* 内存不足 */
    }
    arr->data = new_data;
    arr->capacity = capacity;
    return 0;
}

size_t ds_darray_size(const ds_darray_t* arr)
{
    return arr ? arr->size : 0;
}

size_t ds_darray_capacity(const ds_darray_t* arr)
{
    return arr ? arr->capacity : 0;
}

int ds_darray_empty(const ds_darray_t* arr)
{
    return (arr == NULL || arr->size == 0) ? 1 : 0;
}

int ds_darray_resize(ds_darray_t* arr, size_t new_size)
{
    if (!arr) {
        return -1;
    }
    if (new_size > arr->capacity) {
        /* 需要扩容 */
        if (ds_darray_reserve(arr, new_size) != 0) {
            return -1;
        }
    }
    if (new_size > arr->size) {
        /* 扩容部分清零 */
        memset((unsigned char*)arr->data + arr->size * arr->elem_size, 0,
               (new_size - arr->size) * arr->elem_size);
    }
    arr->size = new_size;
    return 0;
}

int ds_darray_shrink_to_fit(ds_darray_t* arr)
{
    if (!arr || arr->size == 0) {
        return -1;
    }
    if (arr->size >= arr->capacity) {
        return 0;  /* 已无闲置空间 */
    }
    void* new_data = realloc(arr->data, arr->elem_size * arr->size);
    if (!new_data) {
        return -1;  /* realloc 缩小通常不会失败，但依然检查 */
    }
    arr->data = new_data;
    arr->capacity = arr->size;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  元素访问                                                           */
/* ------------------------------------------------------------------ */

int ds_darray_get(const ds_darray_t* arr, size_t index, void* out)
{
    if (!arr || !out || index >= arr->size) {
        return -1;
    }
    memcpy(out, (const unsigned char*)arr->data + index * arr->elem_size,
           arr->elem_size);
    return 0;
}

int ds_darray_set(ds_darray_t* arr, size_t index, const void* elem)
{
    if (!arr || !elem || index >= arr->size) {
        return -1;
    }
    memcpy((unsigned char*)arr->data + index * arr->elem_size,
           elem, arr->elem_size);
    return 0;
}

void* ds_darray_data(ds_darray_t* arr)
{
    return arr ? arr->data : NULL;
}

int ds_darray_front(const ds_darray_t* arr, void* out)
{
    return ds_darray_get(arr, 0, out);
}

int ds_darray_back(const ds_darray_t* arr, void* out)
{
    if (!arr || arr->size == 0 || !out) {
        return -1;
    }
    return ds_darray_get(arr, arr->size - 1, out);
}

/* ------------------------------------------------------------------ */
/*  修改操作                                                           */
/* ------------------------------------------------------------------ */

int ds_darray_push_back(ds_darray_t* arr, const void* elem)
{
    if (!arr || !elem) {
        return -1;
    }
    /* 容量不足时自动扩容 */
    if (arr->size >= arr->capacity) {
        size_t new_cap = darray_calc_growth(arr->capacity);
        if (ds_darray_reserve(arr, new_cap) != 0) {
            return -1;
        }
    }
    memcpy((unsigned char*)arr->data + arr->size * arr->elem_size,
           elem, arr->elem_size);
    arr->size++;
    return 0;
}

int ds_darray_pop_back(ds_darray_t* arr, void* out)
{
    if (!arr || arr->size == 0) {
        return -1;
    }
    arr->size--;
    if (out) {
        memcpy(out, (const unsigned char*)arr->data + arr->size * arr->elem_size,
               arr->elem_size);
    }
    return 0;
}

int ds_darray_insert(ds_darray_t* arr, size_t index, const void* elem)
{
    if (!arr || !elem || index > arr->size) {
        return -1;
    }
    /* 容量不足时自动扩容 */
    if (arr->size >= arr->capacity) {
        size_t new_cap = darray_calc_growth(arr->capacity);
        if (ds_darray_reserve(arr, new_cap) != 0) {
            return -1;
        }
    }
    /* 将 index 之后的元素后移 */
    if (index < arr->size) {
        memmove((unsigned char*)arr->data + (index + 1) * arr->elem_size,
                (unsigned char*)arr->data + index * arr->elem_size,
                (arr->size - index) * arr->elem_size);
    }
    memcpy((unsigned char*)arr->data + index * arr->elem_size,
           elem, arr->elem_size);
    arr->size++;
    return 0;
}

int ds_darray_erase(ds_darray_t* arr, size_t index, void* out)
{
    if (!arr || index >= arr->size) {
        return -1;
    }
    if (out) {
        memcpy(out, (const unsigned char*)arr->data + index * arr->elem_size,
               arr->elem_size);
    }
    /* 将后面的元素前移 */
    if (index < arr->size - 1) {
        memmove((unsigned char*)arr->data + index * arr->elem_size,
                (unsigned char*)arr->data + (index + 1) * arr->elem_size,
                (arr->size - index - 1) * arr->elem_size);
    }
    arr->size--;
    return 0;
}

void ds_darray_clear(ds_darray_t* arr)
{
    if (arr) {
        arr->size = 0;
    }
}

/* ------------------------------------------------------------------ */
/*  迭代器                                                             */
/* ------------------------------------------------------------------ */

ds_darray_iter_t ds_darray_begin(const ds_darray_t* arr)
{
    ds_darray_iter_t iter = { arr, 0 };
    return iter;
}

ds_darray_iter_t ds_darray_end(const ds_darray_t* arr)
{
    ds_darray_iter_t iter = { arr, arr ? arr->size : 0 };
    return iter;
}

int ds_darray_iter_next(ds_darray_iter_t* iter)
{
    if (!iter || !iter->array || iter->index >= iter->array->size) {
        return -1;
    }
    iter->index++;
    return 0;
}

int ds_darray_iter_prev(ds_darray_iter_t* iter)
{
    if (!iter || iter->index == 0) {
        return -1;
    }
    iter->index--;
    return 0;
}

void* ds_darray_iter_data(const ds_darray_iter_t* iter)
{
    if (!iter || !iter->array || iter->index >= iter->array->size) {
        return NULL;
    }
    return (unsigned char*)iter->array->data + iter->index * iter->array->elem_size;
}

int ds_darray_iter_equal(const ds_darray_iter_t* a, const ds_darray_iter_t* b)
{
    if (!a || !b) {
        return 0;
    }
    return (a->array == b->array && a->index == b->index) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/*  查找与排序                                                         */
/* ------------------------------------------------------------------ */

int ds_darray_find(const ds_darray_t* arr, const void* key,
                   int (*cmp)(const void* elem, const void* key),
                   size_t* out_idx)
{
    if (!arr || !key || !cmp) {
        return -1;
    }
    for (size_t i = 0; i < arr->size; i++) {
        const void* elem = (const unsigned char*)arr->data + i * arr->elem_size;
        if (cmp(elem, key) == 0) {
            if (out_idx) {
                *out_idx = i;
            }
            return 0;
        }
    }
    return -1;
}

void ds_darray_sort(ds_darray_t* arr,
                    int (*compar)(const void* a, const void* b))
{
    if (!arr || !compar || arr->size < 2) {
        return;
    }
    qsort(arr->data, arr->size, arr->elem_size, compar);
}

int ds_darray_bsearch(const ds_darray_t* arr, const void* key,
                      int (*compar)(const void* a, const void* b),
                      size_t* out_idx)
{
    if (!arr || !key || !compar) {
        return -1;
    }
    void* result = bsearch(key, arr->data, arr->size, arr->elem_size, compar);
    if (!result) {
        return -1;
    }
    if (out_idx) {
        *out_idx = ((const unsigned char*)result - (const unsigned char*)arr->data)
                   / arr->elem_size;
    }
    return 0;
}
