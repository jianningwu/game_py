/**
 * @file darray.h
 * @brief 通用动态数组（Dynamic Array）
 *
 * 特性：
 * - 存储任意类型元素（通过 memcpy 拷贝语义）
 * - 自动扩容（倍增策略，因子 1.5）
 * - 随机访问 O(1)，尾部插入均摊 O(1)
 * - 提供迭代器接口
 * - 所有操作返回状态码，便于错误处理
 *
 * @author 飞机大战 EX Team
 * @version 1.0.0
 * @date 2026-05-27
 */

#ifndef DS_DARRAY_H
#define DS_DARRAY_H

#include <stddef.h>  /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  类型定义                                                           */
/* ------------------------------------------------------------------ */

/** @brief 动态数组结构体（不透明类型，用户通过指针操作） */
typedef struct ds_darray ds_darray_t;

/** @brief 动态数组迭代器 */
typedef struct {
    const ds_darray_t* array;  /**< 所属数组 */
    size_t              index; /**< 当前位置 */
} ds_darray_iter_t;

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 创建动态数组
 * @param elem_size  元素大小（字节），必须 > 0
 * @param capacity   初始容量（元素个数），填 0 使用默认值 16
 * @return 成功返回数组指针，失败返回 NULL
 *
 * @note 即使 capacity = 0，也会分配默认初始容量。
 *       当 elem_size = 0 时返回 NULL。
 */
ds_darray_t* ds_darray_create(size_t elem_size, size_t capacity);

/**
 * @brief 销毁动态数组，释放所有内存
 * @param arr 数组指针（允许 NULL）
 *
 * @note 数组存储的是原始内存拷贝，不会调用元素析构函数。
 *       若元素包含堆指针，需在销毁前自行处理。
 */
void ds_darray_destroy(ds_darray_t* arr);

/* ------------------------------------------------------------------ */
/*  容量操作                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 预分配容量，避免频繁扩容
 * @param arr      数组指针
 * @param capacity 新容量（元素个数）
 * @return 0 成功，-1 失败（内存不足）
 *
 * @note 若 new_capacity <= 当前容量，无操作成功返回。
 *       若 new_capacity < 当前元素个数，返回 -1 不改变容量。
 */
int ds_darray_reserve(ds_darray_t* arr, size_t capacity);

/**
 * @brief 获取当前元素个数
 */
size_t ds_darray_size(const ds_darray_t* arr);

/**
 * @brief 获取当前容量
 */
size_t ds_darray_capacity(const ds_darray_t* arr);

/**
 * @brief 判断数组是否为空
 * @return 非 0 表示为空，0 表示非空
 */
int ds_darray_empty(const ds_darray_t* arr);

/**
 * @brief 调整元素个数
 *
 * 若 new_size > 当前大小，新元素用 0 填充。
 * 若 new_size < 当前大小，超出部分被丢弃。
 *
 * @param arr      数组指针
 * @param new_size 新元素个数
 * @return 0 成功，-1 失败
 */
int ds_darray_resize(ds_darray_t* arr, size_t new_size);

/**
 * @brief 收缩容量以匹配实际元素个数
 * @return 0 成功，-1 失败
 */
int ds_darray_shrink_to_fit(ds_darray_t* arr);

/* ------------------------------------------------------------------ */
/*  元素访问                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取指定位置的元素
 * @param arr   数组指针
 * @param index 索引（从 0 开始）
 * @param out   输出缓冲区（不能为 NULL）
 * @return 0 成功，-1 索引越界
 */
int ds_darray_get(const ds_darray_t* arr, size_t index, void* out);

/**
 * @brief 设置指定位置的元素
 * @param arr   数组指针
 * @param index 索引（从 0 开始）
 * @param elem  元素指针（不能为 NULL）
 * @return 0 成功，-1 索引越界
 */
int ds_darray_set(ds_darray_t* arr, size_t index, const void* elem);

/**
 * @brief 获取指向底层连续内存的指针
 * @param arr 数组指针
 * @return 数据指针，可用于 memcpy 等直接操作
 *
 * @warning 返回的指针在扩容后可能失效。
 *          使用前建议先确认容量足够。
 */
void* ds_darray_data(ds_darray_t* arr);

/**
 * @brief 获取第一个元素
 * @return 0 成功，-1 数组为空
 */
int ds_darray_front(const ds_darray_t* arr, void* out);

/**
 * @brief 获取最后一个元素
 * @return 0 成功，-1 数组为空
 */
int ds_darray_back(const ds_darray_t* arr, void* out);

/* ------------------------------------------------------------------ */
/*  修改操作                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 尾部插入元素
 * @param arr  数组指针
 * @param elem 元素指针（不能为 NULL）
 * @return 0 成功，-1 失败
 *
 * @note 当容量不足时自动扩容。
 *       均摊时间复杂度 O(1)。
 */
int ds_darray_push_back(ds_darray_t* arr, const void* elem);

/**
 * @brief 删除尾部元素
 * @param arr 数组指针
 * @param out 输出缓冲区（可为 NULL，表示丢弃该元素）
 * @return 0 成功，-1 数组为空
 */
int ds_darray_pop_back(ds_darray_t* arr, void* out);

/**
 * @brief 在指定位置插入元素
 * @param arr   数组指针
 * @param index 插入位置（将插入到该索引之前）
 * @param elem  元素指针
 * @return 0 成功，-1 失败
 *
 * @note 插入点之后所有元素向后移动。
 *       时间复杂度 O(n)。
 */
int ds_darray_insert(ds_darray_t* arr, size_t index, const void* elem);

/**
 * @brief 删除指定位置的元素
 * @param arr   数组指针
 * @param index 要删除的位置
 * @param out   输出缓冲区（可为 NULL）
 * @return 0 成功，-1 索引越界
 */
int ds_darray_erase(ds_darray_t* arr, size_t index, void* out);

/**
 * @brief 清空数组
 * @param arr 数组指针
 *
 * @note 不释放底层内存，仅重置大小。
 *       后续 push 可复用已有容量。
 */
void ds_darray_clear(ds_darray_t* arr);

/* ------------------------------------------------------------------ */
/*  迭代器                                                             */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取指向第一个元素的迭代器
 * @return 迭代器。若数组为空，返回的迭代器等于 end()
 */
ds_darray_iter_t ds_darray_begin(const ds_darray_t* arr);

/**
 * @brief 获取末尾哨兵迭代器（不可解引用）
 * @return 迭代器
 */
ds_darray_iter_t ds_darray_end(const ds_darray_t* arr);

/**
 * @brief 迭代器前进到下一个元素
 * @param iter 迭代器指针
 * @return 0 成功，-1 已到达末尾
 */
int ds_darray_iter_next(ds_darray_iter_t* iter);

/**
 * @brief 迭代器后退到上一个元素
 * @param iter 迭代器指针
 * @return 0 成功，-1 已到达开头
 */
int ds_darray_iter_prev(ds_darray_iter_t* iter);

/**
 * @brief 获取迭代器指向的数据
 * @param iter 迭代器指针
 * @return 数据指针，若迭代器无效返回 NULL
 */
void* ds_darray_iter_data(const ds_darray_iter_t* iter);

/**
 * @brief 判断两个迭代器是否相等
 * @return 非 0 表示相等，0 表示不等
 */
int ds_darray_iter_equal(const ds_darray_iter_t* a, const ds_darray_iter_t* b);

/* ------------------------------------------------------------------ */
/*  查找与排序                                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief 线性查找第一个匹配元素
 * @param arr    数组指针
 * @param key    查找目标
 * @param cmp    比较函数，返回 0 表示相等
 * @param out_idx 输出找到的索引（可为 NULL）
 * @return 0 找到，-1 未找到
 *
 * @note 时间复杂度 O(n)。对于已排序数组建议使用二分查找。
 */
int ds_darray_find(const ds_darray_t* arr, const void* key,
                   int (*cmp)(const void* elem, const void* key),
                   size_t* out_idx);

/**
 * @brief 对数组排序
 * @param arr     数组指针
 * @param compar  比较函数（与 qsort 兼容）
 *
 * @note 使用标准库 qsort 实现。
 *       时间复杂度 O(n log n)。
 */
void ds_darray_sort(ds_darray_t* arr,
                    int (*compar)(const void* a, const void* b));

/**
 * @brief 二分查找（数组必须已排序）
 * @param arr    数组指针
 * @param key    查找目标
 * @param compar 比较函数
 * @param out_idx 输出找到的索引（可为 NULL）
 * @return 0 找到，-1 未找到
 */
int ds_darray_bsearch(const ds_darray_t* arr, const void* key,
                      int (*compar)(const void* a, const void* b),
                      size_t* out_idx);

#ifdef __cplusplus
}
#endif

#endif /* DS_DARRAY_H */
