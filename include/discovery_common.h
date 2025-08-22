#ifndef DISCOVERY_COMMON_H
#define DISCOVERY_COMMON_H

#define DISCOVERY_PORT 12345
#define DISCOVER_MESSAGE "DISCOVER"
#define RESPONSE_PREFIX "ME<"
#define RESPONSE_SUFFIX ">"

// 初始化网络库
int init_network(void);
// 清理网络库
void cleanup_network(void);
// 平台无关的socket关闭
void close_socket(SOCKET sock);

#endif