#include "netfunctions.h"
#include "chat.h"

int initializeServer(char *port)
{
    int listenfd;
    struct sockaddr_in servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    char *localIPaddress = getLocalIPaddress();
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port));
    servaddr.sin_addr.s_addr = inet_addr(localIPaddress);

    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    getsockname(listenfd, (struct sockaddr *)&myaddr, &len);
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
    // printf("Sending query to client IP :: %s and PORT :: %u at time :: %.24s :: with the command: %s\n", clientIP, clientPort, ctime(&servtime), servMsg);
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

void sig_chld(int signal)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("child %d terminated\n", pid);
    }
    return;
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
    signal(SIGCHLD, sig_chld);
    for (;;)
    {
        // sleep(10); Sleep adicionado para questão 2 - Tarefa 3
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);

        if ((pid = fork()) == 0)
        {
            close(listenfd);

            ticks = time(NULL);

            // Utilizamos getpeername para obter as informações do socket remoto
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
            {
                perror("fopen");
                exit(1);
            }

            inet_ntop(AF_INET, &(connAddr.sin_addr.s_addr), connAddrIP, INET_ADDRSTRLEN);
            // Print para questão 3
            printf("CONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            sprintf(clientInfo, "CONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            fputs(clientInfo, serverLog);

            queryMsg(connfd, saveMessage, cmdMessage, connAddrIP, ntohs(connAddr.sin_port));
            fclose(saveMessage);
            fclose(cmdMessage);

            // sleep(5); Sleep adicionado para testar multiplas conexões
            write(connfd, "bye\n", strlen("bye\n"));
            close(connfd);
            memset(clientInfo, 0, sizeof(clientInfo));
            // Print para questão 3
            printf("DISCONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            sprintf(clientInfo, "DISCONNECTED -> Remote Socket from client IP :: %s and PORT:: %u at time :: %.24s\n", connAddrIP, ntohs(connAddr.sin_port), ctime(&ticks));
            fputs(clientInfo, serverLog);
            fclose(serverLog);
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
