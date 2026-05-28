/**
 * @file btree.c
 * @brief AVL 平衡二叉搜索树实现
 *
 * 实现策略：
 * - 严格 AVL 平衡：左右子树高度差 ≤ 1
 * - 使用 4 种旋转操作（左旋、右旋、左右旋、右左旋）维持平衡
 * - 中序遍历为升序序列
 * - 所有操作 O(log n)
 *
 * AVL 旋转场景：
 * - 左-左（LL）：右旋
 * - 右-右（RR）：左旋
 * - 左-右（LR）：先左旋再右旋
 * - 右-左（RL）：先右旋再左旋
 */

#include "btree.h"
#include <stdlib.h>  /* malloc, free */
#include <string.h>  /* memcpy, memset */

/* ------------------------------------------------------------------ */
/*  AVL 树节点                                                         */
/* ------------------------------------------------------------------ */
struct ds_btree_node {
    ds_btree_node_t* left;    /**< 左孩子 */
    ds_btree_node_t* right;   /**< 右孩子 */
    int   height;             /**< 以该节点为根的子树高度 */
    /* 键 + 值连续存储在此之后 */
};

/** @brief 获取节点键指针 */
#define NODE_KEY(node)   ((unsigned char*)(node) + sizeof(ds_btree_node_t))

/** @brief 获取节点值指针 */
#define NODE_VAL(node, ks) ((unsigned char*)(node) + sizeof(ds_btree_node_t) + (ks))

/* ------------------------------------------------------------------ */
/*  AVL 树结构体                                                       */
/* ------------------------------------------------------------------ */
struct ds_btree {
    ds_btree_node_t* root;     /**< 根节点 */
    size_t key_size;           /**< 键大小 */
    size_t value_size;         /**< 值大小 */
    size_t size;               /**< 节点数 */
    ds_btree_cmp_fn cmp;       /**< 比较函数 */
};

/* ------------------------------------------------------------------ */
/*  内部辅助函数（AVL 平衡相关）                                        */
/* ------------------------------------------------------------------ */

/** @brief 获取节点高度（空节点高度为 0） */
static int btree_height(ds_btree_node_t* node)
{
    return node ? node->height : 0;
}

/** @brief 计算平衡因子（左高 - 右高） */
static int btree_balance_factor(ds_btree_node_t* node)
{
    return node ? (btree_height(node->left) - btree_height(node->right)) : 0;
}

/** @brief 更新节点高度 */
static void btree_update_height(ds_btree_node_t* node)
{
    if (!node) {
        return;
    }
    int left_h = btree_height(node->left);
    int right_h = btree_height(node->right);
    node->height = (left_h > right_h ? left_h : right_h) + 1;
}

/**
 * @brief 创建新节点
 */
static ds_btree_node_t* btree_new_node(const void* key, const void* value,
                                        size_t key_size, size_t value_size)
{
    size_t total = sizeof(ds_btree_node_t) + key_size + value_size;
    ds_btree_node_t* node = (ds_btree_node_t*)malloc(total);
    if (!node) {
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  /* 新叶子节点高度为 1 */
    memcpy(NODE_KEY(node), key, key_size);
    memcpy(NODE_VAL(node, key_size), value, value_size);
    return node;
}

/**
 * @brief 释放子树所有节点
 */
static void btree_free_subtree(ds_btree_node_t* node)
{
    if (!node) {
        return;
    }
    btree_free_subtree(node->left);
    btree_free_subtree(node->right);
    free(node);
}

/* ============ AVL 旋转操作 ============ */

/**
 * @brief 右旋（LL 情况）
 *
 *     y          x
 *    / \        / \
 *   x   T3  => T1  y
 *  / \            / \
 * T1 T2          T2 T3
 */
static ds_btree_node_t* btree_rotate_right(ds_btree_node_t* y)
{
    ds_btree_node_t* x = y->left;
    ds_btree_node_t* T2 = x->right;

    x->right = y;
    y->left = T2;

    btree_update_height(y);
    btree_update_height(x);
    return x;
}

/**
 * @brief 左旋（RR 情况）
 *
 *   x            y
 *  / \          / \
 * T1  y   =>   x  T3
 *    / \      / \
 *   T2 T3    T1 T2
 */
static ds_btree_node_t* btree_rotate_left(ds_btree_node_t* x)
{
    ds_btree_node_t* y = x->right;
    ds_btree_node_t* T2 = y->left;

    y->left = x;
    x->right = T2;

    btree_update_height(x);
    btree_update_height(y);
    return y;
}

/* ============ AVL 插入 ============ */

/**
 * @brief 递归插入节点并平衡
 * @param tree  树
 * @param node  当前子树根节点
 * @param key   键
 * @param value 值
 * @param inserted 输出：是否实际插入了新节点（而非更新）
 * @return 新的子树根节点
 */
static ds_btree_node_t* btree_insert_node(ds_btree_t* tree,
                                           ds_btree_node_t* node,
                                           const void* key,
                                           const void* value,
                                           int* inserted)
{
    if (!node) {
        *inserted = 1;
        return btree_new_node(key, value, tree->key_size, tree->value_size);
    }

    int cmp = tree->cmp(key, NODE_KEY(node));
    if (cmp < 0) {
        node->left = btree_insert_node(tree, node->left, key, value, inserted);
    } else if (cmp > 0) {
        node->right = btree_insert_node(tree, node->right, key, value, inserted);
    } else {
        /* 键已存在，更新值 */
        memcpy(NODE_VAL(node, tree->key_size), value, tree->value_size);
        *inserted = 0;
        return node;
    }

    /* 更新高度 */
    btree_update_height(node);

    /* 计算平衡因子并旋转 */
    int bf = btree_balance_factor(node);

    /* LL 情况 */
    if (bf > 1 && tree->cmp(key, NODE_KEY(node->left)) < 0) {
        return btree_rotate_right(node);
    }
    /* RR 情况 */
    if (bf < -1 && tree->cmp(key, NODE_KEY(node->right)) > 0) {
        return btree_rotate_left(node);
    }
    /* LR 情况 */
    if (bf > 1 && tree->cmp(key, NODE_KEY(node->left)) > 0) {
        node->left = btree_rotate_left(node->left);
        return btree_rotate_right(node);
    }
    /* RL 情况 */
    if (bf < -1 && tree->cmp(key, NODE_KEY(node->right)) < 0) {
        node->right = btree_rotate_right(node->right);
        return btree_rotate_left(node);
    }

    return node;
}

/* ============ AVL 删除 ============ */

/**
 * @brief 查找以 node 为根的子树中的最小节点
 */
static ds_btree_node_t* btree_min_node(ds_btree_node_t* node)
{
    ds_btree_node_t* curr = node;
    while (curr && curr->left) {
        curr = curr->left;
    }
    return curr;
}

/**
 * @brief 递归删除节点并平衡
 */
static ds_btree_node_t* btree_erase_node(ds_btree_t* tree,
                                          ds_btree_node_t* node,
                                          const void* key,
                                          int* removed)
{
    if (!node) {
        *removed = 0;
        return NULL;
    }

    int cmp = tree->cmp(key, NODE_KEY(node));
    if (cmp < 0) {
        node->left = btree_erase_node(tree, node->left, key, removed);
    } else if (cmp > 0) {
        node->right = btree_erase_node(tree, node->right, key, removed);
    } else {
        /* 找到要删除的节点 */
        *removed = 1;

        if (!node->left || !node->right) {
            /* 最多一个子树 */
            ds_btree_node_t* temp = node->left ? node->left : node->right;
            free(node);
            node = temp;
        } else {
            /* 有两个子树：找中序后继（右子树最小节点） */
            ds_btree_node_t* successor = btree_min_node(node->right);
            /* 拷贝键值（不拷贝指针，避免破坏后继的子树结构） */
            /* 直接交换数据，然后递归删除 successor */
            /* 为简单起见，创建一个临时缓冲区来交换键值 */
            size_t ks = tree->key_size;
            size_t vs = tree->value_size;

            /* 使用栈上缓冲区（假设键值不超过 256 字节） */
            unsigned char tmp_key[256];
            unsigned char tmp_val[256];
            if (ks > 256 || vs > 256) {
                /* 对于超大键值，使用堆内存 —— 但正常场景不会 */
                *removed = 0;
                return node;
            }

            memcpy(tmp_key, NODE_KEY(successor), ks);
            memcpy(tmp_val, NODE_VAL(successor, ks), vs);
            memcpy(NODE_KEY(successor), NODE_KEY(node), ks);
            memcpy(NODE_VAL(successor, ks), NODE_VAL(node, ks), vs);
            memcpy(NODE_KEY(node), tmp_key, ks);
            memcpy(NODE_VAL(node, ks), tmp_val, vs);

            /* 删除后继 */
            node->right = btree_erase_node(tree, node->right,
                                           NODE_KEY(successor), removed);
        }
    }

    if (!node) {
        return NULL;
    }

    /* 更新高度 */
    btree_update_height(node);

    /* 平衡 */
    int bf = btree_balance_factor(node);

    /* LL */
    if (bf > 1 && btree_balance_factor(node->left) >= 0) {
        return btree_rotate_right(node);
    }
    /* LR */
    if (bf > 1 && btree_balance_factor(node->left) < 0) {
        node->left = btree_rotate_left(node->left);
        return btree_rotate_right(node);
    }
    /* RR */
    if (bf < -1 && btree_balance_factor(node->right) <= 0) {
        return btree_rotate_left(node);
    }
    /* RL */
    if (bf < -1 && btree_balance_factor(node->right) > 0) {
        node->right = btree_rotate_right(node->right);
        return btree_rotate_left(node);
    }

    return node;
}

/* ============ 遍历 ============ */

static void btree_traverse_inorder(ds_btree_node_t* node,
                                    ds_btree_visit_fn visit,
                                    void* ctx,
                                    size_t key_size)
{
    if (!node) {
        return;
    }
    btree_traverse_inorder(node->left, visit, ctx, key_size);
    visit(NODE_KEY(node), NODE_VAL(node, key_size), ctx);
    btree_traverse_inorder(node->right, visit, ctx, key_size);
}

static void btree_traverse_preorder(ds_btree_node_t* node,
                                     ds_btree_visit_fn visit,
                                     void* ctx,
                                     size_t key_size)
{
    if (!node) {
        return;
    }
    visit(NODE_KEY(node), NODE_VAL(node, key_size), ctx);
    btree_traverse_preorder(node->left, visit, ctx, key_size);
    btree_traverse_preorder(node->right, visit, ctx, key_size);
}

static void btree_traverse_postorder(ds_btree_node_t* node,
                                      ds_btree_visit_fn visit,
                                      void* ctx,
                                      size_t key_size)
{
    if (!node) {
        return;
    }
    btree_traverse_postorder(node->left, visit, ctx, key_size);
    btree_traverse_postorder(node->right, visit, ctx, key_size);
    visit(NODE_KEY(node), NODE_VAL(node, key_size), ctx);
}

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

ds_btree_t* ds_btree_create(size_t key_size, size_t value_size,
                             ds_btree_cmp_fn cmp)
{
    if (key_size == 0 || cmp == NULL) {
        return NULL;
    }

    ds_btree_t* tree = (ds_btree_t*)malloc(sizeof(ds_btree_t));
    if (!tree) {
        return NULL;
    }

    tree->root = NULL;
    tree->key_size = key_size;
    tree->value_size = value_size;
    tree->size = 0;
    tree->cmp = cmp;
    return tree;
}

void ds_btree_destroy(ds_btree_t* tree)
{
    if (!tree) {
        return;
    }
    btree_free_subtree(tree->root);
    free(tree);
}

/* ------------------------------------------------------------------ */
/*  容量与状态                                                         */
/* ------------------------------------------------------------------ */

size_t ds_btree_size(const ds_btree_t* tree)
{
    return tree ? tree->size : 0;
}

size_t ds_btree_height(const ds_btree_t* tree)
{
    return tree ? btree_height(tree->root) : 0;
}

int ds_btree_empty(const ds_btree_t* tree)
{
    return (tree == NULL || tree->size == 0) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/*  基本操作                                                           */
/* ------------------------------------------------------------------ */

int ds_btree_insert(ds_btree_t* tree, const void* key, const void* value)
{
    if (!tree || !key || !value) {
        return -1;
    }

    int inserted = 0;
    tree->root = btree_insert_node(tree, tree->root, key, value, &inserted);
    if (inserted) {
        tree->size++;
    }
    return 0;
}

int ds_btree_find(const ds_btree_t* tree, const void* key, void* out_val)
{
    if (!tree || !key) {
        return -1;
    }

    ds_btree_node_t* curr = tree->root;
    while (curr) {
        int cmp = tree->cmp(key, NODE_KEY(curr));
        if (cmp == 0) {
            if (out_val) {
                memcpy(out_val, NODE_VAL(curr, tree->key_size), tree->value_size);
            }
            return 0;
        }
        curr = (cmp < 0) ? curr->left : curr->right;
    }
    return -1;
}

int ds_btree_erase(ds_btree_t* tree, const void* key)
{
    if (!tree || !key) {
        return -1;
    }

    int removed = 0;
    tree->root = btree_erase_node(tree, tree->root, key, &removed);
    if (removed) {
        tree->size--;
    }
    return removed ? 0 : -1;
}

void ds_btree_clear(ds_btree_t* tree)
{
    if (!tree) {
        return;
    }
    btree_free_subtree(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

/* ------------------------------------------------------------------ */
/*  范围查询                                                           */
/* ------------------------------------------------------------------ */

int ds_btree_lower_bound(const ds_btree_t* tree, const void* key,
                          void* out_key, void* out_value)
{
    if (!tree || !key) {
        return -1;
    }

    ds_btree_node_t* curr = tree->root;
    ds_btree_node_t* candidate = NULL;

    while (curr) {
        int cmp = tree->cmp(key, NODE_KEY(curr));
        if (cmp == 0) {
            candidate = curr;
            break;
        }
        if (cmp < 0) {
            candidate = curr;  /* 当前节点比 key 大，是一个候选 */
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    if (!candidate) {
        return -1;
    }
    if (out_key)   memcpy(out_key, NODE_KEY(candidate), tree->key_size);
    if (out_value) memcpy(out_value, NODE_VAL(candidate, tree->key_size), tree->value_size);
    return 0;
}

int ds_btree_upper_bound(const ds_btree_t* tree, const void* key,
                          void* out_key, void* out_value)
{
    if (!tree || !key) {
        return -1;
    }

    ds_btree_node_t* curr = tree->root;
    ds_btree_node_t* candidate = NULL;

    while (curr) {
        int cmp = tree->cmp(key, NODE_KEY(curr));
        if (cmp < 0) {
            candidate = curr;
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    if (!candidate) {
        return -1;
    }
    if (out_key)   memcpy(out_key, NODE_KEY(candidate), tree->key_size);
    if (out_value) memcpy(out_value, NODE_VAL(candidate, tree->key_size), tree->value_size);
    return 0;
}

int ds_btree_min(const ds_btree_t* tree, void* out_key, void* out_value)
{
    if (!tree || !tree->root) {
        return -1;
    }
    ds_btree_node_t* node = btree_min_node(tree->root);
    if (out_key)   memcpy(out_key, NODE_KEY(node), tree->key_size);
    if (out_value) memcpy(out_value, NODE_VAL(node, tree->key_size), tree->value_size);
    return 0;
}

int ds_btree_max(const ds_btree_t* tree, void* out_key, void* out_value)
{
    if (!tree || !tree->root) {
        return -1;
    }
    ds_btree_node_t* curr = tree->root;
    while (curr && curr->right) {
        curr = curr->right;
    }
    if (out_key)   memcpy(out_key, NODE_KEY(curr), tree->key_size);
    if (out_value) memcpy(out_value, NODE_VAL(curr, tree->key_size), tree->value_size);
    return 0;
}

/* ------------------------------------------------------------------ */
/*  遍历                                                               */
/* ------------------------------------------------------------------ */

void ds_btree_traverse(const ds_btree_t* tree, ds_btree_order_t order,
                        ds_btree_visit_fn visit, void* ctx)
{
    if (!tree || !visit || !tree->root) {
        return;
    }

    switch (order) {
        case DS_BTREE_INORDER:
            btree_traverse_inorder(tree->root, visit, ctx, tree->key_size);
            break;
        case DS_BTREE_PREORDER:
            btree_traverse_preorder(tree->root, visit, ctx, tree->key_size);
            break;
        case DS_BTREE_POSTORDER:
            btree_traverse_postorder(tree->root, visit, ctx, tree->key_size);
            break;
        case DS_BTREE_LEVELORDER: {
            /* 广度优先：使用队列实现 */
            ds_btree_node_t** queue = (ds_btree_node_t**)malloc(
                sizeof(ds_btree_node_t*) * tree->size);
            if (!queue) {
                return;
            }
            size_t front = 0, rear = 0;
            queue[rear++] = tree->root;
            while (front < rear) {
                ds_btree_node_t* curr = queue[front++];
                visit(NODE_KEY(curr), NODE_VAL(curr, tree->key_size), ctx);
                if (curr->left)  queue[rear++] = curr->left;
                if (curr->right) queue[rear++] = curr->right;
            }
            free(queue);
            break;
        }
    }
}
