#include "discovery/router.h"

extern Graph* g_dev_topology;
extern GraphNode* g_cur_dev;

Path* path_create(int *node_ids, int length, float total_cost) {
    Path *path = (Path*)MALLOC_S(sizeof(Path));
    if (!path) return NULL;
    
    path->node_ids = (int*)MALLOC_S(length * sizeof(int));
    if (!path->node_ids) {
        FREE_S(path);
        return NULL;
    }
    
    memcpy(path->node_ids, node_ids, length * sizeof(int));
    path->length = length;
    path->total_cost = total_cost;
    
    return path;
}

void path_destroy(Path *path) {
    if (!path) return;
    
    if (path->node_ids) {
        FREE_S(path->node_ids);
    }
    FREE_S(path);
}

PathWithEdges* path_with_edges_create(GraphNode **nodes, EdgeData **edges, int length, float total_cost) {
    PathWithEdges *path = (PathWithEdges*)MALLOC_S(sizeof(PathWithEdges));
    if (!path) return NULL;
    
    path->nodes = (GraphNode**)MALLOC_S(length * sizeof(GraphNode*));
    path->edges = (EdgeData**)MALLOC_S((length - 1) * sizeof(EdgeData*));
    
    if (!path->nodes || !path->edges) {
        if (path->nodes) FREE_S(path->nodes);
        if (path->edges) FREE_S(path->edges);
        FREE_S(path);
        return NULL;
    }
    
    memcpy(path->nodes, nodes, length * sizeof(GraphNode*));
    if (length > 1) {
        memcpy(path->edges, edges, (length - 1) * sizeof(EdgeData*));
    }
    
    path->length = length;
    path->total_cost = total_cost;
    
    return path;
}

void path_with_edges_destroy(PathWithEdges *path) {
    if (!path) return;
    
    if (path->nodes) FREE_S(path->nodes);
    if (path->edges) FREE_S(path->edges);
    FREE_S(path);
}

// 路径列表操作
PathList* path_list_create() {
    PathList *list = (PathList*)MALLOC_S(sizeof(PathList));
    if (!list) return NULL;
    
    list->head = list->tail = NULL;
    list->count = 0;
    
    return list;
}

void path_list_add(PathList *list, Path *path) {
    if (!list || !path) return;
    
    PathListNode *node = (PathListNode*)MALLOC_S(sizeof(PathListNode));
    if (!node) return;
    
    node->path = path;
    node->next = NULL;
    
    if (list->tail) {
        list->tail->next = node;
        list->tail = node;
    } else {
        list->head = list->tail = node;
    }
    
    list->count++;
}

void path_list_remove(PathList *list, Path *path) {
    if (!list || !path || !list->head) return;
    
    PathListNode *prev = NULL;
    PathListNode *current = list->head;
    
    while (current) {
        if (current->path == path) {
            if (prev) {
                prev->next = current->next;
            } else {
                list->head = current->next;
            }
            
            if (current == list->tail) {
                list->tail = prev;
            }
            
            FREE_S(current);
            list->count--;
            return;
        }
        
        prev = current;
        current = current->next;
    }
}

void path_list_destroy(PathList *list) {
    if (!list) return;
    
    PathListNode *current = list->head;
    while (current) {
        PathListNode *next = current->next;
        path_destroy(current->path);
        FREE_S(current);
        current = next;
    }
    
    FREE_S(list);
}

// 路径工具函数
void path_print(Path *path, Graph *graph) {
    if (!path || !graph) return;
    
    printf("Path (cost: %.2f): ", path->total_cost);
    for (int i = 0; i < path->length; i++) {
        GraphNode *node = graph_get_node(graph, path->node_ids[i]);
        if (node) {
            printf("%s", node->data.hostname);
        } else {
            printf("[%d]", path->node_ids[i]);
        }
        
        if (i < path->length - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

void path_with_edges_print(PathWithEdges *path) {
    if (!path) return;
    
    printf("Path (cost: %.2f): ", path->total_cost);
    for (int i = 0; i < path->length; i++) {
        printf("%s", path->nodes[i]->data.hostname);
        
        if (i < path->length - 1) {
            printf(" -(%.2f)-> ", path->edges[i]->latency);
        }
    }
    printf("\n");
}

// 路径查找算法
Path* graph_find_shortest_path(Graph *graph, int start_id, int end_id) {
    if (!graph) return NULL;
    
    // 使用Dijkstra算法查找最短路径
    int node_count = graph->node_count;
    float *distances = (float*)MALLOC_S(node_count * sizeof(float));
    int *previous = (int*)MALLOC_S(node_count * sizeof(int));
    bool *visited = (bool*)MALLOC_S(node_count * sizeof(bool));
    
    if (!distances || !previous || !visited) {
        if (distances) FREE_S(distances);
        if (previous) FREE_S(previous);
        if (visited) FREE_S(visited);
        return NULL;
    }
    
    // 初始化
    for (int i = 0; i < node_count; i++) {
        distances[i] = FLT_MAX;
        previous[i] = -1;
        visited[i] = false;
    }
    
    distances[start_id] = 0;
    
    // 主循环
    for (int i = 0; i < node_count - 1; i++) {
        // 找到未访问的最小距离节点
        float min_distance = FLT_MAX;
        int min_index = -1;
        
        for (int j = 0; j < node_count; j++) {
            if (!visited[j] && distances[j] < min_distance) {
                min_distance = distances[j];
                min_index = j;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = true;
        
        // 更新邻居节点的距离
        GraphNode *node = graph_get_node(graph, min_index);
        for (int j = 0; j < node->neighbor_count; j++) {
            GraphNode *neighbor = node->neighbors[j];
            EdgeData *edge = node->edge_data[j];
            float alt = distances[min_index] + edge->latency; // 使用延迟作为成本
            
            if (alt < distances[neighbor->id]) {
                distances[neighbor->id] = alt;
                previous[neighbor->id] = min_index;
            }
        }
    }
    
    // 构建路径
    if (distances[end_id] == FLT_MAX) {
        // 没有路径
        FREE_S(distances);
        FREE_S(previous);
        FREE_S(visited);
        return NULL;
    }
    
    // 计算路径长度
    int path_length = 1;
    int current = end_id;
    while (current != start_id) {
        path_length++;
        current = previous[current];
    }
    
    // 创建路径数组
    int *path_nodes = (int*)MALLOC_S(path_length * sizeof(int));
    if (!path_nodes) {
        FREE_S(distances);
        FREE_S(previous);
        FREE_S(visited);
        return NULL;
    }
    
    current = end_id;
    for (int i = path_length - 1; i >= 0; i--) {
        path_nodes[i] = current;
        current = previous[current];
    }
    
    Path *path = path_create(path_nodes, path_length, distances[end_id]);
    
    FREE_S(distances);
    FREE_S(previous);
    FREE_S(visited);
    FREE_S(path_nodes);
    
    return path;
}

// int main() {
//     // 创建图（使用之前定义的图结构）
//     // 添加节点和边（省略具体代码）
//     // ...
    
//     // 查找最短路径
//     Path *shortest_path = graph_find_shortest_path(graph, 1, 3);
//     if (shortest_path) {
//         path_print(shortest_path, graph);
//         path_destroy(shortest_path);
//     }
    
//     // 创建路径列表
//     PathList *path_list = path_list_create();
    
//     // 添加多条路径到列表
//     int path1_nodes[] = {1, 2, 3};
//     Path *path1 = path_create(path1_nodes, 3, 5.0);
//     path_list_add(path_list, path1);
    
//     int path2_nodes[] = {1, 4, 3};
//     Path *path2 = path_create(path2_nodes, 3, 7.0);
//     path_list_add(path_list, path2);
    
//     // 打印所有路径
//     PathListNode *current = path_list->head;
//     while (current) {
//         path_print(current->path, graph);
//         current = current->next;
//     }
    
//     // 清理
//     path_list_destroy(path_list);
//     graph_destroy(graph);
    
//     return 0;
// }