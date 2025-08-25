/*
 *  Copyright (C) 2025 kkdc <1557655177@qq.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

/**
 * @file graph.c
 * @author kkdc <1557655177@qq.com>
 */
 
 #include "discovery/graph.h"

/* Global */

static Graph* g_dev_topology = NULL;
static GraphNode* g_cur_dev = NULL;

// Graph create and destory
Graph* GraphCreate(bool directed) {
    Graph *graph = (Graph*)MALLOC_S(sizeof(Graph));
    if (!graph) return NULL;
    
    graph->nodes = (GraphNode**)MALLOC_S(10 * sizeof(GraphNode*));
    graph->node_count = 0;
    graph->capacity = 10;
    graph->directed = directed;
    
    return graph;
}

void GraphDestroy(Graph *graph) {
    if (!graph) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        GraphNode *node = graph->nodes[i];
        free(node->neighbors);
        free(node->edge_data);
        if (node->data.interfaces) {
            free(node->data.interfaces);
        }
        if (node->data.custom_data) {
            free(node->data.custom_data);
        }
        free(node);
    }
    
    free(graph->nodes);
    free(graph);
}

// 节点操作
GraphNode* GraphAddNode(Graph *graph, Device data) {
    if (!graph || graph->node_count >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        GraphNode **new_nodes = (GraphNode**)RELLOC_S(graph->nodes, new_capacity * sizeof(GraphNode*));
        if (!new_nodes) return NULL;
        
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    
    GraphNode *new_node = (GraphNode*)MALLOC_S(sizeof(GraphNode));
    if (!new_node) return NULL;
    
    static int next_id = 1;
    new_node->id = next_id++;
    new_node->data = data;
    new_node->neighbors = (GraphNode**)MALLOC_S(5 * sizeof(GraphNode*));
    new_node->edge_data = (EdgeData**)MALLOC_S(5 * sizeof(EdgeData*));
    new_node->neighbor_count = 0;
    new_node->capacity = 5;
    
    graph->nodes[graph->node_count++] = new_node;
    return new_node;
}

bool GraphRemoveNode(Graph *graph, int node_id) {
    if (!graph) return false;
    
    // 查找节点
    int index = -1;
    GraphNode *target = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == node_id) {
            index = i;
            target = graph->nodes[i];
            break;
        }
    }
    
    if (!target) return false;
    
    // 从所有邻居中移除该节点
    for (int i = 0; i < graph->node_count; i++) {
        GraphNode *node = graph->nodes[i];
        for (int j = 0; j < node->neighbor_count; j++) {
            if (node->neighbors[j] == target) {
                // 移除边
                free(node->edge_data[j]);
                // 将最后一个元素移到当前位置
                node->neighbors[j] = node->neighbors[node->neighbor_count - 1];
                node->edge_data[j] = node->edge_data[node->neighbor_count - 1];
                node->neighbor_count--;
                break;
            }
        }
    }
    
    // 释放节点资源
    free(target->neighbors);
    free(target->edge_data);
    if (target->data.interfaces) {
        free(target->data.interfaces);
    }
    if (target->data.custom_data) {
        free(target->data.custom_data);
    }
    free(target);
    
    // 将最后一个节点移到当前位置
    graph->nodes[index] = graph->nodes[graph->node_count - 1];
    graph->node_count--;
    
    return true;
}

GraphNode* GraphGetNode(Graph *graph, int node_id) {
    if (!graph) return NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == node_id) {
            return graph->nodes[i];
        }
    }
    
    return NULL;
}

// 边操作
bool GraphAddEdge(Graph *graph, int from_id, int to_id, EdgeData data) {
    if (!graph) return false;
    
    GraphNode *from = GraphGetNode(graph, from_id);
    GraphNode *to = GraphGetNode(graph, to_id);
    if (!from || !to) return false;
    
    // 检查是否已存在边
    for (int i = 0; i < from->neighbor_count; i++) {
        if (from->neighbors[i] == to) {
            // 更新现有边数据
            *(from->edge_data[i]) = data;
            return true;
        }
    }
    
    // 需要扩容
    if (from->neighbor_count >= from->capacity) {
        int new_capacity = from->capacity * 2;
        GraphNode **new_neighbors = (GraphNode**)RELLOC_S(from->neighbors, new_capacity * sizeof(GraphNode*));
        EdgeData **new_edge_data = (EdgeData**)RELLOC_S(from->edge_data, new_capacity * sizeof(EdgeData*));
        
        if (!new_neighbors || !new_edge_data) return false;
        
        from->neighbors = new_neighbors;
        from->edge_data = new_edge_data;
        from->capacity = new_capacity;
    }
    
    // 添加新边
    EdgeData *new_edge_data = (EdgeData*)MALLOC_S(sizeof(EdgeData));
    if (!new_edge_data) return false;
    
    *new_edge_data = data;
    from->neighbors[from->neighbor_count] = to;
    from->edge_data[from->neighbor_count] = new_edge_data;
    from->neighbor_count++;
    
    // 如果是无向图，添加反向边
    if (!graph->directed) {
        GraphAddEdge(graph, to_id, from_id, data);
    }
    
    return true;
}

bool GraphRemoveEdge(Graph *graph, int from_id, int to_id) {
    if (!graph) return false;
    
    GraphNode *from = GraphGetNode(graph, from_id);
    GraphNode *to = GraphGetNode(graph, to_id);
    if (!from || !to) return false;
    
    for (int i = 0; i < from->neighbor_count; i++) {
        if (from->neighbors[i] == to) {
            free(from->edge_data[i]);
            // 将最后一个元素移到当前位置
            from->neighbors[i] = from->neighbors[from->neighbor_count - 1];
            from->edge_data[i] = from->edge_data[from->neighbor_count - 1];
            from->neighbor_count--;
            
            // 如果是无向图，移除反向边
            if (!graph->directed) {
                GraphRemoveEdge(graph, to_id, from_id);
            }
            
            return true;
        }
    }
    
    return false;
}

EdgeData* GraphGetEdge(Graph *graph, int from_id, int to_id) {
    if (!graph) return NULL;
    
    GraphNode *from = GraphGetNode(graph, from_id);
    GraphNode *to = GraphGetNode(graph, to_id);
    if (!from || !to) return NULL;
    
    for (int i = 0; i < from->neighbor_count; i++) {
        if (from->neighbors[i] == to) {
            return from->edge_data[i];
        }
    }
    
    return NULL;
}

// 节点数据操作
void NodeInit(Device *data, Platform platform, SubPlatType subplatform) {
    memset(data, 0, sizeof(Device));
    data->platform = platform;
    data->subplatform = subplatform;
    data->interface_count = 0;
    data->interfaces = NULL;
}

void NodeAddInterface(Device *data, const char *name, const char *ip_v4, const char *ip_v6, 
                       const char *mac, unsigned int mtu) {
    if (!data) return;
    
    // 分配或重新分配接口数组
    NetworkInterface *new_interfaces = (NetworkInterface*)RELLOC_S(
        data->interfaces, 
        (data->interface_count + 1) * sizeof(NetworkInterface)
    );
    
    if (!new_interfaces) return;
    
    data->interfaces = new_interfaces;
    NetworkInterface *iface = &data->interfaces[data->interface_count];
    
    strncpy(iface->name, name, sizeof(iface->name) - 1);
    strncpy(iface->ip.v4, ip_v4, sizeof(iface->ip.v4) - 1);
    strncpy(iface->ip.v6, ip_v6, sizeof(iface->ip.v6) - 1);
    strncpy(iface->mac, mac, sizeof(iface->mac) - 1);
    iface->mtu = mtu;
    
    data->interface_count++;
}

void NodeSetPublicIp(Device *data, const char *ip_v4, const char *ip_v6) {
    if (!data) return;
    if (ip_v4) strncpy(data->public_ip.v4, ip_v4, sizeof(data->public_ip.v4) - 1);
    if (ip_v6) strncpy(data->public_ip.v6, ip_v6, sizeof(data->public_ip.v6) - 1);
}

void NodeSetPrivateIp(Device *data, const char *ip_v4, const char *ip_v6) {
    if (!data) return;
    if (ip_v4) strncpy(data->private_ip.v4, ip_v4, sizeof(data->private_ip.v4) - 1);
    if (ip_v6) strncpy(data->private_ip.v6, ip_v6, sizeof(data->private_ip.v6) - 1);
}

// 工具函数
const char* PlatformToString(Platform platform) {
    switch (platform) {
        case PLAT_NONE: return "None";
        case PLAT_UNIX: return "Unix";
        case PLAT_LINUX: return "Linux";
        case PLAT_APPLE: return "Apple";
        case PLAT_ANDROID: return "Android";
        case PLAT_WINDOWS: return "Windows";
        default: return "Unknown";
    }
}

const char* SubPlatformToString(SubPlatType subplatform) {
    switch (subplatform) {
        case SUBPLAT_NONE: return "None";
        case SUBPLAT_DEBIAN: return "Debian";
        case SUBPLAT_REDHAT: return "RedHat";
        case SUBPLAT_ARCH: return "Arch";
        case SUBPLAT_UBUNTU: return "Ubuntu";
        case SUBPLAT_MACOS: return "macOS";
        case SUBPLAT_IOS: return "iOS";
        case SUBPLAT_ANDROID_PHONE: return "Android Phone";
        case SUBPLAT_ANDROID_TABLET: return "Android Tablet";
        case SUBPLAT_WINDOWS_DESKTOP: return "Windows Desktop";
        case SUBPLAT_WINDOWS_SERVER: return "Windows Server";
        default: return "Unknown";
    }
}

void PrintNodeInfo(GraphNode *node) {
    if (!node) return;
    
    printf("Node ID: %d\n", node->id);
    printf("Platform: %s\n", PlatformToString(node->data.platform));
    printf("Subplatform: %s\n", SubPlatformToString(node->data.subplatform));
    printf("Hostname: %s\n", node->data.hostname);
    printf("OS Version: %s\n", node->data.os_version);
    printf("Architecture: %s\n", node->data.architecture);
    printf("Public IP: %s / %s\n", node->data.public_ip.v4, node->data.public_ip.v6);
    printf("Private IP: %s / %s\n", node->data.private_ip.v4, node->data.private_ip.v6);
    printf("Interfaces: %d\n", node->data.interface_count);
    
    for (int i = 0; i < node->data.interface_count; i++) {
        NetworkInterface *iface = &node->data.interfaces[i];
        printf("  %s: %s/%s, MAC: %s, MTU: %u\n", 
               iface->name, iface->ip.v4, iface->ip.v6, iface->mac, iface->mtu);
    }
    
    printf("Neighbors: %d\n", node->neighbor_count);
    for (int i = 0; i < node->neighbor_count; i++) {
        printf("  -> Node %d\n", node->neighbors[i]->id);
    }
    printf("\n");
}

void PrintEdgeInfo(Graph *graph, int from_id, int to_id) {
    EdgeData *edge = GraphGetEdge(graph, from_id, to_id);
    if (!edge) {
        printf("No edge from %d to %d\n", from_id, to_id);
        return;
    }
    
    printf("Edge from %d to %d:\n", from_id, to_id);
    printf("  Bandwidth: %u Mbps\n", edge->bandwidth);
    printf("  Latency: %.2f ms\n", edge->latency);
    printf("  Packet Loss: %.2f%%\n", edge->packet_loss);
    printf("  Protocol: %s\n", edge->protocol);
    printf("  Port: %u\n", edge->port);
    printf("  Traffic: %lu bytes\n", edge->traffic);
    printf("  Last Communication: %s", ctime(&edge->last_communication));
}

// int main() {
//     // 创建有向图
//     Graph *graph = graph_create(true);
    
//     // 创建节点数据
//     Device node1_data;
//     node_init(&node1_data, PLAT_LINUX, SUBPLAT_UBUNTU);
//     strcpy(node1_data.hostname, "server1");
//     strcpy(node1_data.os_version, "20.04");
//     strcpy(node1_data.architecture, "x86_64");
//     node1_data.memory = 16 * 1024 * 1024 * 1024UL; // 16GB
//     node1_data.storage = 500 * 1024 * 1024 * 1024UL; // 500GB
//     node_set_public_ip(&node1_data, "203.0.113.10", "2001:db8::a00:20ff:fea7:ccea");
//     node_set_private_ip(&node1_data, "192.168.1.10", "fd00::a00:20ff:fea7:ccea");
//     node_add_interface(&node1_data, "eth0", "192.168.1.10", "fd00::a00:20ff:fea7:ccea", 
//                       "00:11:22:33:44:55", 1500);
    
//     Device node2_data;
//     node_init(&node2_data, PLAT_WINDOWS, SUBPLAT_WINDOWS_DESKTOP);
//     strcpy(node2_data.hostname, "client1");
//     strcpy(node2_data.os_version, "10");
//     strcpy(node2_data.architecture, "x86_64");
//     node2_data.memory = 8 * 1024 * 1024 * 1024UL; // 8GB
//     node_set_private_ip(&node2_data, "192.168.1.20", NULL);
//     node_add_interface(&node2_data, "eth0", "192.168.1.20", NULL, 
//                       "AA:BB:CC:DD:EE:FF", 1500);
    
//     // 添加节点到图中
//     GraphNode *node1 = graph_add_node(graph, node1_data);
//     GraphNode *node2 = graph_add_node(graph, node2_data);
    
//     // 添加边数据
//     EdgeData edge_data;
//     edge_data.bandwidth = 1000;
//     edge_data.latency = 2.5;
//     edge_data.packet_loss = 0.1;
//     strcpy(edge_data.protocol, "TCP");
//     edge_data.port = 80;
//     edge_data.traffic = 1024 * 1024 * 100; // 100MB
//     edge_data.last_communication = time(NULL);
    
//     graph_add_edge(graph, node1->id, node2->id, edge_data);
    
//     // 打印节点信息
//     print_node_info(node1);
//     print_node_info(node2);
    
//     // 打印边信息
//     print_edge_info(graph, node1->id, node2->id);
    
//     // 清理
//     graph_destroy(graph);
    
//     return 0;
// }