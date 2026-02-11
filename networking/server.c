#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int isPalindrome(int n) {
    int temp = n, digit, rev = 0;
    while (n > 0) {
        digit = n % 10;
        rev = rev * 10 + digit;
        n = n / 10;
    }
    return (rev == temp);
}

int main() {
    struct sockaddr_in client, server;
    int s, sock, n;
    char buffer[1024];

    // Create socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(0);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(2000); // network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&(server.sin_zero), 0, 8); // zero the rest

    // Bind
    if (bind(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("bind error");
        exit(0);
    }

    // Listen
    listen(s, 1);
    n = sizeof(client);

    // Accept connection
    sock = accept(s, (struct sockaddr*)&client, &n);
    if (sock < 0) {
        perror("accept error");
        exit(0);
    }

    printf("Client connected!\n");

    for (;;) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0'; // null terminate
        if (strcmp(buffer, "pali") == 0) {
            char *msg = "Enter a number: ";
            send(sock, msg, strlen(msg), 0);

            memset(buffer, 0, sizeof(buffer));
            bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) break;
            buffer[bytes] = '\0';

            int num = atoi(buffer);
            if (isPalindrome(num)) {
                send(sock, "Palindrome\n", 11, 0);
            } else {
                send(sock, "Not a Palindrome\n", 17, 0);
            }
        } else if (strcmp(buffer, "end") == 0) {
            break;
        } else {
            printf("Client: %s\n", buffer);
            char response[1024];
            printf("Server: ");
            fgets(response, sizeof(response), stdin);
            send(sock, response, strlen(response), 0);
            if (strncmp(response, "end", 3) == 0) break;
        }
    }

    close(sock);
    close(s);
    printf("Server closed.\n");
    return 0;
}

