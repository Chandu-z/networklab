#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#define port 5000
#define MAXLINE 1000


int main()
{
    char buffer[10];
    char message[10];
    int listenfd,len;

    struct sockaddr_in servid,clieid;
    bzero(&servid,sizeof(servid));

    listenfd=socket(AF_INET, SOCK_DGRAM,0);
    servid.sin_addr.s_addr = htonl(INADDR_ANY);
    servid.sin_port = htons(port);
    servid.sin_family = AF_INET;  
    
    bind(listenfd,(struct sockaddr*)&servid,sizeof(servid));
    len=sizeof(clieid);

    for(;;)
    {
        printf("server:");
        fgets(message,sizeof(message),stdin);

        int l_clinet=sizeof(clieid);
        int n=recvfrom(listenfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clieid,&l_clinet);
        buffer[n]='\0';
        puts(buffer);

        sendto(listenfd,message,strlen(message),0,(struct sockaddr*)&clieid,sizeof(clieid));
        
        if(strcmp(buffer,"end"))
        {
            printf("connection closed");
            return 1;
        }

    }

}