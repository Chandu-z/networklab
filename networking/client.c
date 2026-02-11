#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    struct sockaddr_in client,server;
    int s,n,sock;

    char b1[10]="",b2[10]="hello";
    if((s=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("socket error");
        exit(0);

    }

    server.sin_family=AF_INET;
    server.sin_port=2000;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    connect(s,(struct sockaddr*)&server,sizeof server);

    for(;;)
    {
        printf("\nClient: ");
        fgets(b2,sizeof b2,stdin);
        send(s,b2,sizeof b2,0);
        if(strcmp(b2,"end")==0)
        {
            break;
        }
        recv(s,b1,sizeof b1,0);
        printf("\nserver: %s",b1);
    }

    close(s);

    }