/**
 * @file graph.c
 * @brief 图数据结构实现（邻接表）
 *
 * 实现策略：
 * - 邻接表存储（适合稀疏图）
 * - 顶点使用哈希表管理（通过 ID 快速查找）
 * - 每个顶点的邻接边使用动态数组存储
 * - 内置 BFS（队列）、DFS（栈/递归）、Dijkstra（数组实现）
 * - 拓扑排序基于 Kahn 算法
 */

#include "graph.h"
#include <stdlib.h>  /* malloc, free, calloc */
#include <string.h>  /* memcpy, memset */
#include <limits.h>  /* INT_MAX */

/* ------------------------------------------------------------------ */
/*  内部常量                                                           */
/* ------------------------------------------------------------------ */

/** @brief Dijkstra 无穷大距离 */
#define INF_DIST 1e18

/** @brief 顶点邻接边数组默认初始容量 */
#define GRAPH_DEFAULT_EDGE_CAPACITY 8

/* ------------------------------------------------------------------ */
/*  邻接边                                                             */
/* ------------------------------------------------------------------ */
typedef struct {
    int    to;      /**< 目标顶点 ID */
    double weight;  /**< 边权重 */
} edge_t;

/* ------------------------------------------------------------------ */
/*  顶点                                                               */
/* ------------------------------------------------------------------ */
typedef struct {
    int    id;                /**< 顶点 ID */
    void*  data;              /**< 用户数据 */
    size_t data_size;         /**< 数据大小 */
    edge_t* edges;            /**< 邻接边数组 */
    size_t  edge_count;       /**< 当前边数 */
    size_t  edge_capacity;    /**< 边数组容量 */
    int     visited;          /**< 遍历标志 */
} vertex_t;

/* ------------------------------------------------------------------ */
/*  图结构体                                                           */
/* ------------------------------------------------------------------ */
struct ds_graph {
    vertex_t* vertices;       /**< 顶点数组 */
    size_t    vertex_count;   /**< 顶点数 */
    size_t    vertex_capacity;/**< 顶点数组容量 */
    size_t    edge_count;     /**< 总边数 */
    int       directed;       /**< 是否有向 */

    /** 用于快速查找顶点的辅助字段 */
    int* id_to_index;         /**< ID 到索引的映射 */
    int  id_min, id_max;      /**< ID 范围 */
};

/* ------------------------------------------------------------------ */
/*  内部辅助函数                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 查找顶点索引
 * @return 顶点在 vertices 数组中的索引，-1 表示未找到
 */
static int graph_find_vertex(const ds_graph_t* graph, int id)
{
    if (graph->vertex_count == 0) {
        return -1;
    }
    /* 使用映射数组加速 */
    if (id >= graph->id_min && id <= graph->id_max) {
        int idx = graph->id_to_index[id - graph->id_min];
        if (idx >= 0 && idx < (int)graph->vertex_count &&
            graph->vertices[idx].id == id) {
            return idx;
        }
    }
    /* 线性搜索（兜底） */
    for (size_t i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}

/**
 * @brief 获取顶点指针
 */
static vertex_t* graph_get_vertex(ds_graph_t* graph, int id)
{
    int idx = graph_find_vertex(graph, id);
    return (idx >= 0) ? &graph->vertices[idx] : NULL;
}

/**
 * @brief 确保顶点有足够的边数组容量
 */
static int graph_ensure_edge_capacity(vertex_t* v)
{
    if (v->edge_count >= v->edge_capacity) {
        size_t new_cap = v->edge_capacity == 0
                         ? GRAPH_DEFAULT_EDGE_CAPACITY
                         : v->edge_capacity * 2;
        edge_t* new_edges = (edge_t*)realloc(v->edges, new_cap * sizeof(edge_t));
        if (!new_edges) {
            return -1;
        }
        v->edges = new_edges;
        v->edge_capacity = new_cap;
    }
    return 0;
}

/**
 * @brief 在顶点邻接表中查找边的索引
 */
static int graph_find_edge(const vertex_t* v, int to)
{
    for (size_t i = 0; i < v->edge_count; i++) {
        if (v->edges[i].to == to) {
            return (int)i;
        }
    }
    return -1;
}

/**
 * @brief 重建 ID 到索引的映射表
 */
static int graph_rebuild_id_map(ds_graph_t* graph)
{
    if (graph->vertex_count == 0) {
        graph->id_to_index = NULL;
        return 0;
    }

    /* 找出 ID 范围 */
    int min_id = graph->vertices[0].id;
    int max_id = graph->vertices[0].id;
    for (size_t i = 1; i < graph->vertex_count; i++) {
        if (graph->vertices[i].id < min_id) min_id = graph->vertices[i].id;
        if (graph->vertices[i].id > max_id) max_id = graph->vertices[i].id;
    }

    graph->id_min = min_id;
    graph->id_max = max_id;

    int range = max_id - min_id + 1;
    free(graph->id_to_index);
    graph->id_to_index = (int*)malloc(range * sizeof(int));
    if (!graph->id_to_index) {
        return -1;
    }

    for (int i = 0; i < range; i++) {
        graph->id_to_index[i] = -1;
    }
    for (size_t i = 0; i < graph->vertex_count; i++) {
        int idx = graph->vertices[i].id - min_id;
        graph->id_to_index[idx] = (int)i;
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

ds_graph_t* ds_graph_create(int directed)
{
    ds_graph_t* graph = (ds_graph_t*)calloc(1, sizeof(ds_graph_t));
    if (!graph) {
        return NULL;
    }
    graph->directed = directed;
    graph->vertex_capacity = 16;
    graph->vertices = (vertex_t*)malloc(graph->vertex_capacity * sizeof(vertex_t));
    if (!graph->vertices) {
        free(graph);
        return NULL;
    }
    graph->vertex_count = 0;
    graph->edge_count = 0;
    graph->id_to_index = NULL;
    return graph;
}

void ds_graph_destroy(ds_graph_t* graph)
{
    if (!graph) {
        return;
    }
    for (size_t i = 0; i < graph->vertex_count; i++) {
        free(graph->vertices[i].data);
        free(graph->vertices[i].edges);
    }
    free(graph->vertices);
    free(graph->id_to_index);
    free(graph);
}

/* ------------------------------------------------------------------ */
/*  顶点操作                                                           */
/* ------------------------------------------------------------------ */

int ds_graph_add_vertex(ds_graph_t* graph, int id,
                         const void* data, size_t data_size)
{
    if (!graph) {
        return -1;
    }

    /* 检查 ID 是否重复 */
    if (graph_find_vertex(graph, id) >= 0) {
        return -1;  /* ID 已存在 */
    }

    /* 扩容顶点数组 */
    if (graph->vertex_count >= graph->vertex_capacity) {
        size_t new_cap = graph->vertex_capacity * 2;
        vertex_t* new_verts = (vertex_t*)realloc(
            graph->vertices, new_cap * sizeof(vertex_t));
        if (!new_verts) {
            return -1;
        }
        graph->vertices = new_verts;
        graph->vertex_capacity = new_cap;
    }

    vertex_t* v = &graph->vertices[graph->vertex_count];
    v->id = id;
    v->data = NULL;
    v->data_size = 0;
    v->edges = NULL;
    v->edge_count = 0;
    v->edge_capacity = 0;
    v->visited = 0;

    if (data && data_size > 0) {
        v->data = malloc(data_size);
        if (!v->data) {
            return -1;
        }
        memcpy(v->data, data, data_size);
        v->data_size = data_size;
    }

    graph->vertex_count++;

    /* 重建 ID 映射 */
    graph_rebuild_id_map(graph);
    return 0;
}

int ds_graph_remove_vertex(ds_graph_t* graph, int id)
{
    if (!graph) {
        return -1;
    }

    int idx = graph_find_vertex(graph, id);
    if (idx < 0) {
        return -1;
    }

    vertex_t* v = &graph->vertices[idx];

    /* 删除所有关联的边 */
    /* 从所有其他顶点中删除到该顶点的边 */
    for (size_t i = 0; i < graph->vertex_count; i++) {
        if ((int)i == idx) continue;
        vertex_t* other = &graph->vertices[i];
        for (size_t j = 0; j < other->edge_count; j++) {
            if (other->edges[j].to == id) {
                /* 用最后一个元素覆盖 */
                other->edges[j] = other->edges[other->edge_count - 1];
                other->edge_count--;
                graph->edge_count--;
                break;
            }
        }
    }

    /* 释放顶点的相关数据 */
    free(v->data);
    free(v->edges);

    /* 用最后一个顶点覆盖 */
    if ((size_t)idx < graph->vertex_count - 1) {
        graph->vertices[idx] = graph->vertices[graph->vertex_count - 1];
    }
    graph->vertex_count--;

    graph_rebuild_id_map(graph);
    return 0;
}

int ds_graph_has_vertex(const ds_graph_t* graph, int id)
{
    return graph ? (graph_find_vertex(graph, id) >= 0) : 0;
}

int ds_graph_vertex_data(const ds_graph_t* graph, int id, void* out_data)
{
    if (!graph || !out_data) {
        return -1;
    }
    vertex_t* v = graph_get_vertex((ds_graph_t*)graph, id);
    if (!v || !v->data) {
        return -1;
    }
    memcpy(out_data, v->data, v->data_size);
    return 0;
}

/* ------------------------------------------------------------------ */
/*  边操作                                                             */
/* ------------------------------------------------------------------ */

int ds_graph_add_edge(ds_graph_t* graph, int from, int to, double weight)
{
    if (!graph) {
        return -1;
    }

    vertex_t* v_from = graph_get_vertex(graph, from);
    vertex_t* v_to   = graph_get_vertex(graph, to);
    if (!v_from || !v_to) {
        return -1;
    }

    /* 检查边是否已存在 */
    int edge_idx = graph_find_edge(v_from, to);
    if (edge_idx >= 0) {
        /* 更新权重 */
        v_from->edges[edge_idx].weight = weight;
        return 0;
    }

    /* 添加边到 from 的邻接表 */
    if (graph_ensure_edge_capacity(v_from) != 0) {
        return -1;
    }
    v_from->edges[v_from->edge_count].to = to;
    v_from->edges[v_from->edge_count].weight = weight;
    v_from->edge_count++;

    if (!graph->directed) {
        /* 无向图需要加反向边 */
        if (graph_ensure_edge_capacity(v_to) != 0) {
            v_from->edge_count--;
            return -1;
        }
        v_to->edges[v_to->edge_count].to = from;
        v_to->edges[v_to->edge_count].weight = weight;
        v_to->edge_count++;
    }

    graph->edge_count++;
    return 0;
}

int ds_graph_remove_edge(ds_graph_t* graph, int from, int to)
{
    if (!graph) {
        return -1;
    }

    vertex_t* v_from = graph_get_vertex(graph, from);
    if (!v_from) {
        return -1;
    }

    int idx = graph_find_edge(v_from, to);
    if (idx < 0) {
        return -1;
    }

    /* 用最后一条边覆盖 */
    v_from->edges[idx] = v_from->edges[v_from->edge_count - 1];
    v_from->edge_count--;
    graph->edge_count--;

    if (!graph->directed) {
        vertex_t* v_to = graph_get_vertex(graph, to);
        if (v_to) {
            int idx2 = graph_find_edge(v_to, from);
            if (idx2 >= 0) {
                v_to->edges[idx2] = v_to->edges[v_to->edge_count - 1];
                v_to->edge_count--;
            }
        }
    }

    return 0;
}

int ds_graph_has_edge(const ds_graph_t* graph, int from, int to)
{
    if (!graph) {
        return 0;
    }
    vertex_t* v = graph_get_vertex((ds_graph_t*)graph, from);
    return v ? (graph_find_edge(v, to) >= 0) : 0;
}

int ds_graph_edge_weight(const ds_graph_t* graph, int from, int to,
                          double* out_weight)
{
    if (!graph || !out_weight) {
        return -1;
    }
    vertex_t* v = graph_get_vertex((ds_graph_t*)graph, from);
    if (!v) {
        return -1;
    }
    int idx = graph_find_edge(v, to);
    if (idx < 0) {
        return -1;
    }
    *out_weight = v->edges[idx].weight;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  遍历                                                               */
/* ------------------------------------------------------------------ */

/**
 * @brief 重置所有顶点的 visited 标志
 */
static void graph_reset_visited(ds_graph_t* graph)
{
    for (size_t i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
    }
}

int ds_graph_bfs(ds_graph_t* graph, int start,
                  ds_graph_visit_fn visit, void* ctx)
{
    if (!graph || !visit) {
        return -1;
    }

    int start_idx = graph_find_vertex(graph, start);
    if (start_idx < 0) {
        return -1;
    }

    graph_reset_visited(graph);

    /* 简易队列（数组实现） */
    int* queue = (int*)malloc(graph->vertex_count * sizeof(int));
    if (!queue) {
        return -1;
    }

    size_t front = 0, rear = 0;
    queue[rear++] = start_idx;
    graph->vertices[start_idx].visited = 1;

    while (front < rear) {
        int idx = queue[front++];
        vertex_t* v = &graph->vertices[idx];
        visit(v->id, v->data, ctx);

        for (size_t i = 0; i < v->edge_count; i++) {
            int neighbor_idx = graph_find_vertex(graph, v->edges[i].to);
            if (neighbor_idx >= 0 && !graph->vertices[neighbor_idx].visited) {
                graph->vertices[neighbor_idx].visited = 1;
                queue[rear++] = neighbor_idx;
            }
        }
    }

    free(queue);
    return 0;
}

/**
 * @brief DFS 递归辅助函数
 */
static void graph_dfs_recursive(ds_graph_t* graph, int idx,
                                 ds_graph_visit_fn visit, void* ctx)
{
    vertex_t* v = &graph->vertices[idx];
    v->visited = 1;
    visit(v->id, v->data, ctx);

    for (size_t i = 0; i < v->edge_count; i++) {
        int neighbor_idx = graph_find_vertex(graph, v->edges[i].to);
        if (neighbor_idx >= 0 && !graph->vertices[neighbor_idx].visited) {
            graph_dfs_recursive(graph, neighbor_idx, visit, ctx);
        }
    }
}

int ds_graph_dfs(ds_graph_t* graph, int start,
                  ds_graph_visit_fn visit, void* ctx)
{
    if (!graph || !visit) {
        return -1;
    }

    int start_idx = graph_find_vertex(graph, start);
    if (start_idx < 0) {
        return -1;
    }

    graph_reset_visited(graph);
    graph_dfs_recursive(graph, start_idx, visit, ctx);
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Dijkstra 最短路径                                                  */
/* ------------------------------------------------------------------ */

int ds_graph_dijkstra(const ds_graph_t* graph, int start,
                       double* out_dist, int* out_prev)
{
    if (!graph || !out_dist) {
        return -1;
    }

    int start_idx = graph_find_vertex(graph, start);
    if (start_idx < 0) {
        return -1;
    }

    size_t n = graph->vertex_count;

    /* 初始化距离和前驱 */
    for (size_t i = 0; i < n; i++) {
        out_dist[i] = INF_DIST;
        if (out_prev) out_prev[i] = -1;
    }
    out_dist[start_idx] = 0;

    char* settled = (char*)calloc(n, 1);
    if (!settled) {
        return -1;
    }

    /* 简单的 O(V^2) 实现（适合中等规模图） */
    for (size_t count = 0; count < n; count++) {
        /* 选择未 settled 且距离最小的顶点 */
        double min_dist = INF_DIST;
        int min_idx = -1;
        for (size_t i = 0; i < n; i++) {
            if (!settled[i] && out_dist[i] < min_dist) {
                min_dist = out_dist[i];
                min_idx = (int)i;
            }
        }

        if (min_idx < 0) {
            break;  /* 剩余顶点不可达 */
        }

        settled[min_idx] = 1;
        vertex_t* v = &graph->vertices[min_idx];

        /* 松弛所有邻接边 */
        for (size_t i = 0; i < v->edge_count; i++) {
            if (v->edges[i].weight < 0) {
                /* 检测负权边 */
                free(settled);
                return -1;
            }
            int neighbor_idx = graph_find_vertex(graph, v->edges[i].to);
            if (neighbor_idx < 0 || settled[neighbor_idx]) {
                continue;
            }
            double new_dist = out_dist[min_idx] + v->edges[i].weight;
            if (new_dist < out_dist[neighbor_idx]) {
                out_dist[neighbor_idx] = new_dist;
                if (out_prev) out_prev[neighbor_idx] = v->id;
            }
        }
    }

    free(settled);
    return 0;
}

/* ------------------------------------------------------------------ */
/*  拓扑排序                                                           */
/* ------------------------------------------------------------------ */

int ds_graph_topological_sort(ds_graph_t* graph,
                               int* out_order, size_t* out_size)
{
    if (!graph || !out_order || !out_size) {
        return -1;
    }

    if (!graph->directed) {
        return -1;  /* 无向图不能拓扑排序 */
    }

    size_t n = graph->vertex_count;

    /* 计算入度 */
    int* in_degree = (int*)calloc(n, sizeof(int));
    if (!in_degree) {
        return -1;
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < graph->vertices[i].edge_count; j++) {
            int neighbor_idx = graph_find_vertex(graph,
                                                  graph->vertices[i].edges[j].to);
            if (neighbor_idx >= 0) {
                in_degree[neighbor_idx]++;
            }
        }
    }

    /* 将入度为 0 的顶点入队 */
    int* queue = (int*)malloc(n * sizeof(int));
    if (!queue) {
        free(in_degree);
        return -1;
    }
    size_t front = 0, rear = 0;

    for (size_t i = 0; i < n; i++) {
        if (in_degree[i] == 0) {
            queue[rear++] = (int)i;
        }
    }

    /* 处理队列 */
    size_t count = 0;
    while (front < rear) {
        int idx = queue[front++];
        out_order[count++] = graph->vertices[idx].id;

        for (size_t j = 0; j < graph->vertices[idx].edge_count; j++) {
            int neighbor_idx = graph_find_vertex(graph,
                                                  graph->vertices[idx].edges[j].to);
            if (neighbor_idx >= 0) {
                in_degree[neighbor_idx]--;
                if (in_degree[neighbor_idx] == 0) {
                    queue[rear++] = neighbor_idx;
                }
            }
        }
    }

    *out_size = count;

    free(in_degree);
    free(queue);

    /* 如果处理的顶点数不等于总顶点数，说明有环 */
    return (count == n) ? 0 : -1;
}

/* ------------------------------------------------------------------ */
/*  状态查询                                                           */
/* ------------------------------------------------------------------ */

size_t ds_graph_vertex_count(const ds_graph_t* graph)
{
    return graph ? graph->vertex_count : 0;
}

size_t ds_graph_edge_count(const ds_graph_t* graph)
{
    return graph ? graph->edge_count : 0;
}

int ds_graph_is_directed(const ds_graph_t* graph)
{
    return graph ? graph->directed : 0;
}

int ds_graph_neighbors(const ds_graph_t* graph, int vertex,
                        ds_graph_edge_info_t* out_edges, size_t* out_count)
{
    if (!graph || !out_edges || !out_count) {
        return -1;
    }

    vertex_t* v = graph_get_vertex((ds_graph_t*)graph, vertex);
    if (!v) {
        return -1;
    }

    *out_count = v->edge_count;
    for (size_t i = 0; i < v->edge_count; i++) {
        out_edges[i].to = v->edges[i].to;
        out_edges[i].weight = v->edges[i].weight;
    }
    return 0;
}
