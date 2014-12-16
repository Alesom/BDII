Universidade Federal da Fronteira Sul

Bando de Dados II

Professor: Dênio Duarte

Alunos: Alesom Zorzi, Elias Fank e João Gehlen

DO TRABALHO:

- Ao criar uma tabela, ter a possibilidade de definir chaves primárias e estrangeiras.

- Fazer uma funcionalidade para abrir uma tabela para trabalhar (uma função que englobe leObjeto e leSchema)

- Fazer uma funcionalidade que exclua uma tabela (um drop table). A tabela deve ser excluída do disco e do dicionário.

- Atualizar o txt com as instruções e comentar o código para facilitar as futuras manutenções.

DAS ALTERAÇÕES:

-Acrescentamos três parâmetros na funçao adicionaCampo()

Que agora ficou assim:

table *adicionaCampo(table *t,char *nomeCampo, char tipoCampo, int tamanhoCampo, int pk, int fk, char *ref);

Exemplo de chamada para chave primaria:

adicionaCampo(t, "CPF", 'S', 10, 1, 0, NULL);

Exemplo de chamada para chave estrangeira:

adicionaCampo(t, "CCPF", 'S', 10, 0, 1, "Cliente");

-Acrescentamos teste para validação de tabela com chve estrangeira na funçao finalizaTabela;

DAS CRIAÇÕES:

-Foi criada a funçao validaCampos():

int ValidaCampos(char NomeDaTabela[], char *dt, int flag);

Que está sendo chamada dentro da funçao finalizaInsert()

Para validar os dados de entrada das tuplas.

-Foi criada a função AbrirTabela();

tp_Object_Schema AbrirTabela(char * nomeTabela);

Pode ser chamada para criar uma estrutura que armazena os dados:

-nome da tabela

-cod da tabela

-nome do arquivo da tabela

-quantidade de campos da tabela

-lista com dados dos campos da tabela:

	-nome do campo

	-tipo do campo

	-tamanho do campo

	-informaçoes sobre PK e FK	
	
-Foi criada a funçao dropTable()

int dropTable(char * nomeTabela);

Para chamar somente passar o nome da tabela que o usuário deseja excluir

Caso a tabela for excluida retorna SUCCESS, ao contrario provavelmente a tabela deve ser "pai" de outra

Entao primeiro deve-se excluir a tabela "filho"

para auxiliar na funçao dropTable foram criadas mais 3 funçoes:

int validaatualizacao(char *nTabela);//verifica se é possivel excluir

int atualizaObjeto(char *nTabela);// exclui a tabela do dicionário

int atualizaSchema(int codigo);// exclui a tabela do esquema

DA COMPILAÇÃO:

gcc *.c -o main -Wall

Para testar as funções de validação foi criada a main
que pode ser usada depois da compilação com:
./main


