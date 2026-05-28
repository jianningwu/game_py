/**
 * @file test_btree.c
 * @brief AVL 树单元测试
 */
#include "btree.h"
#include <stdio.h>
#include <string.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) do { printf("  TEST: %s ... ", name); } while(0)
#define PASS() do { printf("PASS\n"); tests_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); tests_failed++; } while(0)
#define ASSERT(cond, msg) do { if (!(cond)) { FAIL(msg); return; } } while(0)

static int cmp_int(const void* a, const void* b)
{
    int va = *(const int*)a;
    int vb = *(const int*)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

static void test_create_destroy(void)
{
    TEST("create and destroy");
    ds_btree_t* tree = ds_btree_create(sizeof(int), sizeof(int), cmp_int);
    ASSERT(tree != NULL, "create failed");
    ASSERT(ds_btree_size(tree) == 0, "size 0");
    ASSERT(ds_btree_empty(tree), "empty");
    ASSERT(ds_btree_height(tree) == 0, "height 0");
    ds_btree_destroy(tree);
    ds_btree_destroy(NULL);
    PASS();
}

static void test_insert_find(void)
{
    TEST("insert and find");
    ds_btree_t* tree = ds_btree_create(sizeof(int), sizeof(int), cmp_int);

    for (int i = 0; i < 100; i++) {
        int val = i * 2;
        ASSERT(ds_btree_insert(tree, &i, &val) == 0, "insert");
    }
    ASSERT(ds_btree_size(tree) == 100, "size 100");

    for (int i = 0; i < 100; i++) {
        int val;
        ASSERT(ds_btree_find(tree, &i, &val) == 0, "find");
        ASSERT(val == i * 2, "value match");
    }

    ds_btree_destroy(tree);
    PASS();
}

static void test_erase(void)
{
    TEST("erase");
    ds_btree_t* tree = ds_btree_create(sizeof(int), sizeof(int), cmp_int);

    for (int i = 0; i < 20; i++) {
        ds_btree_insert(tree, &i, &i);
    }
    ASSERT(ds_btree_size(tree) == 20, "size before erase");

    int key = 10;
    ASSERT(ds_btree_erase(tree, &key) == 0, "erase");
    ASSERT(ds_btree_size(tree) == 19, "size after erase");
    ASSERT(ds_btree_find(tree, &key, NULL) == -1, "not found after erase");

    ASSERT(ds_btree_erase(tree, &key) == -1, "erase nonexistent");

    /* 删除根节点 */
    key = 0;
    ds_btree_erase(tree, &key);
    ASSERT(ds_btree_find(tree, &key, NULL) == -1, "root erased");

    ds_btree_destroy(tree);
    PASS();
}

static int g_balance_ok = 1;
static int g_balance_prev = -1;

static void balance_visit(const void* key, const void* value, void* ctx)
{
    (void)value;
    (void)ctx;
    int k = *(const int*)key;
    if (k <= g_balance_prev) {
        g_balance_ok = 0;
    }
    g_balance_prev = k;
}

static void test_balance(void)
{
    TEST("AVL balance (sequential insert)");
    ds_btree_t* tree = ds_btree_create(sizeof(int), sizeof(int), cmp_int);

    /* 顺序插入 1000 个元素，考验 AVL 自平衡 */
    for (int i = 0; i < 1000; i++) {
        ds_btree_insert(tree, &i, &i);
    }
    ASSERT(ds_btree_size(tree) == 1000, "size 1000");

    /* AVL 树高度应 ≤ 1.44 * log2(n) ≈ 15 */
    size_t h = ds_btree_height(tree);
    ASSERT(h <= 18, "height should be balanced");

    /* 验证中序遍历有序 */
    g_balance_ok = 1;
    g_balance_prev = -1;
    ds_btree_traverse(tree, DS_BTREE_INORDER, balance_visit, NULL);
    ASSERT(g_balance_ok, "inorder should be sorted");

    ds_btree_destroy(tree);
    PASS();
}

/* 比较函数使用 C 风格回调捕获 */
struct visit_ctx {
    int count;
    int sum;
};

static void visit_fn(const void* key, const void* value, void* ctx)
{
    struct visit_ctx* c = (struct visit_ctx*)ctx;
    c->count++;
    c->sum += *(const int*)value;
}

static void test_traverse(void)
{
    TEST("traversal");
    ds_btree_t* tree = ds_btree_create(sizeof(int), sizeof(int), cmp_int);

    for (int i = 0; i < 10; i++) {
        ds_btree_insert(tree, &i, &i);
    }

    struct visit_ctx ctx = {0, 0};
    ds_btree_traverse(tree, DS_BTREE_INORDER, visit_fn, &ctx);
    ASSERT(ctx.count == 10, "inorder count");
    ASSERT(ctx.sum == 45, "inorder sum (0-9)");

    ds_btree_destroy(tree);
    PASS();
}

static void test_min_max(void)
{
    TEST("min/max");
    ds_btree_t* tree = ds_btree_create(sizeof(int), sizeof(int), cmp_int);

    int data[] = {5, 3, 8, 1, 9, 6};
    for (int i = 0; i < 6; i++) {
        ds_btree_insert(tree, &data[i], &data[i]);
    }

    int key, val;
    ASSERT(ds_btree_min(tree, &key, &val) == 0 && key == 1, "min");
    ASSERT(ds_btree_max(tree, &key, &val) == 0 && key == 9, "max");

    ds_btree_destroy(tree);
    PASS();
}

static void test_bound(void)
{
    TEST("lower_bound/upper_bound");
    ds_btree_t* tree = ds_btree_create(sizeof(int), sizeof(int), cmp_int);

    int data[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        ds_btree_insert(tree, &data[i], &data[i]);
    }

    int key, val;
    int q = 25;
    ASSERT(ds_btree_lower_bound(tree, &q, &key, &val) == 0 && key == 30,
           "lower_bound");
    ASSERT(ds_btree_upper_bound(tree, &q, &key, &val) == 0 && key == 30,
           "upper_bound");

    q = 50;
    ASSERT(ds_btree_lower_bound(tree, &q, &key, &val) == 0 && key == 50,
           "lower_bound exact");

    /* 没有上界的情况 */
    q = 55;
    ASSERT(ds_btree_upper_bound(tree, &q, &key, &val) == -1,
           "upper_bound beyond max");

    ds_btree_destroy(tree);
    PASS();
}

void test_btree_all(void)
{
    printf("=== AVL Tree Tests ===\n");
    test_create_destroy();
    test_insert_find();
    test_erase();
    test_balance();
    test_traverse();
    test_min_max();
    test_bound();
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);
}
