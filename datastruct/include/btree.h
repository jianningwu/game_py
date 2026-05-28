/**
 * @file btree.h
 * @brief AVL 平衡二叉搜索树（Balanced Binary Search Tree）
 *
 * 特性：
 * - 严格平衡（任意节点左右子树高度差 ≤ 1）
 * - 泛型键值存储
 * - 查找/插入/删除 均为 O(log n)
 * - 中序遍历得到有序序列
 * - 支持范围查询（前驱、后继）
 *
 * @author 飞机大战 EX Team
 * @version 1.0.0
 * @date 2026-05-27
 */

#ifndef DS_BTREE_H
#define DS_BTREE_H

#include <stddef.h>  /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  类型定义                                                           */
/* ------------------------------------------------------------------ */

/** @brief AVL 树结构体（不透明） */
typedef struct ds_btree ds_btree_t;

/** @brief AVL 树节点（不透明） */
typedef struct ds_btree_node ds_btree_node_t;

/**
 * @brief 键比较函数
 * @param a 键 a
 * @param b 键 b
 * @return 负数表示 a < b, 0 表示 a == b, 正数表示 a > b
 */
typedef int (*ds_btree_cmp_fn)(const void* a, const void* b);

/**
 * @brief 遍历回调函数
 * @param key   当前节点键指针
 * @param value 当前节点值指针
 * @param ctx   用户上下文
 */
typedef void (*ds_btree_visit_fn)(const void* key, const void* value, void* ctx);

/**
 * @brief 遍历顺序
 */
typedef enum {
    DS_BTREE_INORDER,   /**< 中序遍历（左-根-右）：升序 */
    DS_BTREE_PREORDER,  /**< 前序遍历（根-左-右） */
    DS_BTREE_POSTORDER, /**< 后序遍历（左-右-根） */
    DS_BTREE_LEVELORDER /**< 层序遍历（广度优先） */
} ds_btree_order_t;

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 创建 AVL 树
 * @param key_size   键大小（字节）
 * @param value_size 值大小（字节）
 * @param cmp        比较函数（不能为 NULL）
 * @return 成功返回树指针，失败返回 NULL
 */
ds_btree_t* ds_btree_create(size_t key_size, size_t value_size,
                             ds_btree_cmp_fn cmp);

/**
 * @brief 销毁 AVL 树
 * @param tree 树指针（允许 NULL）
 */
void ds_btree_destroy(ds_btree_t* tree);

/* ------------------------------------------------------------------ */
/*  容量与状态                                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取节点数
 */
size_t ds_btree_size(const ds_btree_t* tree);

/**
 * @brief 获取树高度
 */
size_t ds_btree_height(const ds_btree_t* tree);

/**
 * @brief 判断树是否为空
 * @return 非 0 表示空
 */
int ds_btree_empty(const ds_btree_t* tree);

/* ------------------------------------------------------------------ */
/*  基本操作                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 插入（或更新）键值对
 * @param tree  树指针
 * @param key   键指针
 * @param value 值指针
 * @return 0 成功，-1 失败
 *
 * @note 若键已存在，更新对应的值。
 */
int ds_btree_insert(ds_btree_t* tree, const void* key, const void* value);

/**
 * @brief 查找指定键的值
 * @param tree     树指针
 * @param key      键指针
 * @param out_val  输出值缓冲区（可为 NULL，仅检查存在性）
 * @return 0 找到，-1 未找到
 */
int ds_btree_find(const ds_btree_t* tree, const void* key, void* out_val);

/**
 * @brief 删除指定键
 * @param tree 树指针
 * @param key  键指针
 * @return 0 成功删除，-1 键不存在
 */
int ds_btree_erase(ds_btree_t* tree, const void* key);

/**
 * @brief 清空树
 * @param tree 树指针
 */
void ds_btree_clear(ds_btree_t* tree);

/* ------------------------------------------------------------------ */
/*  范围查询                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 查找不小于 key 的最小节点（上界）
 * @param tree     树指针
 * @param key      键指针
 * @param out_key   输出键（可为 NULL）
 * @param out_value 输出值（可为 NULL）
 * @return 0 找到，-1 未找到
 */
int ds_btree_lower_bound(const ds_btree_t* tree, const void* key,
                          void* out_key, void* out_value);

/**
 * @brief 查找大于 key 的最小节点（严格上界）
 */
int ds_btree_upper_bound(const ds_btree_t* tree, const void* key,
                          void* out_key, void* out_value);

/**
 * @brief 获取最小键
 * @return 0 找到，-1 树空
 */
int ds_btree_min(const ds_btree_t* tree, void* out_key, void* out_value);

/**
 * @brief 获取最大键
 * @return 0 找到，-1 树空
 */
int ds_btree_max(const ds_btree_t* tree, void* out_key, void* out_value);

/* ------------------------------------------------------------------ */
/*  遍历                                                               */
/* ------------------------------------------------------------------ */

/**
 * @brief 遍历树
 * @param tree  树指针
 * @param order 遍历顺序
 * @param visit 回调函数
 * @param ctx   用户上下文（传入 visit 的第三个参数）
 */
void ds_btree_traverse(const ds_btree_t* tree, ds_btree_order_t order,
                        ds_btree_visit_fn visit, void* ctx);

#ifdef __cplusplus
}
#endif

#endif /* DS_BTREE_H */
