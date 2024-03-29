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
//Includes adicionados posteriormente
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <string.h>

#define LISTENQ 10
#define MAXDATASIZE 100
// Questão 7 -> Define o tamanho maximo de caracteres lidos da mensagem do cliente.
#define MAXCLIREAD 255

int main (int argc, char **argv) {
    int    listenfd, connfd;
    struct sockaddr_in servaddr;
    char   buf[MAXDATASIZE];
    time_t ticks;


    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    /** Questão 3 -> Modificado de htons(13) -> porta privilegiada
                                   para htons(22350) -> porta 'comum'
    
    servaddr.sin_port = htons(22350); (Questão 3)
    */
    /** Questão 4 -> Modificado de htons(22350) -> porta fixa
     *                             para htons(0) -> para porta automatica
     */
    servaddr.sin_port = htons(0);

    // Questão 4 -> Pegar o IP Local da interface definida como "default" -> Passa esse endereço para a variavel host
    FILE *f;
    char line[100] , *p , *c;
    
    f = fopen("/proc/net/route" , "r");
    
    while(fgets(line , 100 , f))
    {
		p = strtok(line , " \t");
		c = strtok(NULL , " \t");
		
		if(p!=NULL && c!=NULL)
		{
			if(strcmp(c , "00000000") == 0)
			{
				break;
			}
		}
	}

    struct ifaddrs *actualIP;
    if (getifaddrs(&actualIP) == -1) {
        perror("getifaddrs");
        exit(1);
    }

    int family;
    char host[NI_MAXHOST];
    for (struct ifaddrs *ifa = actualIP; ifa != NULL; ifa = ifa->ifa_next){
        family = ifa->ifa_addr->sa_family;
        if(family == AF_INET && strcmp(ifa->ifa_name, p) == 0){
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            break;
        }
    }

    /* Questão 4 -> Modificado servaddr.sin_addr.s_addr = htonl(INADDR_ANY)
    *               para servaddr.sin_addr.s_addr = htonl(host)
    */
    servaddr.sin_addr.s_addr = inet_addr(host);
    // --------------------------------------------------------------------

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    
    if (listen(listenfd, LISTENQ) == -1) {
        perror("listen");
        exit(1);
    }

    /* Questão 4
    * Utiliza o getsockname para obter a porta escolhida automaticamente pelo sistema.
    */
    struct sockaddr_in myaddr;
    socklen_t len = sizeof(myaddr);
    if(getsockname(listenfd, (struct sockaddr *) &myaddr, &len) == -1)
        perror("getsockname");
    printf("Server started with IP :: %s and PORT:: %u\n\n", host, ntohs(myaddr.sin_port));
    // ----------------------------------------------------------------------------------

    // Questão 6 -> Inicializa variaveis para serem usadas no getpeername
    struct sockaddr_in connAddr;
    socklen_t connLen = sizeof(connAddr);
    char connAddrIP[INET_ADDRSTRLEN];
    // ------------------------------------------------------------------
    // Questão 7 -> Inicializa buffer de leitura da mensagem do cliente
    char recvline[MAXCLIREAD + 1];
    int n;
    // ------------------------------------------------------------------
    for ( ; ; ) {
        if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
        perror("accept");
        exit(1);
        }

        // Questão 6 -> Utilizamos getpeername para obter as informações do socket remoto
        if (getpeername(connfd, (struct sockaddr *) &connAddr, &connLen) == -1){
            perror("getpeername");
            exit(1);
        }
        inet_ntop(AF_INET, &(connAddr.sin_addr.s_addr), connAddrIP, INET_ADDRSTRLEN);
        printf("Questao 6 -> Socket Remoto Cliente IP :: %s and PORT:: %u\n", connAddrIP, ntohs(connAddr.sin_port));
        // --------------------------------------------------------------------------------------------

        // Questão 7 -> Lê a mensagem enviada pelo cliente
        char msg[MAXCLIREAD + 14] = "Questao 7 -> ";
        memset(recvline,0,sizeof(recvline));
        n = read(connfd, recvline, MAXCLIREAD);
        if (fputs(strcat(msg, recvline), stdout) == EOF) {
            perror("fputs error");
            exit(1);
        }
        if (n < 0) {
            perror("read error");
            exit(1);
        }
        // ---------------------------------------------------

        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "Hello from server!\nTime: %.24s\r\n", ctime(&ticks));
        write(connfd, buf, strlen(buf));

        close(connfd);
        printf("\n----------------- Aguardando proxima conexao -------------------------------\n\n");
    }
    return(0);
}
