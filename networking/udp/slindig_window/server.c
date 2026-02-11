#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int expected_frame = 0;
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server waiting for connection...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t *)&addrlen);

    while (1) {
        memset(buffer, 0, MAX);
        read(new_socket, buffer, MAX);

        int frame;
        sscanf(buffer, "%d", &frame);

        if (frame == expected_frame) {
            printf("Received frame %d (OK)\n", frame);
            expected_frame++;
        } else {
            printf("Received frame %d (OUT OF ORDER)\n", frame);
        }

        // Send ACK for last correctly received frame
        char ack[MAX];
        sprintf(ack, "%d", expected_frame - 1);
        send(new_socket, ack, strlen(ack), 0);

        if (expected_frame == 10)
            break;
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
