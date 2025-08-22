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

    // bind
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(DISCOVERY_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("Bind failed");
        close_socket(sock);
        cleanup_network();
        return 1;
    }

    printf("Responder listening on port %d...\n", DISCOVERY_PORT);

    char buffer[1024];
    struct sockaddr_in discoverer_addr;
    socklen_t addr_len = sizeof(discoverer_addr);

    while (1) {
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer)-1, 0,
                                     (struct sockaddr*)&discoverer_addr, &addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            if (strcmp(buffer, DISCOVER_MESSAGE) == 0) {
                // res
                char response[128];
                snprintf(response, sizeof(response), "%s%s%s", 
                        RESPONSE_PREFIX, "YourService", RESPONSE_SUFFIX);
                
                // send
                sendto(sock, response, strlen(response), 0,
                      (struct sockaddr*)&discoverer_addr, addr_len);
                
                printf("Responded to %s\n", inet_ntoa(discoverer_addr.sin_addr));
            }
        }
    }

    close_socket(sock);
    cleanup_network();
    return 0;
}