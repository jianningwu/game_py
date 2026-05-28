/**
 * @file htable.c
 * @brief 哈希表实现（链地址法）
 *
 * 实现策略：
 * - 链地址法：每个桶是一个单向链表
 * - 负载因子 > 0.75 时自动 rehash（桶数翻倍）
 * - 内置整数和字符串哈希函数
 * - 支持 NULL 作为哈希/比较函数，自动选择默认实现
 */

#include "htable.h"
#include <stdlib.h>  /* malloc, free, rand, srand */
#include <string.h>  /* memcpy, memset, memcmp */
#include <time.h>     /* time */

/* ------------------------------------------------------------------ */
/*  内部常量                                                           */
/* ------------------------------------------------------------------ */

/** @brief 默认桶数 */
#define HTABLE_DEFAULT_CAPACITY 64

/** @brief 最大负载因子（超过此值触发 rehash） */
#define HTABLE_MAX_LOAD_FACTOR 0.75

/** @brief 哈希表扩容倍数 */
#define HTABLE_GROWTH_FACTOR 2

/* ------------------------------------------------------------------ */
/*  哈希表条目（单链表节点）                                            */
/* ------------------------------------------------------------------ */
typedef struct htable_entry {
    struct htable_entry* next;  /**< 链表下一个节点 */
    /* 键 + 值连续存储在此之后 */
} htable_entry_t;

/** @brief 获取条目键指针 */
#define ENTRY_KEY(entry)   ((unsigned char*)(entry) + sizeof(htable_entry_t))

/** @brief 获取条目值指针 */
#define ENTRY_VAL(entry, ks) ((unsigned char*)(entry) + sizeof(htable_entry_t) + (ks))

/* ------------------------------------------------------------------ */
/*  哈希表结构体                                                       */
/* ------------------------------------------------------------------ */
struct ds_htable {
    htable_entry_t** buckets;   /**< 桶数组 */
    size_t    bucket_count;     /**< 桶数 */
    size_t    size;             /**< 元素个数 */
    size_t    key_size;         /**< 键大小 */
    size_t    value_size;       /**< 值大小 */
    ds_hash_fn hash_fn;         /**< 哈希函数 */
    ds_eq_fn   eq_fn;           /**< 相等比较函数 */
};

/* ------------------------------------------------------------------ */
/*  内置哈希函数                                                       */
/* ------------------------------------------------------------------ */

uint64_t ds_hash_int(const void* key)
{
    /* Thomas Wang 整数哈希 */
    uint64_t h = *(const int*)key;
    h = (~h) + (h << 21);
    h ^= (h >> 24);
    h = (h + (h << 3)) + (h << 8);
    h ^= (h >> 14);
    h = (h + (h << 2)) + (h << 4);
    h ^= (h >> 28);
    h = h + (h << 31);
    return h;
}

uint64_t ds_hash_str(const void* key)
{
    /* DJB2 哈希算法 */
    const unsigned char* str = (const unsigned char*)key;
    uint64_t hash = 5381;
    int c;
    while ((c = *str++) != 0) {
        hash = ((hash << 5) + hash) + (uint64_t)c;
    }
    return hash;
}

int ds_eq_int(const void* a, const void* b)
{
    return (*(const int*)a == *(const int*)b) ? 0 : -1;
}

int ds_eq_str(const void* a, const void* b)
{
    return strcmp((const char*)a, (const char*)b);
}

/* ------------------------------------------------------------------ */
/*  内部辅助                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 默认哈希函数（逐字节哈希，用于未知类型）
 */
static uint64_t htable_default_hash(const void* key, size_t key_size)
{
    /* FNV-1a 变体 */
    const unsigned char* p = (const unsigned char*)key;
    uint64_t h = 14695981039346656037ULL;
    for (size_t i = 0; i < key_size; i++) {
        h ^= (uint64_t)p[i];
        h *= 1099511628211ULL;
    }
    return h;
}

/**
 * @brief 默认相等比较（memcmp）
 */
static int htable_default_eq(const void* a, const void* b, size_t key_size)
{
    return memcmp(a, b, key_size);
}

/**
 * @brief 对键应用哈希函数，返回桶索引
 */
static size_t htable_hash(const ds_htable_t* ht, const void* key)
{
    uint64_t h;
    if (ht->hash_fn) {
        h = ht->hash_fn(key);
    } else {
        h = htable_default_hash(key, ht->key_size);
    }
    return (size_t)(h % ht->bucket_count);
}

/**
 * @brief 比较两个键是否相等
 */
static int htable_eq(const ds_htable_t* ht, const void* a, const void* b)
{
    if (ht->eq_fn) {
        return ht->eq_fn(a, b);
    }
    return htable_default_eq(a, b, ht->key_size);
}

/**
 * @brief 在指定桶中查找键
 * @param ht     哈希表
 * @param bucket 桶索引
 * @param key    要查找的键
 * @param out_prev 输出前驱节点指针的地址（用于删除）
 * @return 找到的条目，未找到返回 NULL
 */
static htable_entry_t* htable_find_in_bucket(const ds_htable_t* ht,
                                              size_t bucket,
                                              const void* key,
                                              htable_entry_t*** out_prev)
{
    htable_entry_t* entry = ht->buckets[bucket];
    htable_entry_t** prev = &ht->buckets[bucket];

    while (entry) {
        if (htable_eq(ht, ENTRY_KEY(entry), key) == 0) {
            if (out_prev) {
                *out_prev = prev;
            }
            return entry;
        }
        prev = &entry->next;
        entry = entry->next;
    }
    return NULL;
}

/**
 * @brief 创建新条目
 */
static htable_entry_t* htable_new_entry(ds_htable_t* ht,
                                         const void* key,
                                         const void* value)
{
    size_t entry_size = sizeof(htable_entry_t) + ht->key_size + ht->value_size;
    htable_entry_t* entry = (htable_entry_t*)malloc(entry_size);
    if (!entry) {
        return NULL;
    }
    entry->next = NULL;
    memcpy(ENTRY_KEY(entry), key, ht->key_size);
    memcpy(ENTRY_VAL(entry, ht->key_size), value, ht->value_size);
    return entry;
}

/**
 * @brief 释放所有桶中的条目
 */
static void htable_free_buckets(htable_entry_t** buckets, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        htable_entry_t* entry = buckets[i];
        while (entry) {
            htable_entry_t* next = entry->next;
            free(entry);
            entry = next;
        }
    }
}

/**
 * @brief 执行 rehash（扩容或缩容）
 */
static int htable_do_rehash(ds_htable_t* ht, size_t new_bucket_count)
{
    if (new_bucket_count < 4) {
        new_bucket_count = 4;
    }

    htable_entry_t** new_buckets =
        (htable_entry_t**)calloc(new_bucket_count, sizeof(htable_entry_t*));
    if (!new_buckets) {
        return -1;
    }

    /* 重新分配所有条目 */
    size_t old_count = ht->bucket_count;
    htable_entry_t** old_buckets = ht->buckets;

    ht->buckets = new_buckets;
    ht->bucket_count = new_bucket_count;

    for (size_t i = 0; i < old_count; i++) {
        htable_entry_t* entry = old_buckets[i];
        while (entry) {
            htable_entry_t* next = entry->next;
            /* 在新表中计算桶位置，头插法 */
            size_t new_bucket = htable_hash(ht, ENTRY_KEY(entry));
            entry->next = ht->buckets[new_bucket];
            ht->buckets[new_bucket] = entry;
            entry = next;
        }
    }

    free(old_buckets);
    return 0;
}

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

ds_htable_t* ds_htable_create(size_t key_size, size_t value_size,
                               size_t capacity,
                               ds_hash_fn hash_fn,
                               ds_eq_fn eq_fn)
{
    if (key_size == 0) {
        return NULL;
    }

    ds_htable_t* ht = (ds_htable_t*)malloc(sizeof(ds_htable_t));
    if (!ht) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = HTABLE_DEFAULT_CAPACITY;
    }

    ht->buckets = (htable_entry_t**)calloc(capacity, sizeof(htable_entry_t*));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }

    ht->bucket_count = capacity;
    ht->size = 0;
    ht->key_size = key_size;
    ht->value_size = value_size;
    ht->hash_fn = hash_fn;
    ht->eq_fn = eq_fn;
    return ht;
}

void ds_htable_destroy(ds_htable_t* ht)
{
    if (!ht) {
        return;
    }
    htable_free_buckets(ht->buckets, ht->bucket_count);
    free(ht->buckets);
    free(ht);
}

/* ------------------------------------------------------------------ */
/*  容量操作                                                           */
/* ------------------------------------------------------------------ */

size_t ds_htable_size(const ds_htable_t* ht)
{
    return ht ? ht->size : 0;
}

size_t ds_htable_bucket_count(const ds_htable_t* ht)
{
    return ht ? ht->bucket_count : 0;
}

int ds_htable_empty(const ds_htable_t* ht)
{
    return (ht == NULL || ht->size == 0) ? 1 : 0;
}

int ds_htable_rehash(ds_htable_t* ht, size_t new_capacity)
{
    if (!ht) {
        return -1;
    }
    if (new_capacity == 0) {
        /* 自动计算：使负载因子约为 0.5 */
        new_capacity = (size_t)(ht->size / 0.5) + 1;
        if (new_capacity < 4) {
            new_capacity = 4;
        }
    }
    return htable_do_rehash(ht, new_capacity);
}

/* ------------------------------------------------------------------ */
/*  元素操作                                                           */
/* ------------------------------------------------------------------ */

int ds_htable_insert(ds_htable_t* ht, const void* key, const void* value)
{
    if (!ht || !key || !value) {
        return -1;
    }

    /* 检查是否需要 rehash */
    if ((double)ht->size / (double)ht->bucket_count > HTABLE_MAX_LOAD_FACTOR) {
        if (htable_do_rehash(ht, ht->bucket_count * HTABLE_GROWTH_FACTOR) != 0) {
            return -1;
        }
    }

    size_t bucket = htable_hash(ht, key);
    htable_entry_t* existing = htable_find_in_bucket(ht, bucket, key, NULL);
    if (existing) {
        /* 键已存在，更新值 */
        memcpy(ENTRY_VAL(existing, ht->key_size), value, ht->value_size);
        return 0;
    }

    /* 创建新条目，头插法 */
    htable_entry_t* entry = htable_new_entry(ht, key, value);
    if (!entry) {
        return -1;
    }
    entry->next = ht->buckets[bucket];
    ht->buckets[bucket] = entry;
    ht->size++;
    return 0;
}

int ds_htable_find(const ds_htable_t* ht, const void* key, void* out_val)
{
    if (!ht || !key || !out_val) {
        return -1;
    }
    size_t bucket = htable_hash(ht, key);
    htable_entry_t* entry = htable_find_in_bucket(ht, bucket, key, NULL);
    if (!entry) {
        return -1;
    }
    memcpy(out_val, ENTRY_VAL(entry, ht->key_size), ht->value_size);
    return 0;
}

int ds_htable_erase(ds_htable_t* ht, const void* key)
{
    if (!ht || !key) {
        return -1;
    }
    size_t bucket = htable_hash(ht, key);
    htable_entry_t** prev_ptr = NULL;
    htable_entry_t* entry = htable_find_in_bucket(ht, bucket, key, &prev_ptr);
    if (!entry) {
        return -1;
    }
    /* 从链表中摘除 */
    *prev_ptr = entry->next;
    free(entry);
    ht->size--;
    return 0;
}

void ds_htable_clear(ds_htable_t* ht)
{
    if (!ht) {
        return;
    }
    htable_free_buckets(ht->buckets, ht->bucket_count);
    memset(ht->buckets, 0, ht->bucket_count * sizeof(htable_entry_t*));
    ht->size = 0;
}

/* ------------------------------------------------------------------ */
/*  迭代器                                                             */
/* ------------------------------------------------------------------ */

ds_htable_iter_t ds_htable_begin(ds_htable_t* ht)
{
    ds_htable_iter_t iter = { ht, 0, NULL };
    if (!ht || ht->size == 0) {
        return iter;
    }
    /* 找到第一个非空桶 */
    for (size_t i = 0; i < ht->bucket_count; i++) {
        if (ht->buckets[i]) {
            iter.bucket_idx = i;
            iter.entry = ht->buckets[i];
            break;
        }
    }
    return iter;
}

ds_htable_iter_t ds_htable_end(ds_htable_t* ht)
{
    ds_htable_iter_t iter = { ht, ht ? ht->bucket_count : 0, NULL };
    return iter;
}

int ds_htable_iter_next(ds_htable_iter_t* iter)
{
    if (!iter || !iter->table || !iter->entry) {
        return -1;
    }
    htable_entry_t* entry = (htable_entry_t*)iter->entry;
    /* 尝试当前链表的下一个节点 */
    if (entry->next) {
        iter->entry = entry->next;
        return 0;
    }
    /* 当前链表结束，找下一个非空桶 */
    for (size_t i = iter->bucket_idx + 1; i < iter->table->bucket_count; i++) {
        if (iter->table->buckets[i]) {
            iter->bucket_idx = i;
            iter->entry = iter->table->buckets[i];
            return 0;
        }
    }
    /* 没有更多元素了 */
    iter->bucket_idx = iter->table->bucket_count;
    iter->entry = NULL;
    return -1;
}

void* ds_htable_iter_key(const ds_htable_iter_t* iter)
{
    if (!iter || !iter->entry) {
        return NULL;
    }
    return ENTRY_KEY(iter->entry);
}

void* ds_htable_iter_value(const ds_htable_iter_t* iter)
{
    if (!iter || !iter->entry) {
        return NULL;
    }
    return ENTRY_VAL(iter->entry, iter->table->key_size);
}

int ds_htable_iter_equal(const ds_htable_iter_t* a, const ds_htable_iter_t* b)
{
    if (!a || !b) {
        return 0;
    }
    return (a->table == b->table && a->entry == b->entry) ? 1 : 0;
}
