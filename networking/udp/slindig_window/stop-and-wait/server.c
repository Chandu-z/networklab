#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

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
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    struct packet pkt;
    struct ack_packet ack;

    int expected_seq = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("Receiver started...\n");

    while (1) {
        recvfrom(sockfd, &pkt, sizeof(pkt), 0,
                 (struct sockaddr*)&cliaddr, &len);

        printf("Received Frame: Seq=%d Data=%s\n", pkt.seq_no, pkt.data);

        if (pkt.seq_no == expected_seq) {
            printf("Frame Accepted\n");
            ack.ack_no = (expected_seq + 1) % 2;
            expected_seq = ack.ack_no;
        } else {
            printf("Duplicate Frame Discarded\n");
            ack.ack_no = expected_seq;
        }

        sendto(sockfd, &ack, sizeof(ack), 0,
               (struct sockaddr*)&cliaddr, len);

        printf("ACK Sent: %d\n\n", ack.ack_no);
    }

    close(sockfd);
    return 0;
}
