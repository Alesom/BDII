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
			printf("%s(%d-%c)\n",auxatri->nome, auxatri->tamanho, auxatri->tipo);
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
			printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
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
					c = insereValor(c, auxatri->nome, &caracter);
				}else if(auxatri->tipo == 'I'){
					scanf("%d", &inteiro);
					sprintf(string, "%i", inteiro);  
					c = insereValor(c, auxatri->nome, string);
				}else if(auxatri->tipo == 'D'){
					scanf("%lf", &doublevar);
					sprintf(string, "%lf", doublevar); 
					c = insereValor(c, auxatri->nome, string);
				}else if(auxatri->tipo == 'S'){
					scanf("%s", string);
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
	int erro;
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

			erro = colocaTuplaBuffer(bufferpoll, 0, esquema, objeto);

			if(erro != SUCCESS){
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
	return;
}

void le(){
	char B[MAX];
	int tam;

	FILE *arq=fopen("inicialize.dat", "r+");
	if (arq!=NULL){
		printf("cadeeeente\n");
		fseek(arq, 0, SEEK_SET);
		while(!feof(arq)){
			fread(&tam, sizeof(int), 1, arq);
			fread(B, sizeof(char), tam+1, arq);
			B[tam]='\0';
			printf("%d %s\n", tam, B);
		}
	}
	fclose(arq);
}
/*
void inicia(int *argc, char **argv){
	FILE *arq;
	int i, tam;
	arq=fopen("inicialize.dat", "r+");
	if (arq==NULL){
		arq=fopen("inicialize.dat", "w+");
		fseek(arq, 0, SEEK_END);
		for (i=1;i<*argc-1;i++){
			tam=strlen(argv[i]);
			fwrite(&tam, sizeof(int), 1, arq);
			fwrite(argv[i], sizeof(char), tam, arq);
			fwrite(" ", 1, 1, arq);
		}
		fclose(arq);
		//le();
	}else{
	//	le();
		free(argv);
		fseek(arq, 0, SEEK_SET);
		argv=malloc(sizeof(char *) * (*argc+1));
		//argv[0]=malloc(sizeof(char)*7);
		for (i=1; !feof(arq);i++){
			fread(&tam, sizeof(int), 1, arq);
			argv[i] = malloc(sizeof(char) * (tam+1));
			fread(argv[i], sizeof(char), tam+1, arq);
			argv[i][tam]='\0';
		}
		*argc=i;
		for (i=1;i<*argc;i++){
			printf("AR->%s %d\n", argv[i], i);
		}
	}
	fclose(arq);
}*/

int  main(int argc, char **argv){
	char Menu;
	int x;
	char t[2];
	TableList *tabelas=(TableList *) malloc (sizeof(TableList));
	Table *novatabela=NULL;
	Atributo *novoatributo=NULL;
	FILE *arq; //bloco feito por ultimo
	int i, tam;
	
	system("clear");
	arq=fopen("inicialize.dat", "r+");
	
	if (arq==NULL){
		if (argc < 2) {
			printf("Aconteceu um erro grave! Verifique se os parâmetros da execução foram passados corretamente\n");
			return 0;
		}
		arq=fopen("inicialize.dat", "w+");
		for (i=1;i<argc-1;i++){
			tam=strlen(argv[i]);
			fwrite(&tam, sizeof(int), 1, arq);
			fwrite(argv[i], sizeof(char), tam, arq);
			fwrite(" ", 1, 1, arq);
		}
		fclose(arq);
		//le();
	}else{ // VER COMO FAZER PARA ADICIONAR MAIS TABELAS AO BANCO
		fseek(arq, 0, SEEK_SET);
		i=0;
		argv=malloc(sizeof(char *) * MAX);
		for (i=1; !feof(arq);i++){
			fread(&tam, sizeof(int), 1, arq);
			argv[i] = malloc(sizeof(char) * (tam+1));
			fread(argv[i], sizeof(char), tam+1, arq);
			argv[i][tam]='\0';
		}
		argc=i;
		fclose(arq);
	}
	
	//fim do bloco

	inicializalistatabelas(tabelas);
	strcpy(t,"-t");
	//printf("%d\n",argc);
	printf("O seu banco tem as sequintes tabelas:\n\n");
	for (i=1; i<argc;i++){
		//printf("%s %d\n", argv[i], i);
		if(strncmp(argv[i], t, 2)==0){
			if (i!=1)
				printf("\n\n\n");
			printf("Tabela: %s",argv[i+1]);
			novatabela = (Table *) malloc (sizeof(Table));
			novatabela->nome = (char*)malloc(strlen(argv[i+1])*sizeof(char));
			strcpy(novatabela->nome, argv[i+1]);
			printf("--%s\n",novatabela->nome);
			novatabela->atributos = (Atributos *) malloc(sizeof(Atributos));
			inicializalistaatributos(novatabela->atributos);
			i++;
		}else{
			novoatributo = (Atributo *) malloc (sizeof(Atributo));
			printf("Atributo: %s ", argv[i]);
			novoatributo->nome = (char *) malloc (strlen(argv[i])*sizeof(char));
			strcpy(novoatributo->nome, argv[i]);
			i++;
			printf("Tipo: %c ", argv[i][0]);
			novoatributo->tipo= argv[i][0];
			i++;
			printf("Tamanho: %s\n", argv[i]);
			novoatributo->tamanho=atoi(argv[i]);
			insereatributo(novatabela->atributos, novoatributo);
		}
		if((i+1)>=argc || strncmp(argv[i+1], t, 2)==0)
			inseretabela(tabelas, novatabela);
		printf("\n");
	}
	
	//mostrartabelascomatributos(tabelas);
	
	criartabelas(tabelas);
	printf("Deseja ir para o menu?\n");
	printf("[s/n]\n");
	scanf("%c", &Menu);
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
		if(x == 1){
			printf("Em qual tabela deseja adicionar: ");
			scanf("%s",tabela);
			inserirtupla(tabela, tabelas);
		}else	if(x == 2){
			mostrartabelasfinal(tabelas);	
		}else if (x==3){
			system("rm -f *.dat");
			x=4;
		}
	}
	printf("\n");
	return 0;
}
