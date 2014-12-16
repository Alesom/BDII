#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffend.h"

int criatabelas(void){
	int erro;
	table *t = NULL;	
	//#################CRIAÇAO DA TABELA CLIENTE########################################
	
	t = iniciaTabela("Cliente");

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}

	t = adicionaCampo(t, "CPF", 'S', 10, 1, 0, "Cliente");
	t = adicionaCampo(t, "Nome", 'S', 20, 0, 0, NULL);
	t = adicionaCampo(t, "Idade", 'I', (sizeof(int)), 0, 0, NULL);
	t = adicionaCampo(t, "Sexo", 'C', (sizeof(char)), 0, 0, NULL);

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	
	//##################################################################################
	//#################CRIAÇAO DA TABELA PRIVILEGIOS####################################
	
	t = iniciaTabela("Privilegios");

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}

	t = adicionaCampo(t, "ID", 'S', 5, 1, 0, "Privilegios");
	t = adicionaCampo(t, "Descr", 'S', 20, 0, 0, NULL);

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	//##################################################################################
	//#################CRIAÇAO DA TABELA CLIPRI#########################################
	
	t = iniciaTabela("CliPri");

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}

	t = adicionaCampo(t, "CCPF", 'S', 10, 0, 1, "Cliente");
	t = adicionaCampo(t, "PID", 'S', 5, 0, 1, "Privilegios");

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	//##################################################################################
	return 0;
}

int inserirCliente(){
	int erro;
	column *c=NULL;
	printf("\nInserindo na tabela Cliente / CPF: 123, Nome: Alguem, Idade: 21, Sexo:M\n");
	c = insereValor(c, "CPF", "123");
	c = insereValor(c, "Nome", "Alguem");
	c = insereValor(c, "Idade", "21");
	c = insereValor(c, "Sexo", "M");

	erro = finalizaInsert("Cliente", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);
	
	c=NULL;

	printf("\nInserindo na tabela Cliente / CPF: 123, Nome: Outro, Idade: 21, Sexo:M\n");
	c = insereValor(c, "CPF", "123");
	c = insereValor(c, "Nome", "Outro");
	c = insereValor(c, "Idade", "21");
	c = insereValor(c, "Sexo", "M");

	erro = finalizaInsert("Cliente", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);

	c=NULL;

	printf("\nInserindo na tabela Cliente / CPF: 165, Nome: Outra, Idade: 40, Sexo:F\n");
	c = insereValor(c, "CPF", "165");
	c = insereValor(c, "Nome", "Outra");
	c = insereValor(c, "Idade", "40");
	c = insereValor(c, "Sexo", "F");

	erro = finalizaInsert("Cliente", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);

	c=NULL;

	printf("\nInserindo na tabela Cliente / CPF: 165, Nome: Outra, Idade: 40, Sexo:F\n");
	c = insereValor(c, "CPF", "165");
	c = insereValor(c, "Nome", "Outra");
	c = insereValor(c, "Idade", "40");
	c = insereValor(c, "Sexo", "F");

	erro = finalizaInsert("Cliente", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);
		
	return 0;
}

int inserirPrivi(){
	int erro;
	column *c=NULL;
	printf("\nInserindo na tabela Privilegios / ID: 11, Descr: nada\n");
	c = insereValor(c, "ID", "11");
	c = insereValor(c, "Descr", "nada");

	erro = finalizaInsert("Privilegios", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);

	c=NULL;
	printf("\nInserindo na tabela Privilegios / ID: 22, Descr: tudo\n");
	c = insereValor(c, "ID", "22");
	c = insereValor(c, "Descr", "tudo");

	erro = finalizaInsert("Privilegios", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);
	return 0;
}

int inserirCliPri(){
	int erro;
	column *c=NULL;
	
	printf("\nInserindo na tabela CliPri / CCPF: 123, PID: 22\n");
	c = insereValor(c, "CCPF", "123");
	c = insereValor(c, "PID", "22");

	erro = finalizaInsert("CliPri", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);
	
	c=NULL;
	
	printf("\nInserindo na tabela CliPri / CCPF: 323, PID: 22\n");
	c = insereValor(c, "CCPF", "323");
	c = insereValor(c, "PID", "22");

	erro = finalizaInsert("CliPri", c);
	
	if(erro == SUCCESS)
		printf("Tupla Inserida\n");
	else
		printf("Erro ao inserir Tupla : %d\n",erro);
	return 0;
}
int testeAbrirTabela(){
	
	printf("\nTESTE DA FUNCAO ABRIR TABELA\n");
	tp_Object_Schema Object_Schema = AbrirTabela("CliPri");
	
	printf("Nome da Tabela: %s\n",Object_Schema.nome);
	printf("Cod da Tabela: %d\n",Object_Schema.cod);
	printf("Nome do Arquivo da Tabela: %s\n",Object_Schema.nArquivo);
	printf("Quantidade de Campos da Tabela: %d\n",Object_Schema.qtdCampos);
	
	int x=0;
	while (x < Object_Schema.qtdCampos){
		
		printf("\nNome do Campo: %s\n",Object_Schema.Campos[x].nome);
		printf("Tipo do Campo: %c\n",Object_Schema.Campos[x].tipo);
		printf("Tamanho do Campo: %d\n",Object_Schema.Campos[x].tam);
		printf("PK Campo: %d\n",Object_Schema.Campos[x].pk);
		printf("FK Campo: %d\n",Object_Schema.Campos[x].fk);
		printf("FK do Campo: %s\n",Object_Schema.Campos[x].ref);
		
		x++;
	}
	return 0;
}
int testeExcluir(){
	int teste;
	printf("\nExcluindo tabela Cliente\n");
	teste = dropTable("Cliente");
	if(teste != 0)
	printf("erro ao excluir tabela %d\n",teste);
	else
	printf("tabela excluida com sucesso!\n");
	
	printf("\nExcluindo tabela CliPri\n");
	teste = dropTable("CliPri");
	if(teste != 0)
	printf("erro ao excluir tabela %d\n",teste);
	else
	printf("tabela excluida com sucesso!\n");
	
	return 0;
}
int main(){
	system("rm -f *.dat");// exclui todos arquivos .dat para nao ter problemas na execução
	
	criatabelas();// cria as tabelas para devidos testes

	printf("\nTESTANDO INSERCOES");
	
	inserirCliente(); //  insere dados
	inserirPrivi();   //  nas tabelas
	inserirCliPri();  //  criadas
	
	testeAbrirTabela(); // testa a funçao criada que retorna as tabelas com seus campos e dados
	
	printf("\nTESTE EXCLUIR TABELA");
	
	testeExcluir(); // tenta excluir uma tabela com chave e uma sem chave
	
	printf("\n");
	printf("Pressione enter para Sair\n");
	getchar();
	system("rm -f *.dat");
	return 0;
}
