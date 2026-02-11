#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define TOTAL_FRAMES 10
#define TIMEOUT 2

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t len;

    int base = 0;
    int nextseq = 0;
    int ack;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(servaddr);

    printf("Sender (Client) started...\n");

    while (base < TOTAL_FRAMES)
    {
        while (nextseq < base + WINDOW_SIZE && nextseq < TOTAL_FRAMES)
        {
            printf("Sending Frame: %d\n", nextseq);
            sendto(sockfd, &nextseq, sizeof(nextseq), 0,
                   (struct sockaddr *)&servaddr, len);
            nextseq++;
            sleep(1);
        }

        fd_set fds;
        struct timeval tv;

        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        int rv = select(sockfd + 1, &fds, NULL, NULL, &tv);

        if (rv > 0)
        {
            recvfrom(sockfd, &ack, sizeof(ack), 0,
                     (struct sockaddr *)&servaddr, &len);
            printf("ACK Received: %d\n", ack);
            base = ack + 1;
        }
        else
        {
            printf("Timeout → Go-Back-N Retransmission from %d\n", base);
            nextseq = base;
        }
    }

    int end = -1;
    sendto(sockfd, &end, sizeof(end), 0,
           (struct sockaddr *)&servaddr, len);

    close(sockfd);
    return 0;
}
