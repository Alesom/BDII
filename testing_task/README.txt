Universidade Federal da Fronteira Sul
Bando de Dados II
Alunos: Alesom Zorzi, Elias Fank e João Gehlen

Do trabalho:
O trabalho foi desenvolvido para a disciplina de Banco de Dados II, com o intuito de Criar um Banco de dados usando as bibliotecas já desenvolvidas no semestre passado.
O trabalho, além a função main.c, tem também um outro arquivo com o nome de ERROS.txt, que consiste no relatório de erros encontrado no trabalho anterior
O arquivo foi feito e totalmente em linux. O uso em outros sistemas operacionais não é recomendado.


Da compilação e execução:
Compilar gcc *.c -o main -Wall
formato de chamada ./main -t <nome_tabela> <atributo> <tipo> <tamanho> ...
exemplo de chamada: ./main -t pessoa nome S 20 idade I 2 -t endereco rua S 20 numero S 20

No exemplo será criado 2 tabelas: pessoa, que tem como atributos nome e idade e endereco, que tem como atributos rua e numero.
Tipos existentes : char (C) , int (I), double (D), string (S);
Esse processo deve ser feito na primeira vez que o programa é executado, para que as tableas sejam criadas. Depois disso a execução pode ser feita apenas com ./main  ;


P.s.: Lembre-se de clonar o repositório com as devidas permições

Enunciado do Trabalho:

Esta atividade consiste em clonar o repositório Git dado e testar o código para encontrar bugs. A partir de agora, serão formadas equipes de trabalho. Cada equipe poderá ter no mínimo 2 e no máximo 3 integrantes.

Atividade:

    Criar um programa que utilize as bibliotecas. O programa deverá criar um banco de dados (pelo menos quatro tabelas), inserir dados no banco criado e recuperar as tuplas das tabelas.
    Conselho: façam o programa o mais flexível possível para que seja possível criar as tuplas em tempo de execução (o banco de dados pode ser estático - criado fixo dentro do programa).

O que vai ser avaliado:

    a flexibilidade do programa criado.
    o número de bugs encontrados no código atual.

O que vai ser entregue:

    um arquivo txt com o endereço do git para ser clonado (no diretório git deve ter o relatório de bugs com os dados dos mesmos: descrição do problema, função que gerou, forma que foi gerado, entre outros).

    ao clonar, deve ser baixado o programa principal mais as bibliotecas atuais, além do relatório de bugs.

    o nome do txt deve ter as duas primeiras letras do primeiro nome de cada membro da equipe. Por exemplo, se a equipe for composta por John, Kevin e Richard, o nome será jokeri.c. Caso a equipe seja composta por Angelina e Brad, o nome será anbr.c.


