// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include<unistd.h>
// #include<arpa/inet.h>

// #define port 8080

// int main(){
//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     int addrlen=sizeof(address);
//     int frame;

//     server_fd=socket(AF_INET,SOCK_STREAM,0);

//     address.sin_family=AF_INET;
//     address.sin_addr.s_addr=INADDR_ANY;
//     address.sin_port=htons(port);

//     bind(server_fd,(struct sockadr *)&address,sizeof(address));
    
// }























#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd, newsock;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    int frame;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket error");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    printf("Server waiting for connection...\n");

    len = sizeof(cliaddr);
    newsock = accept(sockfd, (struct sockaddr *)&cliaddr, &len);

    printf("Client connected!\n");

    while (1)
    {
        recv(newsock, &frame, sizeof(frame), 0);

        if (frame == -1)
            break;

        printf("Received Frame: %d\n", frame);

        send(newsock, &frame, sizeof(frame), 0); // Sending ACK
        printf("ACK Sent: %d\n", frame);
    }

    close(newsock);
    close(sockfd);

    return 0;
}
