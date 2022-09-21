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

#define MAXLINE 4096
// Questão 7 -> Define tamanho maximo da mensagem enviada para o servidor
#define MAXSERVMSG 255

int main(int argc, char **argv) {
    int    sockfd, n;
    char   recvline[MAXLINE + 1];
    char   error[MAXLINE + 1];
    struct sockaddr_in servaddr;

    // Questão 4 -> Adicionado o uso do argumento port em argv[2]
    if (argc != 3) {
        strcpy(error,"uso: ");
        strcat(error,argv[0]);
        strcat(error," <IPaddress> <Port>");
        perror(error);
        exit(1);
    }
    // ----------------------------------------------------------

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    /** Questão 3 
     * Modificado de htons(13) -> porta privilegiada
                                   para htons(22350) -> porta 'comum'
    */
    servaddr.sin_port   = htons(atoi(argv[2]));
    // --------------------------------------------------------------

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }
    
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    // Questão 5 -> Obter informações do socket local
    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    if(getsockname(sockfd, (struct sockaddr *) &myaddr, &len) == -1)
        perror("getsockname");
    char myaddrIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(myaddr.sin_addr.s_addr), myaddrIP, INET_ADDRSTRLEN);
    printf("Questao 5 -> Socket Local IP :: %s and PORT:: %u\n", myaddrIP, ntohs(myaddr.sin_port));
    // --------------------------------------------------------------------------------------------

    // Questão 7 -> Pega a mensagem e a envia para o servidor
    char servMsg[MAXSERVMSG + 1];
    printf("Type a message for the server (Max 250 characters): ");
    fflush(stdout);
    fgets(servMsg, sizeof(servMsg), stdin);
    write(sockfd, servMsg, strlen(servMsg));
    // ------------------------------------------------------------

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF) {
            perror("fputs error");
            exit(1);
        }
    }

    if (n < 0) {
        perror("read error");
        exit(1);
    }

    exit(0);
}
