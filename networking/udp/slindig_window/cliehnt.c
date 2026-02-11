#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10
#define MAX 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX];
    int base = 0;
    int next_frame = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (base < TOTAL_FRAMES) {

        // Send frames within window
        while (next_frame < base + WINDOW_SIZE && next_frame < TOTAL_FRAMES) {
            printf("Sending frame %d\n", next_frame);
            sprintf(buffer, "%d", next_frame);
            send(sock, buffer, strlen(buffer), 0);
            next_frame++;
            sleep(1);  // delay for clarity
        }

        // Receive ACK
        memset(buffer, 0, MAX);
        read(sock, buffer, MAX);
        int ack = atoi(buffer);

        printf("Received ACK %d\n", ack);

        if (ack >= base)
            base = ack + 1;
        else
            next_frame = base;  // Go-Back-N
    }

    printf("All frames sent successfully!\n");
    close(sock);
    return 0;
}
