Universidade Federal da Fronteira Sul
Bando de Dados II
Alunos: Alesom Zorzi, Elias Fank e João Gehlen

Do trabalho:
O trabalho foi desenvolvido para a disciplina de Banco de Dados II, com o intuito de Criar um Banco de dados usando as bibliotecas já desenvolvidas no semestre passado.
O arquivo foi feito e totalmente em linux. O uso em outros sistemas operacionais não é recomendado.


Da compilação e execução:
Compilar gcc *.c -o main -Wall
formato de chamada ./main -t <nome_tabela> <atributo> <tipo> <tamanho> ...
exemplo de chamada: ./main -t pessoa nome S 20 idade I 2 -t endereco rua S 20 numero S 20

No exemplo será criado 2 tabelas: pessoa, que tem como atributos nome e idade e endereco, que tem como atributos rua e numero.
Tipos existentes : char (C) , int (I), double (D), string (S);
Esse processo deve ser feito na primeira vez que o programa é executado, para que as tableas sejam criadas. Depois disso a execução pode ser feita apenas com ./main  ;


P.s.: Lembre-se de clonar o repositório com as devidas permições 
