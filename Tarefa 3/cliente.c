#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define LISTENQ 10
#define MAXCLIREAD 255
#define MAXLINE 4096

int startConnection(char *ipAddress, char *port)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port));

    if (inet_pton(AF_INET, ipAddress, &servaddr.sin_addr) <= 0)
    {
        perror("inet_pton error");
        exit(1);
    }

    printf("Establishing communication...\n");
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }

    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    if (getsockname(sockfd, (struct sockaddr *)&myaddr, &len) == -1)
        perror("getsockname");
    char myaddrIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(myaddr.sin_addr.s_addr), myaddrIP, INET_ADDRSTRLEN);
    printf("...Connection succesfull\n");
    printf("Socket Local IP :: %s and PORT:: %u\n", myaddrIP, ntohs(myaddr.sin_port));

    if (getpeername(sockfd, (struct sockaddr *)&myaddr, &len) == -1)
        perror("getpeername");
    inet_ntop(AF_INET, &(myaddr.sin_addr.s_addr), myaddrIP, INET_ADDRSTRLEN);
    printf("Socket Server IP :: %s and PORT:: %u\n", myaddrIP, ntohs(myaddr.sin_port));

    return sockfd;
}

char* readMsg(int connfd)
{
    char* recvline = (char*) malloc((MAXCLIREAD + 1) * sizeof(char));
    int n;
    memset(recvline, 0, sizeof(*recvline));
    n = read(connfd, recvline, MAXCLIREAD);
    if (n < 0)
    {
        perror("read error");
        exit(1);
    }
    return recvline;
}

int sendMsg(int connfd, char* msg){
    write(connfd, msg, strlen(msg));
    return 0;
}

int main(int argc, char **argv)
{
    int sockfd;
    char* recvline = NULL;
    char error[MAXLINE + 1];

    if (argc != 3)
    {
        strcpy(error, "uso: ");
        strcat(error, argv[0]);
        strcat(error, " <IPaddress> <Port>");
        perror(error);
        exit(1);
    }

    sockfd = startConnection(argv[1], argv[2]);

    do{
        if (recvline != NULL)
            free(recvline);
        recvline = readMsg(sockfd);
        sendMsg(sockfd, recvline);
    }while(strcmp(recvline, "exit\n") != 0);

    return 0;
}
