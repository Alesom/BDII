#include "biblioteca.h"

int main(){
	sentinela_lista2_t *um;
	um=(sentinela_lista2_t*)calloc(1, sizeof(sentinela_lista2_t));
	
	int n;
	char c[112345];
	Dado D;
	
	while (scanf("%d", &n)!=EOF && n!=-1){
		switch(n){
			case 1:
				getchar();
				fgets(c, 80, stdin);
				c[strlen(c)-1]='\0';
				D.i=NULL; D.d=NULL;
				D.S=(char *)malloc(strlen(c)*sizeof(char)+20);
				strcpy(D.S, c);
				D.tam=80;
				insert(D, um);
				break;
			case 2:
				D.i=(int *)malloc(sizeof(int));
				scanf("%d", D.i);
				D.d=NULL;
				D.S=NULL;
				D.tam=1;
				insert(D, um);
				//printf("oi");
				break;
			case 3:
				D.d=(double *)malloc(sizeof(double));
				scanf("%lf", D.d);
				D.i=NULL;
				D.S=NULL;
				D.tam=1;
				insert(D, um);
				break;
			case 4:
				print(um);
				break;
			case 5:
				QuickSort(um, um->First, um->Last);
				print(um);
				break;
		}
	}
	
	return 0;
}
