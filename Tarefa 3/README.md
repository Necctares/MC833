## Questão 1

- O backlog é responsavel por determinar o numero de conexões pendentes que a fila irá manter, sendo que, quando temos multiplas conexões, o servidor irá processar essas conexões na fila, de modo que o tamanho do backlog irá determinar quantas conexões ativas + conexões incompletas poderão estar na fila. Durante o TCP 3WHS, temos duas filas, uma fila para conexões incompletas (fila SYN) e fila das conexões já completas, conexões com o estado "SYN RECEIVED" são adicionadas para a fila de conexões incompletas e depois que seu estado é mudado para "ESTABLISHED" elas são movidas para a fila de conexões já completas, onde a função "accept" irá utiliza-las.

## Questão 3

- Um processo zumbi, é um processo em que sua execução já foi finalizada mas ainda esta registrado na tabela de processos do sistema esperando pelo processo pai remove-lo de lá. Eles são gerados quando o processo pai é clonado, gerando um processo filho, e o processo pai não espera pelo fim do processo filho e continua executando suas proximas tarefas, deste modo, quando o processo filho termina e envia o sinal de termino, o processo pai não lê esse sinal e não remove o processo filho da tabela de processos, deixando assim, o processo filho em estado zumbi permanentemente. Eles devem ser tratados corretamente pois, apesar de serem "dummys" e não gastar muito recurso do sistema, eles continuam ocupando espaço na tabela de processos que é finita, ou seja, se muitos processos forem gerados e não terminados, pode ser que a tabela fique cheia e então o sistema não consiga mais criar novos processos.

## Questão 5

- A principal diferença entre o wait() e o waitpid() é seu comportamento. O wait() espera o processo filho terminar para poder criar um novo processo filho, já o waitpid() tem comportamento variavel, dependendo dos parametros passados, neste caso em que usamos o "waitpid(-1, &stat, WNOHANG)", ele não irá esperar pelos processos filhos terminarem para executar novos processos, mas irá previnir que no fim, não tenha processos zumbis.
