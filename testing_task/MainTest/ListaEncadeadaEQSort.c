#include "biblioteca.h"

void insert (Dado novo, sentinela_lista2_t *lista){
	Nodo_t *nodo;
	nodo=(Nodo_t*)malloc(sizeof(Nodo_t));//alocando espaço para o novo nó
	nodo->next=NULL;
	nodo->previous=NULL;
	nodo->ID=novo;

	if (lista->First==NULL){//se a lista esta vazia
		lista->First=nodo;
		lista->Last=nodo;
		lista->First->next=NULL;
		lista->Last->next=NULL;
		lista->First->previous=NULL;
		lista->First->previous=NULL;
		nodo->next=NULL;
		nodo->previous=NULL;
		//printf("Sucesso! Elemento inserido com êxito \n");
	}else {	
		lista->Last->next=nodo;
		nodo->previous=lista->Last;	
		lista->Last=nodo;
		nodo->next=NULL;
		//printf("Sucesso! Elemento inserido com êxito \n");
	}
}//fim

int choice (Dado A, Dado B){
	if (A.i != NULL){
		if (*(A.i) <= *(B.i))
			return 1;
		return 0;
	}else if (A.d != NULL){
		if (*(A.d) <= *(B.d))
			return 1;
		return 0;
	}else{
		if (strncmp(A.S, B.S, A.tam) <= 0)
			return 1;
		return 0;
	}
}

Nodo_t *DivideLista (sentinela_lista2_t *A, Nodo_t *P, Nodo_t *R){ // divide a lista em duas
	Nodo_t *Ptri, *Ptrj;
	int most=0;
	Dado x=R->ID;
	Ptri=P;

	for (Ptrj=P; Ptrj !=R ; Ptrj=Ptrj->next){		
		//if (((*Ptrj).ID) <= x){
		if (choice(Ptrj->ID, x)){
			most++;
			if (most<=1)
				Ptri=P;
			else
				Ptri=Ptri->next;

			troca (Ptri, Ptrj);
		}
	}

	if (most!=0){	 
		troca(Ptri->next, R);
		return Ptri->next;
	}else{ 
		troca(Ptri, R);
		return Ptri;
	}
}

void troca (Nodo_t *um, Nodo_t *dois){ //sweep
	int Ia, Ib;
	double Da, Db;
	char *Sa, *Sb;
	 
	if (um->ID.i != NULL){
		Ia=*(um->ID.i);
		Ib=*(dois->ID.i);
		
		*(um->ID.i)=Ib;
		*(dois->ID.i)=Ia;
	}else if (um->ID.d != NULL){
		Da=*(um->ID.d);
		Db=*(dois->ID.d);
		
		*(um->ID.d)=Db;
		*(dois->ID.d)=Da;
	}else{
		Ia=strlen(um->ID.S);
		Ib=strlen(dois->ID.S);
		Sa=(char *) malloc(sizeof(char));
		Sb=(char *) malloc(sizeof(char));
		strcpy(Sa, um->ID.S);
		strcpy(Sb, dois->ID.S);
		
		strcpy(um->ID.S, Sb);
		strcpy(dois->ID.S, Sa);
	}
}

int func(Dado *A, Dado *B){
	if (A->i != NULL){
		if (*(A->i) > *(B->i))
			return 1;
		return 0;
	}else if (A->d != NULL){
		if (*(A->d) > *(B->d))
			return 1;
		return 0;
	}else{
		if (strcmp(A->S, B->S) > 0)
			return 1;
		return 0;
	}
}

void QuickSort (sentinela_lista2_t *A, Nodo_t *P, Nodo_t *R){ //funcao principal do Quick Sort
	Nodo_t *Ptrq;
	if ((P!=R->previous) && (P!=R)){
		Ptrq=DivideLista(A, P, R);
		if (Ptrq!=P)
			QuickSort(A, P, Ptrq->previous);

		if (Ptrq!=R)
			QuickSort(A, Ptrq->next, R);
	}else{
		if (func(&(*P).ID , &(*R).ID)){
			troca (P, R);
		}
	}
}

void print (sentinela_lista2_t *lista){
	Nodo_t *ptr;
	unsigned int n=0;
	if (lista->First==NULL){
		printf("A lista esta vazia!! Não é possível mostrar nenhum elemento!");
	}else{
		system("clear");
		ptr=lista->First;
		while (ptr!=lista->Last){
			n++;
			printf("O %dº elemento da lista é o que possui código:	", n);
			if ((*ptr).ID.i != NULL){
				printf("%d", *((*ptr).ID.i)); 
			}else if ((*ptr).ID.d != NULL){
				printf("%lf", *((*ptr).ID.d)); 
			}else{
				printf("%s", (*ptr).ID.S);
			}
			printf("\n\n\n");
			ptr=ptr->next;		
		}//while
		n++;
		printf("O %dº elemento da lista é o que possui código:	", n);
		if ((*ptr).ID.i != NULL){
			printf("%d", *((*lista->Last).ID.i)); 
		}else if ((*ptr).ID.d != NULL){
			printf("%lf", *((*lista->Last).ID.d)); 
		}else{
			printf("%s", (*lista->Last).ID.S);
		}
		printf("\n\n\n");
	}
}




