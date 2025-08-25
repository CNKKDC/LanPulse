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
 * @file graph.h
 * @author kkdc <1557655177@qq.com>
 */

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "util/memory.h"

/* Platform */
typedef enum {
    PLAT_NONE = 0,
    PLAT_UNIX,
    PLAT_LINUX,
    PLAT_APPLE,
    PLAT_ANDROID,
    PLAT_WINDOWS,
    PLAT_MAX
} Platform;

/* Sub-Platform */
typedef enum {
    SUBPLAT_NONE = 0,
    SUBPLAT_DEBIAN,
    SUBPLAT_REDHAT,
    SUBPLAT_ARCH,
    SUBPLAT_UBUNTU,
    SUBPLAT_MACOS,
    SUBPLAT_IOS,
    SUBPLAT_ANDROID_PHONE,
    SUBPLAT_ANDROID_TABLET,
    SUBPLAT_WINDOWS_DESKTOP,
    SUBPLAT_WINDOWS_SERVER,
    SUBPLAT_MAX
} SubPlatType;

/* Address */
typedef struct IPAddress_ {
    char family;
    union {
        struct in6_addr addr_in6;
        uint8_t         addr_u8[16];
        uint16_t        addr_u16[8];
        uint32_t        addr_u32[4];
    } address;
} IPAddress;

typedef struct NetworkInterface_ {
    char name[128];     /* Interface */
    IPAddress ip;       /* IP */
    char mac[18];       /* MAC */
    unsigned int mtu;   /* MTU */
} NetworkInterface;

/* Node data */
typedef struct Device_ {
    Platform platform;        /* Platform */
    SubPlatType subplatform;  /* Sub-Platform */
    char hostname[128];       /* Hostname */
    char os_version[32];      /* OS Version */
    char architecture[16];    /* Architecture */
    unsigned long memory;     /* Memory total */
    unsigned long storage;    /* Storage total */
    IPAddress public_ip;      /* Public IP */
    IPAddress private_ip;     /* Private IP */
    NetworkInterface *ifaces; /* Interfaces List */
    NetworkInterface *iface;  /* Interfaces Used */
    int iface_count;          /* Interfaces count */
    void *data;
} Device;

/* Edge */
typedef struct EdgeData_ {
    char utilize;
    unsigned int bandwidth;     /* Band（Mbps） */
    float latency;              /* Ping（ms） */
    float packet_loss;          /* Loss（%） */
    unsigned short port;        /* Port */
    unsigned long traffic;      /* Traffic used */
    time_t last_communication;  /* Last communication */
    void *data;
} EdgeData;

/* Node */
typedef struct GraphNode_ {
    int id;
    Device data;
    struct GraphNode **neighbors;
    struct EdgeData **edge_data;
    int neighbor_count;
    int capacity;
    // TODO: LAN IDS
} GraphNode;

/* Graph */
typedef struct {
    GraphNode **nodes;
    int node_count;
    int capacity;
    bool directed;
} Graph;

/* Function */

Graph* GraphCreate(bool directed);
void GraphDestroy(Graph *graph);

GraphNode* GraphAddNode(Graph *graph, Device data);
bool GraphRemoveNode(Graph *graph, int node_id);
GraphNode* GraphGetNode(Graph *graph, int node_id);

bool GraphAddEdge(Graph *graph, int from_id, int to_id, EdgeData data);
bool GraphRemoveEdge(Graph *graph, int from_id, int to_id);
EdgeData* GraphGetEdge(Graph *graph, int from_id, int to_id);

void GraphDFS(Graph *graph, int start_id, void (*visit)(GraphNode*));
void GraphBFS(Graph *graph, int start_id, void (*visit)(GraphNode*));
int GraphShortestPath(Graph *graph, int start_id, int end_id, int **path);

void NodeInit(Device *data, Platform platform, SubPlatType subplatform);
void NodeAddInterface(Device *data, const char *name, const char *ip_v4, const char *ip_v6, 
                       const char *mac, unsigned int mtu);
void NodeSetPublicIp(Device *data, const char *ip_v4, const char *ip_v6);
void NodeSetPrivateIp(Device *data, const char *ip_v4, const char *ip_v6);

const char* PlatformToString(Platform platform);
const char* SubPlatformToString(SubPlatType subplatform);
void PrintNodeInfo(GraphNode *node);
void PrintEdgeInfo(Graph *graph, int from_id, int to_id);


#endif /* __GRAPH_H__ */