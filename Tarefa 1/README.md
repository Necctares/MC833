# Q1
a - ifconfig será utilizado para exibir informações sobre uma interface, para listar todas as interfaces podemos utilizar o comando "ifconfig -a", para listar uma interface especifica, podemos utilizar o comando "ifconfig <!nome da interface!>"
b - end. ip: 143.106.16.49  broadcast: 143.106.16.63
c - [...]
# Q2
a - 143.106.143.186
b - Non-authoritative. Uma resposta autoritaria significa que a resposta pela requisição veio de um nameserver que está na lista oficial do dominio que eu estava procurando (www.unicamp.br). Já uma resposta não autoritaria significa que a resposta obtida veio de um nameserver terceiro, que não é oficial, ou seja, não está na lista oficial de nameserver do dominio que foi procurado, o que pode ser um cache e estar desatualizado.
c - nslookup -type=soa www.unicamp.br -> Olhar em "origin =" -> Resposta: "ns1.unicamp.br" (EM DUVIDA)
d - nslookup -type=soa unicamp.br -> Obtemos a partir de origin = "ns1.unicamp.br" o servidor autoritario, após isso, utilizamos novamente: nslookup www.unicamp.br ns1.unicamp.br -> Resposta: 143.106.143.186

# Q3
