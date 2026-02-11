#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT 5000
#define MAXLINE 1000


int main()
{
    char buffer[10];
    struct sockaddr_in servid;
    char *message="hello server";
    int listenfd,n;

    bzero(&servid,sizeof(servid));
    servid.sin_addr.s_addr=inet_addr("127.0.0.1");
    servid.sin_port=htons(PORT);
    servid.sin_family=AF_INET;

    listenfd=socket(AF_INET,SOCK_DGRAM,0);

    if(connect(listenfd,(struct sockaddr*)&servid,sizeof(servid))<0)
    {
        perror("connection erro!!");
        exit(0);
    }

    for(;;)
    {

    sendto(listenfd,message,MAXLINE,0,(struct sockaddr*)NULL,sizeof(servid));

    recvfrom(listenfd,buffer,sizeof(buffer),0,(struct sockaddr*)NULL,NULL);
    puts(buffer);

    if(strcmp(buffer,"end"))
    {
        perror("closed");
        return 1; 
    }
    }

    close(listenfd);

    

}