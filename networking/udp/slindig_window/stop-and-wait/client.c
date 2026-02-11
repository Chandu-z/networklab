#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define PORT 8080

struct packet {
    int seq_no;
    char data[100];
};

struct ack_packet {
    int ack_no;
};

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct packet pkt;
    struct ack_packet ack;

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int seq = 0;

    while (1) {
        pkt.seq_no = seq;
        printf("Enter data: ");
        scanf("%s", pkt.data);

        sendto(sockfd, &pkt, sizeof(pkt), 0,
               (struct sockaddr*)&servaddr, sizeof(servaddr));

        printf("Frame Sent: Seq=%d\n", pkt.seq_no);

        if (recvfrom(sockfd, &ack, sizeof(ack), 0, NULL, NULL) < 0) {
            printf("Timeout → Retransmitting...\n");
            continue;
        }

        if (ack.ack_no == (seq + 1) % 2) {
            printf("ACK Received: %d\n\n", ack.ack_no);
            seq = (seq + 1) % 2;
        }
    }

    close(sockfd);
    return 0;
}
