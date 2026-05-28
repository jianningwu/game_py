/**
 * @file test_htable.c
 * @brief 哈希表单元测试
 */
#include "htable.h"
#include <stdio.h>
#include <string.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) do { printf("  TEST: %s ... ", name); } while(0)
#define PASS() do { printf("PASS\n"); tests_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); tests_failed++; } while(0)
#define ASSERT(cond, msg) do { if (!(cond)) { FAIL(msg); return; } } while(0)

static void test_create_destroy(void)
{
    TEST("create and destroy");
    ds_htable_t* ht = ds_htable_create(sizeof(int), sizeof(int), 0,
                                         ds_hash_int, ds_eq_int);
    ASSERT(ht != NULL, "create failed");
    ASSERT(ds_htable_size(ht) == 0, "size 0");
    ASSERT(ds_htable_empty(ht), "empty");
    ds_htable_destroy(ht);
    ds_htable_destroy(NULL);
    PASS();
}

static void test_insert_find(void)
{
    TEST("insert and find");
    ds_htable_t* ht = ds_htable_create(sizeof(int), sizeof(int), 0,
                                         ds_hash_int, ds_eq_int);
    ASSERT(ht != NULL, "create failed");

    for (int i = 0; i < 100; i++) {
        int val = i * i;
        ASSERT(ds_htable_insert(ht, &i, &val) == 0, "insert");
    }
    ASSERT(ds_htable_size(ht) == 100, "size 100");

    for (int i = 0; i < 100; i++) {
        int val;
        ASSERT(ds_htable_find(ht, &i, &val) == 0, "find");
        ASSERT(val == i * i, "value match");
    }

    ds_htable_destroy(ht);
    PASS();
}

static void test_update(void)
{
    TEST("update existing key");
    ds_htable_t* ht = ds_htable_create(sizeof(int), sizeof(int), 0,
                                         ds_hash_int, ds_eq_int);

    int key = 1, val = 100;
    ds_htable_insert(ht, &key, &val);
    val = 200;
    ds_htable_insert(ht, &key, &val);

    int found;
    ds_htable_find(ht, &key, &found);
    ASSERT(found == 200, "update failed");

    ds_htable_destroy(ht);
    PASS();
}

static void test_erase(void)
{
    TEST("erase");
    ds_htable_t* ht = ds_htable_create(sizeof(int), sizeof(int), 0,
                                         ds_hash_int, ds_eq_int);

    for (int i = 0; i < 10; i++) {
        ds_htable_insert(ht, &i, &i);
    }
    ASSERT(ds_htable_size(ht) == 10, "size before erase");

    int key = 5;
    ASSERT(ds_htable_erase(ht, &key) == 0, "erase");
    ASSERT(ds_htable_size(ht) == 9, "size after erase");

    int val;
    ASSERT(ds_htable_find(ht, &key, &val) == -1, "should not find erased");

    ASSERT(ds_htable_erase(ht, &key) == -1, "erase nonexistent");

    ds_htable_destroy(ht);
    PASS();
}

static void test_string_keys(void)
{
    TEST("string keys");
    ds_htable_t* ht = ds_htable_create(32, sizeof(int), 0,
                                         ds_hash_str, ds_eq_str);
    ASSERT(ht != NULL, "create failed");

    const char* keys[] = {"apple", "banana", "cherry", "date"};
    for (int i = 0; i < 4; i++) {
        char key[32];
        strcpy(key, keys[i]);
        ASSERT(ds_htable_insert(ht, key, &i) == 0, "insert str key");
    }

    for (int i = 0; i < 4; i++) {
        char key[32];
        strcpy(key, keys[i]);
        int val;
        ASSERT(ds_htable_find(ht, key, &val) == 0, "find str key");
        ASSERT(val == i, "str value match");
    }

    ds_htable_destroy(ht);
    PASS();
}

static void test_clear(void)
{
    TEST("clear");
    ds_htable_t* ht = ds_htable_create(sizeof(int), sizeof(int), 0,
                                         ds_hash_int, ds_eq_int);

    for (int i = 0; i < 50; i++) {
        ds_htable_insert(ht, &i, &i);
    }
    ASSERT(ds_htable_size(ht) == 50, "size before clear");

    ds_htable_clear(ht);
    ASSERT(ds_htable_size(ht) == 0, "size after clear");

    for (int i = 0; i < 50; i++) {
        int val;
        ASSERT(ds_htable_find(ht, &i, &val) == -1, "not found after clear");
    }

    ds_htable_destroy(ht);
    PASS();
}

static void test_iterator(void)
{
    TEST("iterator");
    ds_htable_t* ht = ds_htable_create(sizeof(int), sizeof(int), 0,
                                         ds_hash_int, ds_eq_int);

    for (int i = 0; i < 20; i++) {
        ds_htable_insert(ht, &i, &i);
    }

    int count = 0;
    int sum = 0;
    for (ds_htable_iter_t it = ds_htable_begin(ht);
         !ds_htable_iter_equal(&it, &ds_htable_end(ht));
         ds_htable_iter_next(&it)) {
        int* k = (int*)ds_htable_iter_key(&it);
        int* v = (int*)ds_htable_iter_value(&it);
        ASSERT(k != NULL && v != NULL, "iter data");
        sum += *v;
        count++;
    }
    ASSERT(count == 20, "iter count");
    ASSERT(sum == 190, "iter sum (0+1+...+19=190)");

    ds_htable_destroy(ht);
    PASS();
}

void test_htable_all(void)
{
    printf("=== Hash Table Tests ===\n");
    test_create_destroy();
    test_insert_find();
    test_update();
    test_erase();
    test_string_keys();
    test_clear();
    test_iterator();
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);
}
