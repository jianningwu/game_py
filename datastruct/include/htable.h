/**
 * @file htable.h
 * @brief 通用哈希表（Hash Table）
 *
 * 特性：
 * - 链地址法解决哈希冲突
 * - 泛型键值存储（任意类型可通过 memcpy 拷贝）
 * - 支持自定义哈希函数和相等比较函数
 * - 自动扩容（负载因子 > 0.75 时自动 rehash）
 * - 内置常用类型的哈希函数（整数、字符串）
 * - 提供迭代器遍历
 *
 * @author 飞机大战 EX Team
 * @version 1.0.0
 * @date 2026-05-27
 */

#ifndef DS_HTABLE_H
#define DS_HTABLE_H

#include <stddef.h>  /* size_t */
#include <stdint.h>  /* uint64_t */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  类型定义                                                           */
/* ------------------------------------------------------------------ */

/** @brief 哈希表结构体（不透明） */
typedef struct ds_htable ds_htable_t;

/** @brief 哈希表迭代器 */
typedef struct {
    ds_htable_t* table;      /**< 所属哈希表 */
    size_t       bucket_idx; /**< 当前桶索引 */
    void*        entry;      /**< 当前桶中链表的节点 */
} ds_htable_iter_t;

/**
 * @brief 哈希函数类型
 * @param key 键指针
 * @return 64 位哈希值
 */
typedef uint64_t (*ds_hash_fn)(const void* key);

/**
 * @brief 键相等比较函数
 * @param a 键 a
 * @param b 键 b
 * @return 0 表示相等，非 0 表示不等
 */
typedef int (*ds_eq_fn)(const void* a, const void* b);

/* ------------------------------------------------------------------ */
/*  内置哈希函数                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 整数键（int）哈希函数
 * @note 键指针指向 int 类型
 */
uint64_t ds_hash_int(const void* key);

/**
 * @brief 字符串键（以 '\0' 结尾）哈希函数
 * @note 使用 DJB2 算法
 */
uint64_t ds_hash_str(const void* key);

/**
 * @brief 整数键相等比较
 */
int ds_eq_int(const void* a, const void* b);

/**
 * @brief 字符串键相等比较（strcmp）
 */
int ds_eq_str(const void* a, const void* b);

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 创建哈希表
 * @param key_size    键大小（字节）
 * @param value_size  值大小（字节）
 * @param capacity    初始桶数（填 0 使用默认值 64）
 * @param hash_fn     哈希函数（若为 NULL 使用默认的整数哈希）
 * @param eq_fn       相等比较函数（若为 NULL 使用 memcmp）
 * @return 成功返回哈希表指针，失败返回 NULL
 */
ds_htable_t* ds_htable_create(size_t key_size, size_t value_size,
                               size_t capacity,
                               ds_hash_fn hash_fn,
                               ds_eq_fn eq_fn);

/**
 * @brief 销毁哈希表，释放所有内存
 * @param ht 哈希表指针（允许 NULL）
 */
void ds_htable_destroy(ds_htable_t* ht);

/* ------------------------------------------------------------------ */
/*  容量操作                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取哈希表中元素个数
 */
size_t ds_htable_size(const ds_htable_t* ht);

/**
 * @brief 获取哈希表桶数
 */
size_t ds_htable_bucket_count(const ds_htable_t* ht);

/**
 * @brief 判断哈希表是否为空
 */
int ds_htable_empty(const ds_htable_t* ht);

/**
 * @brief 手动触发 rehash，调整桶数
 * @param ht        哈希表指针
 * @param new_capacity 新桶数（填 0 自动计算合理值）
 * @return 0 成功，-1 失败
 */
int ds_htable_rehash(ds_htable_t* ht, size_t new_capacity);

/* ------------------------------------------------------------------ */
/*  元素操作                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 插入（或更新）键值对
 * @param ht    哈希表指针
 * @param key   键指针
 * @param value 值指针
 * @return 0 成功，-1 失败
 *
 * @note 若键已存在，更新对应的值。
 */
int ds_htable_insert(ds_htable_t* ht, const void* key, const void* value);

/**
 * @brief 查找指定键的值
 * @param ht       哈希表指针
 * @param key      键指针
 * @param out_val  输出值缓冲区（不能为 NULL）
 * @return 0 找到，-1 未找到
 */
int ds_htable_find(const ds_htable_t* ht, const void* key, void* out_val);

/**
 * @brief 删除指定键
 * @param ht  哈希表指针
 * @param key 键指针
 * @return 0 成功删除，-1 键不存在
 */
int ds_htable_erase(ds_htable_t* ht, const void* key);

/**
 * @brief 清空哈希表
 * @param ht 哈希表指针
 */
void ds_htable_clear(ds_htable_t* ht);

/* ------------------------------------------------------------------ */
/*  迭代器                                                             */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取指向第一个元素的迭代器
 */
ds_htable_iter_t ds_htable_begin(ds_htable_t* ht);

/**
 * @brief 获取末尾哨兵迭代器
 */
ds_htable_iter_t ds_htable_end(ds_htable_t* ht);

/**
 * @brief 迭代器前进到下一个元素
 * @return 0 成功，-1 已到达末尾
 */
int ds_htable_iter_next(ds_htable_iter_t* iter);

/**
 * @brief 获取迭代器指向的键
 * @return 键指针，无效迭代器返回 NULL
 */
void* ds_htable_iter_key(const ds_htable_iter_t* iter);

/**
 * @brief 获取迭代器指向的值
 * @return 值指针，无效迭代器返回 NULL
 */
void* ds_htable_iter_value(const ds_htable_iter_t* iter);

/**
 * @brief 判断两个迭代器是否相等
 */
int ds_htable_iter_equal(const ds_htable_iter_t* a, const ds_htable_iter_t* b);

#ifdef __cplusplus
}
#endif

#endif /* DS_HTABLE_H */
