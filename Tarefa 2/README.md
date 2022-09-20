# Q1

Arquivo "cliente.c":
Primeiro o programa checa se foi feita corretamente sua chamada passando 2 parametros via linha de comando (Nome do programa + endereço). Após isso, é criado um socket com a função socket, que recebe 3 parametros: dominio, que no caso foi escolhido AF_INET, o tipo de socket, que foi escolhido o SOCK_STREAM e o Protocolo, que no caso foi passado o 0, que siginifica que será escolhido automaticamente. Esta configuração de valores cria um socket TCP. Após sua chamada, existe uma conferencia se houve ou não erros para esse novo socket, caso retorne algum erro (valor -1) o programa será encerrado com a mensagem de erro.
Na linha 34, é chamada a função "bzero", que serve para apagar os n bytes de memoria que começam na posição apontada por s: bzero(void *s, size_t n), onde no programa, é passado a posição inicial da struct "servaddr" (que é um sockaddr_in, uma struct que descreve um internet socket address) e o seu tamanho total, sendo assim, a função irá apagar todo o dado contido nessa struct.
Após a devida inicialização do servaddr, é passado os valores AF_INET (Familia de endereços IPv4) para seu "sin_family" (familia de endereço) e htons(13) (Função que faz a inversão de ordem de bytes lidos do padrão do host para o padrão da rede) para seu "sin_port" (valor da Porta já no padrão de ordem de bytes lida pela rede)
Na linha 37, é chamada a função "inet_pton", que serve para converter endereços "src" da familia de endereços "af" (IPv4 ou IPv6) de textos para seu "dst" final em forma binaria: inet_pton(int af, const char *restict src, void *restrict dst), no programa, é passado o "af" AF_INET, o "src" passado por argumento de linha de comando pelo usuario que será um texto de endereço para ser convertido e passado para o "dst" que será armazenado no "sin_addr" (internet address) da struct servaddr.
Na linha 42, é chamada a função "connect", que serve para abrir uma conexão no socket "fd" para o par no endereço "addr" com tamanho "len": connect(int fd, const struct sockaddr addr, socklen_t len), que no programa é passado "fd" sockfd (criado no inicio do programa), "addr" servaddr e "len" tamanho de servaddr, com isso, é checado se a função retorna 0 (Tudo certo) ou -1 (erro na conexão).
Na linha 47, é chamada a função "read", que serve para ler até "count" bytes do socket "fd" começando na posição do buffer "buf": read(int fd, void *buf, size_t count), que no programa, é passado o "fd" sockfd, "buf" recvline (vetor de char inicializado no inicio do programa) e "count" MAXLINE (constante definida no inicio do arquivo). A cada vez que é lido do socket, é checado se houve erro ou chegou ao EOF para encerrar o programa.
Na linha 49, é chamada a função "fputs" toda vez que é feito uma nova leitura no socket via função read, que serve para escrever uma string para um Stream, que no caso, foi passado o vetor de caracteres recvline (leitura do socket) para o stdout.

Arquivo "servidor.c":
Na linha 22, é criado um socket utilizando a mesma logica já explicada na parte do cliente para a função "socket".
Na linha 27, é utilizado também a mesma logica da parte do cliente, para zerar os dados (possivel lixo) que tenha na variavel declarada servaddr e nas linhas 28 a 30 segue a mesma logica também do arquivo cliente.c, com uma adição, que é passado para o "s_addr" da struct com o endereço INADDR_ANY (0), que quando for utilizado (na linha 32) a função "bind" irá vincular o socket a todas as interfaces disponiveis.
Após a vinculação, na linha 37 é chamada a função "listen", em que é feita a preparação para estabelecer conexões no socket passado como parametro (listenfd) e o numero limite de requisições de conexão que serão postas em fila (LISTENQ = 10), passando desse numero, os outros pedidos serão rejeitados.
Por fim, depois de toda a preparação, o programa entra em um laço e só é encerrado se a função "accept" retornar um erro. Ao chamar a função "accept" o programa coloca o file descriptor socketfd para ficar esperando por uma conexão que será pega da fila, ao aceitar um pedido de conexão, "accept" retorna um novo file descriptor (que não está em estado de listen) alocado para a variavel "connfd". É utilizado a função "snprintf" para escrever no buf uma mensagem contendo o horario atual na máquina do servidor. Após isso, é utilizada a função "write" para escrever a mensagem do buf no file descriptor connfd.

# Q3

Ao executar o programa do servidor, tivemos um erro: "bind: permission denied". O problema é causado porque a porta escolhida em "sin_port" é a 13, que é reservada como uma porta privilegiada (portas até a 1024 não inclusive). Existem duas opções: executar o programa como root ou modificar a porta para uma porta acima da 1023, que pode ser utilizada por qualquer um. Optei por alterar a porta para a porta 22350, nos arquivos cliente.c e servidor.c

# Q4

Fizemos algumas alterações no arquivo servidor.c e cliente.c (ver comentarios no arquivo)

# Q5

# Q8

Send(), sendto(), sendmsg() -> Write msg
recv(), recvfrom() -> Read msg
