#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
// Includes adicionados posteriormente
#include <ifaddrs.h>
#include <arpa/inet.h>

#define FILE_MODE "a"
#define LISTENQ 10
#define MAXCLIREAD 255
#define MAXLINE 4096

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
    if (getifaddrs(&actualIP) == -1)
    {
        perror("getifaddrs");
        exit(1);
    }

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

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    char *localIPaddress = getLocalIPaddress();
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port));
    servaddr.sin_addr.s_addr = inet_addr(localIPaddress);

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
    printf("Server started with IP :: %s and PORT:: %u\n\n", localIPaddress, ntohs(myaddr.sin_port));

    free(localIPaddress);
    return listenfd;
}

int readMsg(int connfd, FILE *stream, char* clientIP, uint16_t clientPort)
{
    char recvline[MAXCLIREAD + 1];
    char clientInfo[80];
    sprintf(clientInfo, "Query Result from client IP :: %s and PORT :: %u\n", clientIP, clientPort);
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

char *queryMsg(int connfd, FILE *stream, char* clientIP, uint16_t clientPort)
{
    char *servMsg = (char *)malloc((MAXCLIREAD + 1) * sizeof(char));
    fflush(stdout);
    fgets(servMsg, sizeof(servMsg), stdin);
    write(connfd, servMsg, strlen(servMsg));
    readMsg(connfd, stream, clientIP, clientPort);
    return servMsg;
}

int runServer(char *port)
{
    int connfd, listenfd;
    time_t ticks;
    struct sockaddr_in connAddr;
    socklen_t connLen = sizeof(connAddr);
    char connAddrIP[INET_ADDRSTRLEN];
    char *query = NULL;

    listenfd = initializeServer(port);

    for (;;)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            perror("accept");
            exit(1);
        }

        ticks = time(NULL);

        // Utilizamos getpeername para obter as informações do socket remoto
        if (getpeername(connfd, (struct sockaddr *)&connAddr, &connLen) == -1)
        {
            perror("getpeername");
            exit(1);
        }

        inet_ntop(AF_INET, &(connAddr.sin_addr.s_addr), connAddrIP, INET_ADDRSTRLEN);
        printf("CONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));

        FILE *f;
        f = fopen("queryResult.txt", FILE_MODE);
        if (f == NULL)
        {
            perror("fopen");
            exit(1);
        }
        do
        {
            if (query != NULL)
                free(query);
            printf("Send a command to the client (Max %d characters, type 'exit' to end):\n", MAXCLIREAD);
            query = queryMsg(connfd, f, connAddrIP, ntohs(connAddr.sin_port));
        } while (strcmp(query, "exit\n") != 0);
        fclose(f);

        close(connfd);
        printf("DISCONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
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
