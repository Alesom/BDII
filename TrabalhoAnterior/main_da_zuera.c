//só compilar
//formato de chamada ./main_da_zuera -t <nome_tabela> <atributo> <tipo> <tamanho> ...
//exemplo de chamada: ./main_da_zuera -t pessoa idade I 2 nome S 20 -t endereco rua S 20 bairo S 20
//tipos existentes : char, int, double, string
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _atributo{
	char *nome;
	char *tipo;
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
			printf("%s(%d-%s)\n",auxatri->nome, auxatri->tamanho, auxatri->tipo);
			auxatri=auxatri->next;
		}
		aux=aux->next;
	}
}
int  main(int argc, char **argv){
	int i;
	char t[2];
	TableList *tabelas=(TableList *) malloc (sizeof(TableList));
	Table *novatabela=NULL;
	Atributo *novoatributo=NULL;
	inicializalistatabelas(tabelas);
	strcpy(t,"-t");
	printf("%d\n",argc);
	for (i=1; i<argc-1;i++){
		if(strncmp(argv[i], t, 2)==0){
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
				printf("Tipo: %s ", argv[i]);
				novoatributo->tipo = (char *) malloc (strlen(argv[i])*sizeof(char));
				strcpy(novoatributo->tipo, argv[i]);
				i++;
				printf("Tamanho: %s\n", argv[i]);
				novoatributo->tamanho=atoi(argv[i]);
				insereatributo(novatabela->atributos, novoatributo);
			}
			if((i+1)>=argc || strncmp(argv[i+1], t, 2)==0)
			inseretabela(tabelas, novatabela);
	}
	
	mostrartabelascomatributos(tabelas);
	
	

	printf("\n");
	return 0;
}
