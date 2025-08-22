#include "discovery_common.h"
#include <stdio.h>

int init_network() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
    return 0;
#endif
}

void cleanup_network() {
#ifdef _WIN32
    WSACleanup();
#endif
}

void close_socket(SOCKET sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}