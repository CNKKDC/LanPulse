#include "discovery_common.h"
#include <stdio.h>
#include <string.h>

int main() {
    if (init_network() != 0) {
        fprintf(stderr, "Network init failed\n");
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("Socket creation failed");
        cleanup_network();
        return 1;
    }

    // 设置广播选项
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, 
                  (char*)&broadcast, sizeof(broadcast)) < 0) {
        perror("Setsockopt SO_BROADCAST failed");
        close_socket(sock);
        cleanup_network();
        return 1;
    }

    // 设置广播地址
    struct sockaddr_in bc_addr;
    memset(&bc_addr, 0, sizeof(bc_addr));
    bc_addr.sin_family = AF_INET;
    bc_addr.sin_port = htons(DISCOVERY_PORT);
    bc_addr.sin_addr.s_addr = INADDR_BROADCAST; // 255.255.255.255

    // 发送发现包
    if (sendto(sock, DISCOVER_MESSAGE, strlen(DISCOVER_MESSAGE), 0,
              (struct sockaddr*)&bc_addr, sizeof(bc_addr)) < 0) {
        perror("Send broadcast failed");
        close_socket(sock);
        cleanup_network();
        return 1;
    }

    printf("Discovery message sent. Waiting for responses...\n");

    // 接收响应
    char buffer[1024];
    struct sockaddr_in responder_addr;
    socklen_t addr_len = sizeof(responder_addr);

    while (1) {
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer)-1, 0,
                                     (struct sockaddr*)&responder_addr, &addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            char* ip_str = inet_ntoa(responder_addr.sin_addr);
            printf("Found device: IP=%s, Message=%s\n", ip_str, buffer);
        }
    }

    close_socket(sock);
    cleanup_network();
    return 0;
}