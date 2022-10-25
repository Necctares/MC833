<<<<<<< HEAD
=======
#include <time.h>
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
<<<<<<< HEAD
#include <time.h>
#include <unistd.h>
// Includes adicionados posteriormente
=======
#include <unistd.h>
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
#include <ifaddrs.h>
#include <arpa/inet.h>

#define LISTENQ 10
#define MAXCLIREAD 8192
#define MAXLINE 4096
<<<<<<< HEAD
=======
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
    {
        perror("inet_pton error");
        exit(1);
    }
    return result;
}

int Connect(int fd, const struct sockaddr *addr, socklen_t len){
    int result = connect(fd, addr, len);
    if (result < 0)
    {
        perror("connect error");
        exit(1);
    }
    return result;
}
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76

// Pega o IP Local da interface definida como "default" -> Passa esse endereço para a variavel host
char *getLocalIPaddress()
{
    FILE *f;
    char line[100], *p, *c;

    f = fopen("/proc/net/route", "r");

    while (fgets(line, 100, f))
    {
        p = strtok(line, " \t");
        c = strtok(NULL, " \t");

        if (p != NULL && c != NULL)
        {
            if (strcmp(c, "00000000") == 0)
            {
                break;
            }
        }
    }

    struct ifaddrs *actualIP;
<<<<<<< HEAD
    if (getifaddrs(&actualIP) == -1)
    {
        perror("getifaddrs");
        exit(1);
    }
=======
    Getifaddrs(&actualIP);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76

    int family;
    char host[NI_MAXHOST];
    for (struct ifaddrs *ifa = actualIP; ifa != NULL; ifa = ifa->ifa_next)
    {
        family = ifa->ifa_addr->sa_family;
        if (family == AF_INET && strcmp(ifa->ifa_name, p) == 0)
        {
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            break;
        }
    }
    char *ipAddress = (char *)malloc(sizeof(host));
    strcpy(ipAddress, host);
    return ipAddress;
}

int initializeServer(char *port)
{
    int listenfd;
    struct sockaddr_in servaddr;

<<<<<<< HEAD
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
=======
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76

    char *localIPaddress = getLocalIPaddress();
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port));
    servaddr.sin_addr.s_addr = inet_addr(localIPaddress);

<<<<<<< HEAD
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(listenfd, LISTENQ) == -1)
    {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    if (getsockname(listenfd, (struct sockaddr *)&myaddr, &len) == -1)
        perror("getsockname");
=======
    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    getsockname(listenfd, (struct sockaddr *)&myaddr, &len);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
    printf("Server started with IP :: %s and PORT:: %u\n\n", localIPaddress, ntohs(myaddr.sin_port));

    free(localIPaddress);
    return listenfd;
}

int readMsg(int connfd, FILE *stream, char *clientIP, uint16_t clientPort, char *servMsg)
{
    char recvline[MAXCLIREAD];
    char clientInfo[255];
    time_t servtime;
    servtime = time(NULL);
    sprintf(clientInfo, "Query Result from client IP :: %s and PORT :: %u at time :: %.24s :: with the command: %s\n", clientIP, clientPort, ctime(&servtime), servMsg);
<<<<<<< HEAD
=======
    printf("Sending query to client IP :: %s and PORT :: %u at time :: %.24s :: with the command: %s\n", clientIP, clientPort, ctime(&servtime), servMsg);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
    int n;
    memset(recvline, 0, sizeof(recvline));
    n = read(connfd, recvline, MAXCLIREAD);
    if (fputs(strcat(clientInfo, recvline), stream) == EOF)
    {
        perror("fputs error");
        exit(1);
    }
    if (n < 0)
    {
        perror("read error");
        exit(1);
    }
    if (fputs("\n********** ::: END OF QUERY ::: **********\n", stream) == EOF)
    {
        perror("fputs error");
        exit(1);
    }
    return 0;
}

int queryMsg(int connfd, FILE *stream, FILE *cmd, char *clientIP, uint16_t clientPort)
{
    char servMsg[MAXCLIREAD + 1];
    do
    {
        if (fgets(servMsg, sizeof(servMsg), cmd) == NULL)
            return -1;
        write(connfd, servMsg, strlen(servMsg));
        readMsg(connfd, stream, clientIP, clientPort, servMsg);
    } while (strncmp(servMsg, "EXIT", strlen("EXIT")) != 0);
    return 0;
}

int runServer(char *port)
{
    int connfd, listenfd;
    time_t ticks;
    struct sockaddr_in connAddr;
    socklen_t connLen = sizeof(connAddr);
    char connAddrIP[INET_ADDRSTRLEN];
    pid_t pid;

    listenfd = initializeServer(port);

    for (;;)
    {
<<<<<<< HEAD
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            perror("accept");
            exit(1);
        }
=======
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76

        if ((pid = fork()) == 0)
        {
            close(listenfd);

            ticks = time(NULL);

            // Utilizamos getpeername para obter as informações do socket remoto
<<<<<<< HEAD
            if (getpeername(connfd, (struct sockaddr *)&connAddr, &connLen) == -1)
            {
                perror("getpeername");
                exit(1);
            }

            inet_ntop(AF_INET, &(connAddr.sin_addr.s_addr), connAddrIP, INET_ADDRSTRLEN);
            printf("CONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));

            FILE *saveMessage;
            FILE *cmdMessage;
            cmdMessage = fopen("commands.txt", "r");
            saveMessage = fopen("queryResult.txt", "a");
            if (saveMessage == NULL || cmdMessage == NULL)
=======
            Getpeername(connfd, (struct sockaddr *)&connAddr, &connLen);

            FILE *serverLog;
            FILE *saveMessage;
            FILE *cmdMessage;
            char clientInfo[255];
            memset(clientInfo, 0, sizeof(clientInfo));
            cmdMessage = fopen("commands.txt", "r");
            saveMessage = fopen("queryResult.txt", "a");
            serverLog = fopen("serverLog.txt", "a");
            if (saveMessage == NULL || cmdMessage == NULL || serverLog == NULL)
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
            {
                perror("fopen");
                exit(1);
            }
<<<<<<< HEAD
=======

            inet_ntop(AF_INET, &(connAddr.sin_addr.s_addr), connAddrIP, INET_ADDRSTRLEN);
            //Print para questão 3
            //printf("CONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            sprintf(clientInfo, "CONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            fputs(clientInfo, serverLog);

>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
            queryMsg(connfd, saveMessage, cmdMessage, connAddrIP, ntohs(connAddr.sin_port));
            fclose(saveMessage);
            fclose(cmdMessage);

<<<<<<< HEAD
            write(connfd, "bye\n", strlen("bye\n"));
            close(connfd);
            // sleep(10); Sleep adicionado para testar multiplas conexões
            printf("DISCONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
=======
            //sleep(30); Sleep adicionado para testar multiplas conexões
            write(connfd, "bye\n", strlen("bye\n"));
            close(connfd);
            memset(clientInfo, 0, sizeof(clientInfo));
            //Print para questão 3
            //printf("DISCONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            sprintf(clientInfo, "DISCONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            fputs(clientInfo, serverLog);
            fclose(serverLog);
>>>>>>> f5f0d63973ce0ebb59257b5c8edb9f1d150edb76
            exit(0);
        }

        // sleep(1000); Foi adicionado para a questão 2
        close(connfd);
    }
    return 0;
}

int main(int argc, char **argv)
{
    char error[MAXLINE + 1];
    if (argc != 2)
    {
        strcpy(error, "uso: ");
        strcat(error, argv[0]);
        strcat(error, "<Port>");
        perror(error);
        exit(1);
    }
    runServer(argv[1]);
    return 0;
}
