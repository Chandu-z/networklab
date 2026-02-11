#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    int frames = 10;
    int base = 0, nextseq = 0;
    int ack;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Connected to server\n");

    while (base < frames)
    {
        while (nextseq < base + WINDOW_SIZE && nextseq < frames)
        {
            sleep(1);   // 🔹 Delay before sending each packet

            printf("Sending Frame: %d\n", nextseq);
            send(sockfd, &nextseq, sizeof(nextseq), 0);
            nextseq++;
        }

        recv(sockfd, &ack, sizeof(ack), 0);
        printf("Received ACK: %d\n", ack);

        base = ack + 1;
    }

    int end = -1;
    send(sockfd, &end, sizeof(end), 0);

    close(sockfd);
    return 0;
}
