/**
 * @file test_graph.c
 * @brief 图数据结构单元测试
 */
#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) do { printf("  TEST: %s ... ", name); } while(0)
#define PASS() do { printf("PASS\n"); tests_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); tests_failed++; } while(0)
#define ASSERT(cond, msg) do { if (!(cond)) { FAIL(msg); return; } } while(0)

/* 遍历回调上下文 */
struct visit_ctx {
    int visited[256];
    int count;
};

static void visit_fn(int id, void* data, void* ctx)
{
    (void)data;
    struct visit_ctx* c = (struct visit_ctx*)ctx;
    if (id >= 0 && id < 256) {
        c->visited[id] = 1;
    }
    c->count++;
}

static void test_create_destroy(void)
{
    TEST("create and destroy");
    ds_graph_t* g = ds_graph_create(1); /* 有向图 */
    ASSERT(g != NULL, "create failed");
    ASSERT(ds_graph_vertex_count(g) == 0, "vertex count 0");
    ASSERT(ds_graph_is_directed(g) == 1, "should be directed");
    ds_graph_destroy(g);
    ds_graph_destroy(NULL);
    PASS();
}

static void test_add_vertex(void)
{
    TEST("add vertex");
    ds_graph_t* g = ds_graph_create(0);

    ASSERT(ds_graph_add_vertex(g, 1, NULL, 0) == 0, "add v1");
    ASSERT(ds_graph_add_vertex(g, 2, NULL, 0) == 0, "add v2");
    ASSERT(ds_graph_has_vertex(g, 1) == 1, "has v1");
    ASSERT(ds_graph_has_vertex(g, 3) == 0, "not have v3");
    ASSERT(ds_graph_add_vertex(g, 1, NULL, 0) == -1, "duplicate id");

    ASSERT(ds_graph_vertex_count(g) == 2, "vertex count 2");
    ds_graph_destroy(g);
    PASS();
}

static void test_add_edge(void)
{
    TEST("add edge");
    /* 有向图 */
    ds_graph_t* g = ds_graph_create(1);
    ds_graph_add_vertex(g, 0, NULL, 0);
    ds_graph_add_vertex(g, 1, NULL, 0);
    ds_graph_add_vertex(g, 2, NULL, 0);

    ASSERT(ds_graph_add_edge(g, 0, 1, 1.0) == 0, "add edge 0->1");
    ASSERT(ds_graph_add_edge(g, 1, 2, 2.0) == 0, "add edge 1->2");
    ASSERT(ds_graph_has_edge(g, 0, 1) == 1, "has 0->1");
    ASSERT(ds_graph_has_edge(g, 1, 0) == 0, "no 1->0 (directed)");

    double w;
    ASSERT(ds_graph_edge_weight(g, 0, 1, &w) == 0 && w == 1.0, "weight");
    ASSERT(ds_graph_edge_count(g) == 2, "edge count 2");

    ds_graph_destroy(g);
    PASS();
}

static void test_undirected_edge(void)
{
    TEST("undirected edge");
    ds_graph_t* g = ds_graph_create(0); /* 无向图 */
    ds_graph_add_vertex(g, 0, NULL, 0);
    ds_graph_add_vertex(g, 1, NULL, 0);

    ds_graph_add_edge(g, 0, 1, 1.0);
    ASSERT(ds_graph_has_edge(g, 0, 1) == 1, "has 0-1");
    ASSERT(ds_graph_has_edge(g, 1, 0) == 1, "has 1-0 (undirected)");
    ASSERT(ds_graph_edge_count(g) == 1, "edge count 1 (undirected counts as 1)");

    ds_graph_destroy(g);
    PASS();
}

static void test_bfs(void)
{
    TEST("BFS traversal");
    ds_graph_t* g = ds_graph_create(0); /* 无向图 */
    /*
     *    0 -- 1 -- 2
     *    |    |
     *    3 -- 4
     */
    for (int i = 0; i < 5; i++) {
        ds_graph_add_vertex(g, i, NULL, 0);
    }
    ds_graph_add_edge(g, 0, 1, 1);
    ds_graph_add_edge(g, 1, 2, 1);
    ds_graph_add_edge(g, 0, 3, 1);
    ds_graph_add_edge(g, 3, 4, 1);
    ds_graph_add_edge(g, 1, 4, 1);

    struct visit_ctx ctx = {{{0}}, 0};
    ASSERT(ds_graph_bfs(g, 0, visit_fn, &ctx) == 0, "bfs");

    int all_visited = 1;
    for (int i = 0; i < 5; i++) {
        if (!ctx.visited[i]) { all_visited = 0; break; }
    }
    ASSERT(all_visited, "bfs visited all");

    ds_graph_destroy(g);
    PASS();
}

static void test_dfs(void)
{
    TEST("DFS traversal");
    ds_graph_t* g = ds_graph_create(0);
    for (int i = 0; i < 5; i++) {
        ds_graph_add_vertex(g, i, NULL, 0);
    }
    ds_graph_add_edge(g, 0, 1, 1);
    ds_graph_add_edge(g, 1, 2, 1);
    ds_graph_add_edge(g, 2, 3, 1);
    ds_graph_add_edge(g, 3, 4, 1);

    struct visit_ctx ctx = {{{0}}, 0};
    ds_graph_dfs(g, 0, visit_fn, &ctx);
    ASSERT(ctx.count == 5, "dfs visited all");

    ds_graph_destroy(g);
    PASS();
}

static void test_dijkstra(void)
{
    TEST("Dijkstra shortest path");
    ds_graph_t* g = ds_graph_create(1); /* 有向图 */
    /*
     *  0 --> 1 --> 3
     *  |     ^
     *  v    /
     *  2 <-/
     */
    for (int i = 0; i < 4; i++) {
        ds_graph_add_vertex(g, i, NULL, 0);
    }
    ds_graph_add_edge(g, 0, 1, 4);
    ds_graph_add_edge(g, 0, 2, 1);
    ds_graph_add_edge(g, 2, 1, 2);
    ds_graph_add_edge(g, 1, 3, 1);
    ds_graph_add_edge(g, 2, 3, 5);

    double dist[4];
    int prev[4];
    ds_graph_dijkstra(g, 0, dist, prev);

    /* 最短路径: 0->2(1) 2->1(2) 1->3(1) = 4 */
    ASSERT(fabs(dist[0] - 0.0) < 0.001, "dist[0] = 0");
    ASSERT(fabs(dist[1] - 3.0) < 0.001, "dist[1] = 3");
    ASSERT(fabs(dist[3] - 4.0) < 0.001, "dist[3] = 4");

    ds_graph_destroy(g);
    PASS();
}

static void test_topological_sort(void)
{
    TEST("topological sort");
    ds_graph_t* g = ds_graph_create(1);
    /*
     *  0 --> 1 --> 3
     *  |           ^
     *  v           |
     *  2 ----------/
     */
    for (int i = 0; i < 4; i++) {
        ds_graph_add_vertex(g, i, NULL, 0);
    }
    ds_graph_add_edge(g, 0, 1, 1);
    ds_graph_add_edge(g, 0, 2, 1);
    ds_graph_add_edge(g, 1, 3, 1);
    ds_graph_add_edge(g, 2, 3, 1);

    int order[4];
    size_t n;
    ASSERT(ds_graph_topological_sort(g, order, &n) == 0, "topo sort");
    ASSERT(n == 4, "topo sort all vertices");

    /* 验证 0 在 1 之前，1 在 3 之前 */
    int pos[4];
    for (size_t i = 0; i < n; i++) {
        pos[order[i]] = (int)i;
    }
    ASSERT(pos[0] < pos[1], "topo: 0 before 1");
    ASSERT(pos[1] < pos[3], "topo: 1 before 3");
    ASSERT(pos[0] < pos[2], "topo: 0 before 2");
    ASSERT(pos[2] < pos[3], "topo: 2 before 3");

    ds_graph_destroy(g);
    PASS();
}

static void test_remove_vertex(void)
{
    TEST("remove vertex");
    ds_graph_t* g = ds_graph_create(0);
    ds_graph_add_vertex(g, 1, NULL, 0);
    ds_graph_add_vertex(g, 2, NULL, 0);
    ds_graph_add_edge(g, 1, 2, 1.0);

    ASSERT(ds_graph_remove_vertex(g, 1) == 0, "remove vertex");
    ASSERT(ds_graph_has_vertex(g, 1) == 0, "not have removed");
    ASSERT(ds_graph_vertex_count(g) == 1, "count after remove");

    ASSERT(ds_graph_remove_vertex(g, 99) == -1, "remove nonexistent");

    ds_graph_destroy(g);
    PASS();
}

void test_graph_all(void)
{
    printf("=== Graph Tests ===\n");
    test_create_destroy();
    test_add_vertex();
    test_add_edge();
    test_undirected_edge();
    test_bfs();
    test_dfs();
    test_dijkstra();
    test_topological_sort();
    test_remove_vertex();
    printf("  Results: %d passed, %d failed\n\n", tests_passed, tests_failed);
}
