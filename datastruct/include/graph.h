/**
 * @file graph.h
 * @brief 通用图数据结构（Graph）
 *
 * 特性：
 * - 支持有向图和无向图
 * - 邻接表存储（适用于稀疏图）
 * - 顶点附带用户数据
 * - 带权边（double 类型权重）
 * - 内置 BFS、DFS 遍历
 * - 内置 Dijkstra 最短路径算法
 * - 拓扑排序（有向无环图）
 *
 * @author 飞机大战 EX Team
 * @version 1.0.0
 * @date 2026-05-27
 */

#ifndef DS_GRAPH_H
#define DS_GRAPH_H

#include <stddef.h>  /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/*  类型定义                                                           */
/* ------------------------------------------------------------------ */

/** @brief 图结构体（不透明） */
typedef struct ds_graph ds_graph_t;

/** @brief 边结构体（不透明） */
typedef struct ds_graph_edge ds_graph_edge_t;

/**
 * @brief 顶点访问回调
 * @param id   顶点 ID
 * @param data 顶点用户数据指针
 * @param ctx  用户上下文
 */
typedef void (*ds_graph_visit_fn)(int id, void* data, void* ctx);

/* ------------------------------------------------------------------ */
/*  生命周期管理                                                       */
/* ------------------------------------------------------------------ */

/**
 * @brief 创建图
 * @param directed 非 0 表示有向图，0 表示无向图
 * @return 成功返回图指针，失败返回 NULL
 */
ds_graph_t* ds_graph_create(int directed);

/**
 * @brief 销毁图
 * @param graph 图指针（允许 NULL）
 */
void ds_graph_destroy(ds_graph_t* graph);

/* ------------------------------------------------------------------ */
/*  顶点操作                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 添加顶点
 * @param graph     图指针
 * @param id        顶点 ID（必须 ≥ 0，且不与其他顶点重复）
 * @param data      顶点关联数据（可为 NULL）
 * @param data_size 数据大小（data 为 NULL 时填 0）
 * @return 0 成功，-1 失败（ID 重复或内存不足）
 *
 * @note data 会被拷贝到内部存储。若 data_size = 0，该顶点无附加数据。
 */
int ds_graph_add_vertex(ds_graph_t* graph, int id,
                         const void* data, size_t data_size);

/**
 * @brief 删除顶点及其所有关联的边
 * @param graph 图指针
 * @param id    顶点 ID
 * @return 0 成功，-1 顶点不存在
 */
int ds_graph_remove_vertex(ds_graph_t* graph, int id);

/**
 * @brief 判断顶点是否存在
 * @return 非 0 存在，0 不存在
 */
int ds_graph_has_vertex(const ds_graph_t* graph, int id);

/**
 * @brief 获取顶点关联数据
 * @param graph   图指针
 * @param id      顶点 ID
 * @param out_data 输出数据缓冲区（可为 NULL）
 * @return 0 成功，-1 顶点不存在
 */
int ds_graph_vertex_data(const ds_graph_t* graph, int id, void* out_data);

/* ------------------------------------------------------------------ */
/*  边操作                                                             */
/* ------------------------------------------------------------------ */

/**
 * @brief 添加边
 * @param graph  图指针
 * @param from   源顶点 ID
 * @param to     目标顶点 ID
 * @param weight 边的权重（默认 1.0）
 * @return 0 成功，-1 失败
 *
 * @note 无向图时，会自动添加反向边。
 *       若边已存在，会更新权重。
 */
int ds_graph_add_edge(ds_graph_t* graph, int from, int to, double weight);

/**
 * @brief 删除边
 * @param graph 图指针
 * @param from  源顶点 ID
 * @param to    目标顶点 ID
 * @return 0 成功，-1 边不存在
 */
int ds_graph_remove_edge(ds_graph_t* graph, int from, int to);

/**
 * @brief 判断边是否存在
 * @return 非 0 存在，0 不存在
 */
int ds_graph_has_edge(const ds_graph_t* graph, int from, int to);

/**
 * @brief 获取边的权重
 * @param graph      图指针
 * @param from       源顶点 ID
 * @param to         目标顶点 ID
 * @param out_weight 输出权重
 * @return 0 成功，-1 边不存在
 */
int ds_graph_edge_weight(const ds_graph_t* graph, int from, int to,
                          double* out_weight);

/* ------------------------------------------------------------------ */
/*  遍历与路径                                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief 广度优先搜索
 * @param graph  图指针
 * @param start  起始顶点 ID
 * @param visit  每个顶点访问一次的回调
 * @param ctx    用户上下文
 * @return 0 成功，-1 起始顶点不存在
 */
int ds_graph_bfs(ds_graph_t* graph, int start,
                  ds_graph_visit_fn visit, void* ctx);

/**
 * @brief 深度优先搜索
 * @param graph  图指针
 * @param start  起始顶点 ID
 * @param visit  每个顶点访问一次的回调
 * @param ctx    用户上下文
 * @return 0 成功，-1 起始顶点不存在
 */
int ds_graph_dfs(ds_graph_t* graph, int start,
                  ds_graph_visit_fn visit, void* ctx);

/**
 * @brief Dijkstra 最短路径算法
 * @param graph    图指针（权重必须非负）
 * @param start    起始顶点 ID
 * @param out_dist 输出距离数组（调用者分配，至少 graph 顶点数项）
 * @param out_prev 输出前驱数组（调用者分配，至少 graph 顶点数项，可为 NULL）
 * @return 0 成功，-1 起始顶点不存在或存在负权边
 *
 * @note 不可达顶点的距离为 INF（1e18）。
 *       out_prev 中不可达顶点的前驱为 -1。
 *       调用者需确保 out_dist 和 out_prev 容量足够。
 */
int ds_graph_dijkstra(const ds_graph_t* graph, int start,
                       double* out_dist, int* out_prev);

/**
 * @brief 拓扑排序（仅适用于有向无环图）
 * @param graph     图指针
 * @param out_order 输出排序后的顶点 ID 数组（调用者分配足够空间）
 * @param out_size  输出实际排序的顶点数
 * @return 0 成功，-1 图中有环或非有向图
 */
int ds_graph_topological_sort(ds_graph_t* graph,
                               int* out_order, size_t* out_size);

/* ------------------------------------------------------------------ */
/*  状态查询                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 获取顶点数量
 */
size_t ds_graph_vertex_count(const ds_graph_t* graph);

/**
 * @brief 获取边数量
 */
size_t ds_graph_edge_count(const ds_graph_t* graph);

/**
 * @brief 判断是否为有向图
 */
int ds_graph_is_directed(const ds_graph_t* graph);

/**
 * @brief 获取某顶点的邻接边（用于遍历邻接关系）
 * @param graph    图指针
 * @param vertex   顶点 ID
 * @param out_edges 输出边数组首地址（调用者分配）
 * @param out_count 输出边数
 * @return 0 成功，-1 顶点不存在
 *
 * @note 每条边包含 to（目标顶点 ID）和 weight（权重）。
 *       调用者可遍历 out_edges[0..out_count-1]。
 */
typedef struct {
    int     to;     /**< 目标顶点 */
    double  weight; /**< 边权重 */
} ds_graph_edge_info_t;

int ds_graph_neighbors(const ds_graph_t* graph, int vertex,
                        ds_graph_edge_info_t* out_edges, size_t* out_count);

#ifdef __cplusplus
}
#endif

#endif /* DS_GRAPH_H */
