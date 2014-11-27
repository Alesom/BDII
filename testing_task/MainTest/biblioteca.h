#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dado{
	int *i;
	double *d;
	char *S;
	int tam;
} Dado;

typedef struct SN{
	Dado ID;
	struct SN *next;
	struct SN *previous; 
} Nodo_t;

typedef struct {
	Nodo_t *First;
	Nodo_t *Last;
} sentinela_lista2_t;

void insert (Dado novo, sentinela_lista2_t *lista);

void troca(Nodo_t *um, Nodo_t *dois);

Nodo_t *DivideLista (sentinela_lista2_t *A, Nodo_t *P, Nodo_t *R);

void QuickSort (sentinela_lista2_t *A, Nodo_t *P, Nodo_t *R);

void print (sentinela_lista2_t *lista);

sentinela_lista2_t *Duplic_list (const sentinela_lista2_t *loc);	

