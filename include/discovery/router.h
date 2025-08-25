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
 * @file router.h
 * @author kkdc <1557655177@qq.com>
 */

#ifndef __ROUTER_H__
#define __ROUTER_H__

#include "util/memory.h"
#include "discovery/graph.h"

// 路径结构 - 只存储节点ID
typedef struct {
    int *node_ids;   // 节点ID数组
    int length;      // 路径长度（节点数量）
    float total_cost; // 路径总成本
} Path;

// 带边信息的路径结构
typedef struct {
    GraphNode **nodes; // 节点指针数组
    EdgeData **edges;  // 边数据指针数组
    int length;        // 路径长度（节点数量）
    float total_cost;  // 路径总成本
} PathWithEdges;

// 路径列表节点
typedef struct PathListNode {
    Path *path;
    struct PathListNode *next;
} PathListNode;

// 路径列表
typedef struct {
    PathListNode *head;
    PathListNode *tail;
    int count;
} PathList;

#endif /* __ROUTER_H__ */