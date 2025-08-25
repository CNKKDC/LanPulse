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
 * @file discovery_common.h
 * @brief Common definitions and utilities for network discovery in LanPulse.
 * @author kkdc <1557655177@qq.com>
 */

#ifndef DISCOVERY_COMMON_H
#define DISCOVERY_COMMON_H

#include "autoconfig.h"

#define DISCOVERY_PORT 12345
#define DISCOVER_MESSAGE "DISCOVER"
#define RESPONSE_PREFIX "ME<"
#define RESPONSE_SUFFIX ">"

/* Master & Slave priority */
enum DiscoveryPriority{
    PEER_TO_PEER = 0,
    STRICT_MASTER,
    STRICT_SLAVE,
    PRIOR_MASTER,
    PRIOR_SLAVE,
    MS_PRIOR_MAX
}

typedef enum DiscoveryPriority DiscoveryPriority;

/* Discovery status */
enum DiscoveryStatus{
    DSTATUS_NONE = 0, /* start */
    DSTATUS_MULTI_SEND, /* UDP multicast send */
    DSTATUS_MULTI_RECV, /* UDP multicast recived */
    DSTATUS_UNI_CONFIRM, /
    DSTATUS_JOINED,
    DSTATUS_FIN,
    DSTATUS_EXITED,
    DSTATUS_MAX
}

typedef enum DiscoveryStatus DiscoveryStatus;

// 初始化网络库
int init_network(void);
// 清理网络库
void cleanup_network(void);
// 平台无关的socket关闭
void close_socket(SOCKET sock);

#endif /* DISCOVERY_COMMON_H */