/**
 * @file test_dlist.c
 * @brief 双向链表单元测试
 */
#include "dlist.h"
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
    ds_dlist_t* list = ds_dlist_create(sizeof(int), NULL);
    ASSERT(list != NULL, "create failed");
    ASSERT(ds_dlist_size(list) == 0, "size should be 0");
    ASSERT(ds_dlist_empty(list), "should be empty");
    ds_dlist_destroy(list);
    ds_dlist_destroy(NULL);
    PASS();
}

static void test_push_pop(void)
{
    TEST("push and pop");
    ds_dlist_t* list = ds_dlist_create(sizeof(int), NULL);
    ASSERT(list != NULL, "create failed");

    int val;
    for (int i = 0; i < 5; i++) {
        ASSERT(ds_dlist_push_back(list, &i) == 0, "push_back");
    }
    ASSERT(ds_dlist_size(list) == 5, "size after push_back");

    ASSERT(ds_dlist_front(list, &val) == 0 && val == 0, "front");
    ASSERT(ds_dlist_back(list, &val) == 0 && val == 4, "back");

    /* pop_front */
    ASSERT(ds_dlist_pop_front(list, &val) == 0 && val == 0, "pop_front");
    ASSERT(ds_dlist_pop_back(list, &val) == 0 && val == 4, "pop_back");
    ASSERT(ds_dlist_size(list) == 3, "size after pops");

    /* push_front */
    val = 100;
    ds_dlist_push_front(list, &val);
    ASSERT(ds_dlist_front(list, &val) == 0 && val == 100, "push_front");

    ds_dlist_destroy(list);
    PASS();
}

static void test_iterator(void)
{
    TEST("iterator");
    ds_dlist_t* list = ds_dlist_create(sizeof(int), NULL);

    for (int i = 0; i < 5; i++) {
        ds_dlist_push_back(list, &i);
    }

    /* forward */
    int count = 0;
    for (ds_dlist_iter_t it = ds_dlist_begin(list);
         !ds_dlist_iter_equal(&it, &ds_dlist_end(list));
         ds_dlist_iter_next(&it)) {
        int* val = (int*)ds_dlist_iter_data(&it);
        ASSERT(val && *val == count, "forward iter wrong");
        count++;
    }
    ASSERT(count == 5, "forward count");

    /* insert_after */
    ds_dlist_iter_t it = ds_dlist_begin(list);
    int val = 99;
    ds_dlist_insert_after(&it, &val);
    ASSERT(ds_dlist_size(list) == 6, "size after insert_after");

    /* erase */
    it = ds_dlist_begin(list);
    ds_dlist_erase(&it, NULL);
    ASSERT(ds_dlist_size(list) == 5, "size after erase");

    ds_dlist_destroy(list);
    PASS();
}

static void test_clear(void)
{
    TEST("clear");
    ds_dlist_t* list = ds_dlist_create(sizeof(int), NULL);

    for (int i = 0; i < 10; i++) {
        ds_dlist_push_back(list, &i);
    }
    ASSERT(ds_dlist_size(list) == 10, "size before clear");

    ds_dlist_clear(list);
    ASSERT(ds_dlist_size(list) == 0, "size after clear");
    ASSERT(ds_dlist_empty(list), "empty after clear");

    ds_dlist_destroy(list);
    PASS();
}

void test_dlist_all(void)
{
    printf("=== Doubly Linked List Tests ===\n");
    test_create_destroy();
    test_push_pop();
    test_iterator();
    test_clear();
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);
}
