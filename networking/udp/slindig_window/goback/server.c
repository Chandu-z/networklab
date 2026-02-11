#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    int expected = 0;
    int frame;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Receiver (Server) started...\n");

    len = sizeof(cliaddr);

    while (1)
    {
        recvfrom(sockfd, &frame, sizeof(frame), 0,
                 (struct sockaddr *)&cliaddr, &len);

        if (frame == -1)
            break;

        printf("Received Frame: %d\n", frame);

        if (frame == expected)
        {
            printf("Frame %d accepted\n", frame);
            sendto(sockfd, &frame, sizeof(frame), 0,
                   (struct sockaddr *)&cliaddr, len);
            expected++;
        }
        else
        {
            printf("Out-of-order frame. Expected: %d\n", expected);
            int ack = expected - 1;
            sendto(sockfd, &ack, sizeof(ack), 0,
                   (struct sockaddr *)&cliaddr, len);
        }
    }

    close(sockfd);
    return 0;
}
