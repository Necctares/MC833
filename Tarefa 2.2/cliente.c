<<<<<<< HEAD
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
#define MAXRESPONSE 8192

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
=======
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#define LISTENQ 10
#define MAXCLIREAD 8192
#define MAXLINE 4096
#define MAXRESPONSE 8192

int Getifaddrs(struct ifaddrs **_ifaddrs)
{
    int result = getifaddrs(_ifaddrs);
    if (result == -1)
    {
        perror("getifaddrs");
        exit(1);
    }
    return result;
}

int Socket(int domain, int type, int protocol)
{
    int socketfd = socket(domain, type, protocol);
    if (socketfd == -1)
    {
        perror("socket");
        exit(1);
    }
    return socketfd;
}

int Bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    int result = bind(fd, addr, len);
    if (result == -1)
    {
        perror("bind");
        exit(1);
    }
    return result;
}

int Listen(int fd, int n)
{
    int result = listen(fd, n);
    if (result == -1)
    {
        perror("listen");
        exit(1);
    }
    return result;
}

int Getsockname(int fd, struct sockaddr *addr, socklen_t *len)
{
    int result = getsockname(fd, addr, len);
    if (result == -1)
        perror("getsockname");
    return result;
}

int Accept(int fd, struct sockaddr *addr, socklen_t *len)
{
    int connfd = accept(fd, addr, len);
    if (connfd == -1)
    {
        perror("accept");
        exit(1);
    }
    return connfd;
}

int Getpeername(int fd, struct sockaddr *addr, socklen_t *len)
{
    int result = getpeername(fd, addr, len);
    if (result == -1)
    {
        perror("getpeername");
        exit(1);
    }
    return result;
}

int Inet_pton(int af, const char *cp, void *buf)
{
    int result = inet_pton(af, cp, buf);
    if (result <= 0)
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
    {
        perror("inet_pton error");
        exit(1);
    }
<<<<<<< HEAD

    printf("Establishing communication...\n");
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
=======
    return result;
}

int Connect(int fd, const struct sockaddr *addr, socklen_t len){
    int result = connect(fd, addr, len);
    if (result < 0)
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
    {
        perror("connect error");
        exit(1);
    }
<<<<<<< HEAD

    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    if (getsockname(sockfd, (struct sockaddr *)&myaddr, &len) == -1)
        perror("getsockname");
=======
    return result;
}

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
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
    char myaddrIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(myaddr.sin_addr.s_addr), myaddrIP, INET_ADDRSTRLEN);
    printf("...Connection succesfull\n");
    printf("Socket Local IP :: %s and PORT:: %u\n", myaddrIP, ntohs(myaddr.sin_port));

<<<<<<< HEAD
    if (getpeername(sockfd, (struct sockaddr *)&myaddr, &len) == -1)
        perror("getpeername");
=======
    Getpeername(sockfd, (struct sockaddr *)&myaddr, &len);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
    inet_ntop(AF_INET, &(myaddr.sin_addr.s_addr), myaddrIP, INET_ADDRSTRLEN);
    printf("Socket Server IP :: %s and PORT:: %u\n", myaddrIP, ntohs(myaddr.sin_port));

    return sockfd;
}

<<<<<<< HEAD
char *readMsg(int connfd)
{
    char *recvline = (char *)malloc((MAXCLIREAD + 1) * sizeof(char));
    int n;
    memset(recvline, 0, sizeof(*recvline));
    n = read(connfd, recvline, MAXCLIREAD);
=======
int inversePrint(char *word)
{
    for (int i = strlen(word) - 1; i >= 0; i--)
    {
        printf("%c", toupper(word[i]));
    }
    printf("\n");
    return 0;
}

char *readMsg(int connfd)
{
    char *recvline = (char *)calloc((MAXCLIREAD + 1), sizeof(char));
    int n;
    n = read(connfd, recvline, MAXCLIREAD);
    inversePrint(recvline);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
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
<<<<<<< HEAD
        write(connfd, "EXIT\n", sizeof(char) * 6);
=======
        write(connfd, "EXIT\n", strlen("EXIT\n"));
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
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
<<<<<<< HEAD
            write(connfd, "Failed to run command\n", sizeof(char) * 23);
=======
            write(connfd, "Failed to run command\n", strlen("Failed to run command\n"));
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
            perror("Failed to run command");
        }
        else
        {
            while (fgets(output, sizeof(output), fp) != NULL)
            {
<<<<<<< HEAD
                strncat(response, output, sizeof(response));
            }
            if (strlen(response) > 0)
            {
                write(connfd, response, sizeof(response));
            }
            else
            {
                write(connfd, "Failed to run command\n", sizeof(char) * 23);
=======
                strncat(response, output, strlen(output));
            }
            if (strlen(response) > 0)
            {
                write(connfd, response, strlen(response));
            }
            else
            {
                write(connfd, "Failed to run command\n", strlen("Failed to run command\n"));
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
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
