#include "buffend.h"

int ValidaCampos(char NomeDaTabela[], char *dt, int flag){
	int Tam, i, EsquemaAcumu;
	FILE *data;
	char *BUFFER, *NomeAuxiliar;
	
	struct fs_objects objeto = leObjeto(NomeDaTabela);
	tp_table *esquema = leSchema(objeto);
	
	if (esquema==ERRO_ABRIR_ESQUEMA)
		return ERRO_O_VALOR_NAO_PODE_SER_INSERIDO;
	
	
	Tam=tamTupla(esquema, objeto);
	BUFFER=malloc(sizeof(char)*Tam);

	if (BUFFER==NULL)
		return ERRO_NAO_HA_ESPACO;
	
	NomeAuxiliar= (char *)malloc(sizeof(char)*strlen(NomeDaTabela)+6);
	
	if (NomeAuxiliar == NULL)
		return ERRO_NAO_HA_ESPACO;
	
	strcpy(NomeAuxiliar, NomeDaTabela);
	strcat(NomeAuxiliar, ".dat");
		
	data=fopen(NomeAuxiliar, "r");
	
	if (data==NULL){
		return ERRO_ABRIR_ARQUIVO;
	}

	fseek(data, 0, SEEK_SET);
	
	for (i=EsquemaAcumu=0;i < objeto.qtdCampos; i++){
		fread(BUFFER, sizeof(char), esquema[i].tam, data);
		if (esquema[i].pk){
			//fseek(data, EsquemaAcumu, SEEK_CUR);
			printf("%s %s\n", BUFFER, dt);
			switch(esquema[i].tipo){
				case 'S':
					if (strcmp(BUFFER, dt)==0)
						return -14;
					break;
				case 'I':
					if (convertI(BUFFER) == convertI(dt))
						return -14;
					break;
				case 'D':
					if (convertD(BUFFER) == convertD(dt))
						return -14;
					break;
				case 'C':
					if (strncmp(BUFFER, dt, 1)==0)
						return -14;
					break;
			}
		}
		//EsquemaAcumu+=esquema[i].tam;
	}
	fclose(data);
	if (!flag) 
		return ERRO_A_FK_NAO_REFERENCIA_UMA_TABELA_OU_CHAVE_VALIDA;
	
	return SUCCESS;
}
