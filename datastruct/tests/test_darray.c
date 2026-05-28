/**
 * @file test_darray.c
 * @brief 动态数组单元测试
 */
#include "darray.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) do { printf("  TEST: %s ... ", name); } while(0)
#define PASS() do { printf("PASS\n"); tests_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); tests_failed++; } while(0)
#define ASSERT(cond, msg) do { if (!(cond)) { FAIL(msg); return; } } while(0)

static void test_create_destroy(void)
{
    TEST("create and destroy");
    ds_darray_t* arr = ds_darray_create(sizeof(int), 0);
    ASSERT(arr != NULL, "create returned NULL");
    ASSERT(ds_darray_size(arr) == 0, "size should be 0");
    ASSERT(ds_darray_capacity(arr) > 0, "capacity should > 0");
    ds_darray_destroy(arr);
    ds_darray_destroy(NULL); /* 应不崩溃 */
    PASS();
}

static void test_push_pop(void)
{
    TEST("push_back and pop_back");
    ds_darray_t* arr = ds_darray_create(sizeof(int), 4);
    ASSERT(arr != NULL, "create failed");

    for (int i = 0; i < 100; i++) {
        ASSERT(ds_darray_push_back(arr, &i) == 0, "push_back failed");
    }
    ASSERT(ds_darray_size(arr) == 100, "size should be 100");

    for (int i = 99; i >= 0; i--) {
        int val;
        ASSERT(ds_darray_pop_back(arr, &val) == 0, "pop_back failed");
        ASSERT(val == i, "pop_back value mismatch");
    }
    ASSERT(ds_darray_size(arr) == 0, "size should be 0 after pops");
    ASSERT(ds_darray_empty(arr), "should be empty");

    /* pop empty */
    ASSERT(ds_darray_pop_back(arr, NULL) == -1, "pop empty should fail");

    ds_darray_destroy(arr);
    PASS();
}

static void test_get_set(void)
{
    TEST("get and set");
    ds_darray_t* arr = ds_darray_create(sizeof(int), 10);
    ASSERT(arr != NULL, "create failed");

    for (int i = 0; i < 10; i++) {
        ds_darray_push_back(arr, &i);
    }

    int val;
    ASSERT(ds_darray_get(arr, 0, &val) == 0 && val == 0, "get[0] failed");
    ASSERT(ds_darray_get(arr, 5, &val) == 0 && val == 5, "get[5] failed");
    ASSERT(ds_darray_get(arr, 9, &val) == 0 && val == 9, "get[9] failed");
    ASSERT(ds_darray_get(arr, 10, &val) == -1, "get[10] should fail");

    int new_val = 42;
    ds_darray_set(arr, 3, &new_val);
    ASSERT(ds_darray_get(arr, 3, &val) == 0 && val == 42, "set[3] failed");
    ASSERT(ds_darray_set(arr, 20, &new_val) == -1, "set out of bounds");

    ds_darray_destroy(arr);
    PASS();
}

static void test_insert_erase(void)
{
    TEST("insert and erase");
    ds_darray_t* arr = ds_darray_create(sizeof(int), 4);
    ASSERT(arr != NULL, "create failed");

    for (int i = 0; i < 5; i++) {
        ds_darray_push_back(arr, &i);
    }

    int val = 99;
    ASSERT(ds_darray_insert(arr, 2, &val) == 0, "insert failed");
    ASSERT(ds_darray_size(arr) == 6, "size after insert");

    int read;
    ds_darray_get(arr, 2, &read);
    ASSERT(read == 99, "inserted value wrong");

    /* erase */
    ASSERT(ds_darray_erase(arr, 2, &read) == 0, "erase failed");
    ASSERT(read == 99, "erased value wrong");
    ASSERT(ds_darray_size(arr) == 5, "size after erase");

    ds_darray_destroy(arr);
    PASS();
}

static void test_reserve_resize(void)
{
    TEST("reserve and resize");
    ds_darray_t* arr = ds_darray_create(sizeof(int), 4);
    ASSERT(arr != NULL, "create failed");

    ASSERT(ds_darray_reserve(arr, 100) == 0, "reserve failed");
    ASSERT(ds_darray_capacity(arr) >= 100, "capacity should be >= 100");

    ASSERT(ds_darray_resize(arr, 50) == 0, "resize failed");
    ASSERT(ds_darray_size(arr) == 50, "size should be 50 after resize");

    int val;
    ds_darray_get(arr, 49, &val);
    ASSERT(val == 0, "resized zeros not filled");

    ds_darray_resize(arr, 10);
    ASSERT(ds_darray_size(arr) == 10, "shrink failed");

    ds_darray_destroy(arr);
    PASS();
}

static void test_iterator(void)
{
    TEST("iterator");
    ds_darray_t* arr = ds_darray_create(sizeof(int), 10);
    ASSERT(arr != NULL, "create failed");

    for (int i = 0; i < 5; i++) {
        ds_darray_push_back(arr, &i);
    }

    int count = 0;
    for (ds_darray_iter_t it = ds_darray_begin(arr);
         !ds_darray_iter_equal(&it, &ds_darray_end(arr));
         ds_darray_iter_next(&it)) {
        int* val = (int*)ds_darray_iter_data(&it);
        ASSERT(val && *val == count, "iterator value wrong");
        count++;
    }
    ASSERT(count == 5, "iterator count wrong");

    ds_darray_destroy(arr);
    PASS();
}

static void test_find_sort(void)
{
    TEST("find and sort");
    ds_darray_t* arr = ds_darray_create(sizeof(int), 10);
    ASSERT(arr != NULL, "create failed");

    int vals[] = {5, 3, 1, 4, 2};
    for (int i = 0; i < 5; i++) {
        ds_darray_push_back(arr, &vals[i]);
    }

    int key = 3;
    size_t idx;
    ASSERT(ds_darray_find(arr, &key,
           (int (*)(const void*, const void*))ds_eq_int, &idx) == 0
           && idx == 1, "find failed");

    int nokey = 99;
    ASSERT(ds_darray_find(arr, &nokey,
           (int (*)(const void*, const void*))ds_eq_int, NULL) == -1,
           "find nonexistent");

    ds_darray_sort(arr, (int (*)(const void*, const void*))ds_eq_int);
    for (size_t i = 1; i < ds_darray_size(arr); i++) {
        int prev, curr;
        ds_darray_get(arr, i-1, &prev);
        ds_darray_get(arr, i, &curr);
        ASSERT(prev <= curr, "sort order wrong");
    }

    ds_darray_destroy(arr);
    PASS();
}

/* 测试字符串数组 */
static void test_strings(void)
{
    TEST("strings");
    char buf[32];
    ds_darray_t* arr = ds_darray_create(32, 4);
    ASSERT(arr != NULL, "create failed");

    const char* strs[] = {"hello", "world", "test"};
    for (int i = 0; i < 3; i++) {
        char tmp[32];
        strcpy(tmp, strs[i]);
        ASSERT(ds_darray_push_back(arr, tmp) == 0, "push str failed");
    }

    ASSERT(ds_darray_size(arr) == 3, "size");
    ds_darray_get(arr, 1, buf);
    ASSERT(strcmp(buf, "world") == 0, "get str failed");

    ds_darray_destroy(arr);
    PASS();
}

void test_darray_all(void)
{
    printf("=== Dynamic Array Tests ===\n");
    test_create_destroy();
    test_push_pop();
    test_get_set();
    test_insert_erase();
    test_reserve_resize();
    test_iterator();
    test_find_sort();
    test_strings();
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);
}
