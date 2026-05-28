/**
 * @file test_mqueue.c
 * @brief 消息队列单元测试
 */
#include "mqueue.h"
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
    ds_mqueue_t* mq = ds_mqueue_create(10, sizeof(int));
    ASSERT(mq != NULL, "create failed");
    ASSERT(ds_mqueue_capacity(mq) == 10, "capacity");
    ASSERT(ds_mqueue_count(mq) == 0, "count");
    ASSERT(ds_mqueue_empty(mq), "should be empty");
    ds_mqueue_destroy(mq);
    ds_mqueue_destroy(NULL);
    PASS();
}

static void test_send_recv(void)
{
    TEST("send and recv (blocking)");
    ds_mqueue_t* mq = ds_mqueue_create(5, sizeof(int));
    ASSERT(mq != NULL, "create failed");

    for (int i = 0; i < 5; i++) {
        ASSERT(ds_mqueue_send(mq, &i, 100) == 0, "send");
    }
    ASSERT(ds_mqueue_full(mq), "should be full");
    ASSERT(ds_mqueue_count(mq) == 5, "count should be 5");

    for (int i = 0; i < 5; i++) {
        int val;
        ASSERT(ds_mqueue_recv(mq, &val, 100) == 0, "recv");
        ASSERT(val == i, "value mismatch");
    }
    ASSERT(ds_mqueue_empty(mq), "should be empty after recv");

    ds_mqueue_destroy(mq);
    PASS();
}

static void test_try_recv(void)
{
    TEST("try_recv on empty queue");
    ds_mqueue_t* mq = ds_mqueue_create(3, sizeof(int));
    ASSERT(mq != NULL, "create failed");

    int val;
    ASSERT(ds_mqueue_try_recv(mq, &val) == -1, "try_recv on empty should fail");

    int x = 42;
    ds_mqueue_send(mq, &x, 0);
    ASSERT(ds_mqueue_try_recv(mq, &val) == 0 && val == 42, "try_recv should succeed");

    ds_mqueue_destroy(mq);
    PASS();
}

static void test_send_urgent(void)
{
    TEST("send_urgent");
    ds_mqueue_t* mq = ds_mqueue_create(5, sizeof(int));
    ASSERT(mq != NULL, "create failed");

    int a = 1, b = 2;
    ds_mqueue_send(mq, &a, 0);
    ds_mqueue_send_urgent(mq, &b, 0);

    int val;
    ds_mqueue_recv(mq, &val, 0);
    ASSERT(val == 2, "urgent message should be at front");

    ds_mqueue_recv(mq, &val, 0);
    ASSERT(val == 1, "second should be original");

    ds_mqueue_destroy(mq);
    PASS();
}

static void test_flush(void)
{
    TEST("flush");
    ds_mqueue_t* mq = ds_mqueue_create(5, sizeof(int));

    for (int i = 0; i < 3; i++) {
        ds_mqueue_send(mq, &i, 0);
    }
    ASSERT(ds_mqueue_count(mq) == 3, "count before flush");

    ds_mqueue_flush(mq);
    ASSERT(ds_mqueue_count(mq) == 0, "count after flush");
    ASSERT(ds_mqueue_empty(mq), "empty after flush");

    ds_mqueue_destroy(mq);
    PASS();
}

void test_mqueue_all(void)
{
    printf("=== Message Queue Tests ===\n");
    test_create_destroy();
    test_send_recv();
    test_try_recv();
    test_send_urgent();
    test_flush();
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);
}
