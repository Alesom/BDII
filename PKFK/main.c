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

int main(){
	
	criatabelas();
	int i;
	struct fs_objects objeto = leObjeto("CliPri");
	tp_table *esquema = leSchema(objeto);
	
	for (i=0;i<objeto.qtdCampos;i++){
		printf("%s\n", esquema[i].nome);
		printf("%c\n", esquema[i].tipo);
	}
	
	if (esquema==ERRO_ABRIR_ESQUEMA)
		return ERRO_O_VALOR_NAO_PODE_SER_INSERIDO;
	
	
	
	
	inserirCliente();
	inserirPrivi();
	inserirCliPri();
	
	printf("\n");
	printf("Pressione uma tecla para Sair\n");
	getchar();
	system("rm -f *.dat");
	return 0;
}
