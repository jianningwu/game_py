/**
 * @file run_tests.c
 * @brief 统一测试入口
 *
 * 编译：
 *   gcc -I../include run_tests.c test_darray.c test_dlist.c test_mqueue.c
 *       test_htable.c test_btree.c test_graph.c
 *       ../src/darray.c ../src/dlist.c ../src/mqueue.c
 *       ../src/htable.c ../src/btree.c ../src/graph.c
 *       -o run_tests -lpthread
 *
 *   MSVC (Visual Studio):
 *     cl /I../include run_tests.c test_*.c ../src/*.c /Fe:run_tests.exe
 */

#include <stdio.h>

/* 每个测试模块的入口 */
void test_darray_all(void);
void test_dlist_all(void);
void test_mqueue_all(void);
void test_htable_all(void);
void test_btree_all(void);
void test_graph_all(void);

int main(void)
{
    printf("========================================\n");
    printf("  数据结构库单元测试\n");
    printf("  Data Structure Library Unit Tests\n");
    printf("========================================\n\n");

    test_darray_all();
    test_dlist_all();
    test_mqueue_all();
    test_htable_all();
    test_btree_all();
    test_graph_all();

    printf("========================================\n");
    printf("  所有测试完成\n");
    printf("========================================\n");
    return 0;
}
