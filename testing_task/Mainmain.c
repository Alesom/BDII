/*Códigos feitos por:
 * Elias Fank
 * Alesom Zorzi
 * João Gehlen
 * */
//só compilar gcc *.c -o main -Wall
//formato de chamada ./main -t <nome_tabela> <atributo> <tipo> <tamanho> ...
//exemplo de chamada: ./main -t pessoa nome S 20 idade I 2 -t endereco rua S 20 numero S 20
//tipos existentes : char, int, double, string
//Obs: caso sair do programa, para executar novamente, chame com todos os parametros de criação de tabelas:
							//se chamou ./main -t pessoa nome S 20 idade I 2 -t endereco rua S 20 numero S 20 na primeira vez
							//chame assim na segunda e nao apenas ./main

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffend.h"

#define MAX 11234

typedef struct _atributo{
	char *nome;
	char tipo;
	int tamanho;
	struct _atributo *next;
}Atributo;

typedef struct _listaatributos{
	int quantidade;
	Atributo *first;
	Atributo *last;
}Atributos;

typedef struct _tabela{
	char *nome;
	Atributos *atributos;
	struct _tabela *next;
}Table;

typedef struct _listatabelas{
	int quantidade;
	Table *first;
	Table *last;
}TableList;

void inicializalistatabelas(TableList *tabelas){
	tabelas->quantidade=0;
	tabelas->first=NULL;
	tabelas->last=NULL;
}

void inicializalistaatributos(Atributos *atributos){
	atributos->quantidade=0;
	atributos->first=NULL;
	atributos->last=NULL;
}
void inseretabela(TableList *tabelas, Table *novatabela){
	tabelas->quantidade++;
	if(tabelas->first == NULL){
		tabelas->first = novatabela;
		tabelas->last = novatabela;
		novatabela->next = NULL;
		}else{
			tabelas->last->next = novatabela;
			tabelas->last = novatabela;
			novatabela->next = NULL;
		}
}

void insereatributo(Atributos *atributos, Atributo *novoatributo){
	atributos->quantidade++;
	if(atributos->first == NULL){
		atributos->first = novoatributo;
		atributos->last = novoatributo;
		novoatributo->next = NULL;
		}else{
			atributos->last->next = novoatributo;
			atributos->last = novoatributo;
			novoatributo->next = NULL;
		}
}

void mostrartabelascomatributos(TableList *tabelas){
	printf("\nBANCO\n");
	Table *aux;
	Atributo *auxatri;
	aux = tabelas->first;
	while(aux!=NULL){
		auxatri=aux->atributos->first;
		printf("|%s|\n", aux->nome);
		while(auxatri!=NULL){
			printf("%s(%c-%d)\n",auxatri->nome, auxatri->tipo, auxatri->tamanho);
			auxatri=auxatri->next;
		}
		aux=aux->next;
	}
}
void criartabelas(TableList *tabelas){
	int erro;
	table *t = NULL;
	Table *aux;
	Atributo *auxatri;
	aux = tabelas->first;
	while(aux!=NULL){
		auxatri=aux->atributos->first;
		t = iniciaTabela(aux->nome);
		if(t == NULL){
			return;
		}
		while(auxatri!=NULL){
			
			if(auxatri->tipo == 'C')
			t = adicionaCampo(t, auxatri->nome, auxatri->tipo, (sizeof(char)));
			if(auxatri->tipo == 'I')
			t = adicionaCampo(t, auxatri->nome, auxatri->tipo, (sizeof(int)));
			if(auxatri->tipo == 'D')
			t = adicionaCampo(t, auxatri->nome, auxatri->tipo, (sizeof(double)));
			if(auxatri->tipo == 'S')
			t = adicionaCampo(t, auxatri->nome, auxatri->tipo, auxatri->tamanho);
			
			auxatri=auxatri->next;
		}
		erro = finalizaTabela(t);
		if(erro != 0){
			printf("Erro %d: na função finalizaTabela().\n", erro);
			return;
		}
		aux=aux->next;
	}
	
}
void mostraropcoes(TableList *tabelas){
	Table *aux;
	aux = tabelas->first;
	while(aux!=NULL){
		printf("1 - |%s|\n", aux->nome);
		aux=aux->next;
	}
	return;
}

void inserirtupla(char *tabela, TableList *tabelas){
	int erro;
	column *c = NULL;
	char string[100],caracter;
	int inteiro;
	double doublevar;
	Table *aux;
	Atributo *auxatri;
	aux = tabelas->first;
	while(aux!=NULL){
		if(!strcmp(aux->nome, tabela)){
			auxatri=aux->atributos->first;
			while(auxatri!=NULL){
				printf("Insira %s(%d-%c):\n",auxatri->nome, auxatri->tamanho, auxatri->tipo);
				if(auxatri->tipo == 'C'){
					scanf("%c", &caracter);
					getchar();
					c = insereValor(c, auxatri->nome, &caracter);
				}else if(auxatri->tipo == 'I'){
					scanf("%d", &inteiro);
					sprintf(string, "%i", inteiro);  
					getchar();
					c = insereValor(c, auxatri->nome, string);
				}else if(auxatri->tipo == 'D'){
					scanf("%lf", &doublevar);
					sprintf(string, "%lf", doublevar); 
					getchar();
					c = insereValor(c, auxatri->nome, string);
				}else if(auxatri->tipo == 'S'){
					//scanf("%s", string);
					fgets(string, auxatri->tamanho, stdin);
					string[strlen(string)-1]='\0';
					c = insereValor(c, auxatri->nome, string);
				}

				auxatri=auxatri->next;
			}
			erro = finalizaInsert(tabela, c);
			if(erro != SUCCESS){
				printf("Erro %d: na função finalizaInsert()\n", erro);
				return ;
			}
		}
		aux=aux->next;
	}
	
}

void mostrartabelasfinal(TableList *tabelas){
	printf("\n");
	printf("TABELAS\n");
	int erro,x,flag;
	Table *aux;
	aux = tabelas->first;
	while(aux!=NULL){
		printf("|%s|\n",aux->nome);
		struct fs_objects objeto = leObjeto(aux->nome);	
		tp_table *esquema = leSchema(objeto);
			if(esquema == ERRO_ABRIR_ESQUEMA){
				printf("Erro ao criar o esquema.\n");
				return;
			}

			tp_buffer *bufferpoll = initbuffer();

			if(bufferpoll == ERRO_DE_ALOCACAO){
				printf("Erro ao alocar memória para o buffer.\n");
				return;
			}
			x=0;
			flag=0;
			while(x!=-1){
				erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);
				if(erro != SUCCESS){
					if(x==0)
					flag=1;
					x=-1;
				}
				else
				x++;
			}
			if(erro != SUCCESS && flag==1){
				printf("Tabela vazia\n");
			}
			else{

			erro = printbufferpoll(bufferpoll, esquema, objeto, 0);

			if(erro != SUCCESS){
				printf("Erro %d: na função printbufferpoll().\n", erro);
				return;
			}
		}
			
			
		aux=aux->next;
	}
	printf("\n");
	return;
}

Table *criaTabela(char Nome[]){
	Table *novatabela;
	novatabela = (Table *) malloc (sizeof(Table));
	novatabela->nome = (char*)malloc(strlen(Nome) + 2);
	strcpy(novatabela->nome,  Nome);
	novatabela->atributos = (Atributos *) malloc(sizeof(Atributos));
	inicializalistaatributos(novatabela->atributos);
	return novatabela;
}

Atributo *criaCampo(char Nome[], char tipo, int tam){
	//Atributo *novoatributo;
	Atributo *novoatributo = (Atributo *) malloc (sizeof(Atributo));
	novoatributo->nome = (char*)malloc(strlen(Nome)*sizeof(char)+2);
	strcpy(novoatributo->nome, Nome);	
	novoatributo->tamanho=tam;
	novoatributo->tipo=tipo;
	//insereatributo(novatabela->atributos, novoatributo);
	return novoatributo;
}

void init(TableList *tabelas){
	
	Table *novatabela=NULL;
	Atributo *novoatributo=NULL;
	
	inicializalistatabelas(tabelas);
	
	/*Criação da Tabela Cliente*/
	novatabela = criaTabela("Cliente");
	
	novoatributo = criaCampo("Nome", 'S', 60);
	insereatributo(novatabela->atributos, novoatributo);

	
	novoatributo = criaCampo("CNPJ", 'I',14);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Rasao Social", 'S', 60);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Endereco", 'S', 200);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Telefone", 'I', 14);
	insereatributo(novatabela->atributos, novoatributo);
	
	inseretabela(tabelas, novatabela);
	
	
	/*Criação da Tabela Produto*/
	novatabela = criaTabela("Produto");
	
	novoatributo = criaCampo("Id", 'I',10);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Nome", 'S', 50);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Especificacao", 'S', 200);
	insereatributo(novatabela->atributos, novoatributo);
	
	inseretabela(tabelas, novatabela);

	/*Criação da Tabela Nota Fiscal*/
	novatabela = criaTabela("NotaFiscal");
	
	novoatributo = criaCampo("Numero", 'I',  8);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Descricao", 'S', 200);
	insereatributo(novatabela->atributos, novoatributo);
	
	inseretabela(tabelas, novatabela);
	
	/*Criação da Tabela Funcionario*/
	novatabela = criaTabela("Funcionario");
	
	novoatributo = criaCampo("Nome", 'S', 60);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("CPF", 'I', 14);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Nome da mae", 'S', 60);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Endereco", 'S', 200);
	insereatributo(novatabela->atributos, novoatributo);
	
	novoatributo = criaCampo("Telefone", 'I', 15);
	insereatributo(novatabela->atributos, novoatributo);
	
	inseretabela(tabelas, novatabela);
}

int  main(){
	char Menu;
	int x;
	TableList *tabelas=(TableList *) malloc (sizeof(TableList));
	init(tabelas); //Incia todas as tabelas do banco;
	
	criartabelas(tabelas);
	printf("Deseja ir para o menu?\n");
	printf("[s/n]\n");
	scanf("%c", &Menu);
	getchar();
	if (Menu=='s'){
		x=1;
	}else
		x=4;

	char tabela[100];
	while(x!=4){
		printf("MENU\n");
		printf("1-Inserir tupla\n");
		printf("2-Mostrar tabelas\n");
		printf("3-Deletar o Banco de Dados e sair\n");
		printf("4-Sair\n");
		scanf("%d",&x);
		getchar();
		if(x == 1){
			printf("Em qual tabela deseja adicionar: ");
			scanf("%s",tabela);
			getchar();
			inserirtupla(tabela, tabelas);
		}else if(x == 2){
			mostrartabelasfinal(tabelas);	
		}else if (x==3){
			printf("Tem certeza que deseja excluir o banco? [s/n]\n");
			scanf("%c", &Menu);
			getchar();
			if (Menu=='s'){
				system("rm -f *.dat");
				x=4;
			}
		}
	}
	printf("\n");
	return 0;
}

