Universidade Federal da Fronteira Sul

Bando de Dados II

Professor: Dênio Duarte

Alunos: Alesom Zorzi, Elias Fank e João Gehlen

Do Trabalho:

Nesta atividade, cada equipe terá que implementar na biblioteca do SGBD a inclusão de chave primária (PK) e chave estrangeira (FK) na criação das tabelas. As FK devem ser criticadas caso a tabela/atributo de origem ainda não existam.

Deverá ser implementada também a inclusão de tuplas que deve garantir as restrições de PK e FK, caso existirem.

Documentar o código e atualizar as instruções do código original conforme às alterações feitas.

Cuidados:

- utilizar as funções originais de criação das tabelas, acrescentando apenas a criação das chaves.

- utilizar as funções originais de a inserção das tuplas, acrescentando apenas a verificação das chaves

- atualizar o dicionário de dados.

Das Alterações:
Acrescentamos três parâmetros na funçao adicionaCampo()

Que agora ficou assim:

table *adicionaCampo(table *t,char *nomeCampo, char tipoCampo, int tamanhoCampo, int pk, int fk, char *ref);

Exemplo de chamada para chave primaria:

adicionaCampo(t, "CPF", 'S', 10, 1, 0, NULL);

Exemplo de chamada para chave estrangeira:

adicionaCampo(t, "CCPF", 'S', 10, 0, 1, "Cliente");

Acrescentamos teste para validação de tabela com chve estrangeira na funçao finalizaTabela;

Das Criações:
Foi criada a funçao validaCampos():
int ValidaCampos(char NomeDaTabela[], char *dt);
Que está sendo chamada dentro da funçao finalizaInsert()
Para validar os dados de entrada das tuplas.

Da Compilação:
gcc *.c -o main -Wall

Para testar as funções de validação foi criada a main
que pode ser usada depois da compilação com:
./main


