#include "buffend.h"

// LEITURA DE DICIONARIO E ESQUEMA

struct fs_objects leObjeto(char *nTabela){

	FILE *dicionario;
	char *tupla = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);
	int cod;
	dicionario = fopen("fs_object.dat", "a+b"); // Abre o dicionario de dados.

	struct fs_objects objeto;

	if(!verificaNomeTabela(nTabela)){
		printf("Erro GRAVE! na função leObjeto(). Nome da tabela inválido.\nAbortando...\n");
		exit(1);
	}

	if (dicionario == NULL)	{
		printf("Erro GRAVE! na função leObjeto(). Arquivo não encontrado.\nAbortando...\n\n");
		exit(1);
	}
	

	while(fgetc (dicionario) != EOF){
		fseek(dicionario, -1, 1);

		fread(tupla, sizeof(char), TAMANHO_NOME_TABELA , dicionario); //Lê somente o nome da tabela

		if(strcmp(tupla, nTabela) == 0){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
			strcpy(objeto.nome, tupla);
			fread(&cod,sizeof(int),1,dicionario);	// Copia valores referentes a tabela pesquisada para a estrutura.
			objeto.cod=cod;
			fread(tupla,sizeof(char),TAMANHO_NOME_TABELA,dicionario);
			strcpy(objeto.nArquivo, tupla);
			fread(&cod,sizeof(int),1,dicionario);
			objeto.qtdCampos = cod;
			
			return objeto;
		}
		fseek(dicionario, 28, 1); // Pula a quantidade de caracteres para a proxima verificacao(4B do codigo, 20B do nome do arquivo e 4B da quantidade de campos).
	}
	return objeto;
}

tp_table *leSchema (struct fs_objects objeto){
	FILE *schema;
	int i = 0, cod;
	char *tupla = (char *)malloc(sizeof(char)*TAMANHO_NOME_CAMPO);  
	tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos); // Aloca esquema com a quantidade de campos necessarios.

	if(esquema == NULL)
		return ERRO_DE_ALOCACAO;

	schema = fopen("fs_schema.dat", "a+b"); // Abre o arquivo de esquemas de tabelas.

	if (schema == NULL)
		return ERRO_ABRIR_ESQUEMA;

	while((fgetc (schema) != EOF) && (i < objeto.qtdCampos)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
		fseek(schema, -1, 1);

		if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
			if(cod == objeto.cod){ // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.
				
				fread(tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
				strcpy(esquema[i].nome,tupla);					// Copia dados do campo para o esquema.
				fread(&esquema[i].tipo, sizeof(char),1,schema);
				fread(&esquema[i].tam, sizeof(int),1,schema);  
				fread(&esquema[i].pk, sizeof (int), 1, schema);
				fread(&esquema[i].fk, sizeof (int), 1, schema);
				fread(esquema[i].ref, sizeof (char), TAMANHO_NOME_TABELA, schema);
				i++; 
			}else{
				int tamanho=sizeof(char) * TAMANHO_NOME_CAMPO +  sizeof(char) + sizeof(int) + sizeof (int) + sizeof (int)+ sizeof (char) *TAMANHO_NOME_TABELA;
				
				fseek(schema, tamanho, 1); // Pula a quantidade de caracteres para a proxima verificacao ( do nome, 1B do tipo e 4B do tamanho). 
					/*foi alterado para os novos valores 100 do nome da tabela referênciada e 8 de dois inteiros.*/
			}
		}
	}
	return esquema;
}
//--------------------------------------------------
// INICIALIZACAO DO BUFFER
tp_buffer * initbuffer(){
	
	tp_buffer *bp = (tp_buffer*)malloc(sizeof(tp_buffer)*PAGES);
	int i;
	if(bp == NULL)
		return ERRO_DE_ALOCACAO;
	for (i = 0;i < PAGES; i++){
		bp->db=0;
		bp->pc=0;
		bp->nrec=0;
		bp++;
	}
	return bp;
}
//--------------------------------------------------
// IMPRESSAO BUFFER
void cria_campo(int tam, int header, char *val, int x){
	int i;
	char aux[30];

	if(header){
		for(i = 0; i <= 30 && val[i] != '\0'; i++){
			aux[i] = val[i];
		}
		for(;i<30;i++)
			aux[i] = ' ';
		aux[i] ='\0';
		printf("%s", aux);

	}

	else{
		for(i = 0; i < x; i++)
			printf(" ");
	}
}
int drawline(tp_buffer *buffpoll, tp_table *s, struct fs_objects objeto, int p, int num_page){
	 
	if (num_page > PAGES || p > SIZE){
		return ERRO_DE_PARAMETRO;
	}
	int *pos_ini, aux = (p * tamTupla(s,objeto)) , num_reg = objeto.qtdCampos;
	pos_ini = &aux;
	int count, pos_aux, bit_pos;
	union c_double cd;
	union c_int ci;
	int x = 0;
	
	count = pos_aux = bit_pos = 0;
	
	for(count = 0; count < num_reg; count++){
		pos_aux = *(pos_ini);
		bit_pos = 0;
		

		switch(s[count].tipo){
			
			case 'S':
				x = 0;
				while(buffpoll[num_page].data[pos_aux] != '\0'){
			
					printf("%c", buffpoll[num_page].data[pos_aux]);
					if ((buffpoll[num_page].data[pos_aux++] & 0xc0) != 0x80) bit_pos++; //Conta apenas bits que possam ser impressos (UTF8)
				x++;
				}
				
				cria_campo((TAMANHO_NOME_CAMPO - (bit_pos)), 0, (char*)' ', (30 - x));
				break;
			
			case 'I':
				while(pos_aux < *(pos_ini) + s[count].tam){
					ci.cnum[bit_pos++] = buffpoll[num_page].data[pos_aux++];
				}
				printf("%d", ci.num); //Controla o número de casas até a centena
				cria_campo((TAMANHO_NOME_CAMPO - (bit_pos)), 0, (char*)' ', 28);
				break;
				
			case 'D':
				while(pos_aux < *(pos_ini) + s[count].tam){
					cd.double_cnum[bit_pos++] = buffpoll[num_page].data[pos_aux++]; // Cópias os bytes do double para área de memória da union
				}
				printf("%.3lf", cd.dnum);
				cria_campo((TAMANHO_NOME_CAMPO - (bit_pos)), 0, (char*)' ', 24);
				break;
			
			case 'C': 
				printf("%c", buffpoll[num_page].data[pos_aux]);
				if(s[count].tam < strlen(s[count].nome)){
					bit_pos = strlen(s[count].nome);
				}
				else{
					bit_pos = s[count].tam;
				}
				cria_campo((bit_pos - 1), 0, (char*)' ', 29);	
				break;
			
			default: 
				return ERRO_IMPRESSAO;
				break;
		}
		*(pos_ini) += s[count].tam;		
	}
	printf("\n");
	return SUCCESS;
}
int cabecalho(tp_table *s, int num_reg){
	int count, aux;
	aux = 0;
	
	for(count = 0; count < num_reg; count++){
		cria_campo(s[count].tam, 1, s[count].nome, 0); // O segundo parâmetro significa se = 1 Cabecalho se = 0 é valor daquele campo
		aux += s[count].tam;
	}
	printf("\n");
	return aux;
}
int printbufferpoll(tp_buffer *buffpoll, tp_table *s,struct fs_objects objeto, int num_page){
	
	int aux, i, num_reg = objeto.qtdCampos;
	
	
	if(buffpoll[num_page].nrec == 0){
		return ERRO_IMPRESSAO;	
	}
	
	i = aux = 0;
	
	aux = cabecalho(s, num_reg);
	

	while(i < buffpoll[num_page].nrec){ // Enquanto i < numero de registros * tamanho de uma instancia da tabela
		drawline(buffpoll, s, objeto, i, num_page);
		i++;
	}
	return SUCCESS;
}
//--------------------------------------------------
// FUNCOES AUXILIARES
int tamTupla(tp_table *esquema, struct fs_objects objeto){// Retorna o tamanho total da tupla da tabela.

    int qtdCampos = objeto.qtdCampos, i, tamanhoGeral = 0;
   
	if(qtdCampos){ // Lê o primeiro inteiro que representa a quantidade de campos da tabela.
		for(i = 0; i < qtdCampos; i++)
			tamanhoGeral += esquema[i].tam; // Soma os tamanhos de cada campo da tabela.
	}

	return tamanhoGeral;
}
int pot10(int n)
{
	if(n == 0)
		return 1;
	return 10 * pot10(n-1);
}
int strtam(char n[])
{
	if(n[0]==0)
		return 0;
	return 1+strtam(n+1);
}
int convertI(char u[])
{
	if(strtam(u) == 0)
		return 0;
	return (u[0]-48)*pot10(strtam(u)-1) + convertI(u+1);
}
double get_decimal(char u[]) 
{
	double decimal=0;
	int i,tamanho = strtam(u);
	for(i=0;i<tamanho && u[i]!='.';i++); // posiciona o indice i encima do ponto
	decimal=convertI(u+i+1);///(pot10(tamanho-i-1));
	decimal=(double)convertI(u+i+1)/(double)(pot10(tamanho-i-1));
	return decimal;
}
double get_inteiro(char v[]) 
{
	double inteiro=0;
	int i,tamanho = strtam(v);
	char u[10];
	strcpy(u,v); //copia o vetor para uma vaiável auxiliar para preservar sua originalidade
	for(i=0;i<tamanho && u[i]!='.';i++); // posiciona o indice i encima do ponto
	u[i]=0; // separa a parte inteira da parte decimal, inserindo um null no lugar do ponto
	inteiro=convertI(u);
	return inteiro;
}
double convertD(char u[]) 
{
	return get_inteiro(u)+get_decimal(u);
	//Soma inteiro com decimal.ss
}
int verificaNomeTabela(char *nomeTabela)
{

	FILE *dicionario;
	char *tupla = (char *)malloc(sizeof(char)*TAMANHO_NOME_TABELA);
	if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
    	return ERRO_ABRIR_ARQUIVO;

    while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        fread(tupla, sizeof(char), TAMANHO_NOME_TABELA, dicionario); //Lê somente o nome da tabela

        if(strcmp(tupla, nomeTabela) == 0){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
      		
        	return 1;
        }
        
        fseek(dicionario, 28, 1);
 	}

 	fclose(dicionario);
 	return 0;
}
int quantidadeTabelas()
{

	FILE *dicionario;
	int codTbl = 0;

    if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
    	return ERRO_ABRIR_ARQUIVO;

	while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

       	codTbl++; // Conta quantas vezes é lido uma tupla no dicionario.

        fseek(dicionario, 48, 1);
 	}

 	fclose(dicionario);

 	return codTbl;
}
//---------------------------------------
// INSERE UMA TUPLA NO BUFFER!
char *getTupla(tp_table *campos,struct fs_objects objeto, int from){ //Pega uma tupla do disco a partir do valor de from

    int tamTpl = tamTupla(campos, objeto);
    char *linha=(char *)malloc(sizeof(char)*tamTpl);
    FILE *dados;

    from = from * tamTpl;

    dados = fopen(objeto.nArquivo, "r");

	if (dados == NULL)
		return ERRO_DE_LEITURA;

    fseek(dados, from, 1);
    if(fgetc (dados) != EOF){
        fseek(dados, -1, 1);
        fread(linha, sizeof(char), tamTpl, dados); //Traz a tupla inteira do arquivo
    }
    else{       //Caso em que o from possui uma valor inválido para o arquivo de dados
        fclose(dados);
        return ERRO_DE_LEITURA;
    }
    fclose(dados);
    return linha;
}

void setTupla(tp_buffer *buffer,char *tupla, int tam, int pos){ //Coloca uma tupla de tamanho "tam" no buffer e na página "pos"
	int i=buffer[pos].position;
	for (;i<buffer[pos].position + tam;i++)
        buffer[pos].data[i] = *(tupla++);
}

int colocaTuplaBuffer(tp_buffer *buffer, int from, tp_table *campos, struct fs_objects objeto){//Define a página que será incluida uma nova tupla
	
	char *tupla = getTupla(campos,objeto,from);

 	if(tupla == ERRO_DE_LEITURA)
 		return ERRO_LEITURA_DADOS;

    int i=0, found=0;
	while (!found && i < PAGES)//Procura pagina com espaço para a tupla.
	{
    	if(SIZE - buffer[i].position > tamTupla(campos, objeto)){// Se na pagina i do buffer tiver espaço para a tupla, coloca tupla.
            setTupla(buffer, tupla, tamTupla(campos, objeto), i);
            found = 1;
            buffer[i].position += tamTupla(campos, objeto); // Atualiza proxima posição vaga dentro da pagina.
            buffer[i].nrec += 1;
            break;
    	}
    	i++;// Se não, passa pra proxima página do buffer.
    }
    
    if (!found)
		return ERRO_BUFFER_CHEIO;

	return SUCCESS;
}
//----------------------------------------
// CRIA TABELA
table *iniciaTabela(char *nome)
{
	if(verificaNomeTabela(nome)){	// Se o nome já existir no dicionario, retorna erro.
		return ERRO_NOME_TABELA_INVALIDO;
	}

	table *t = (table *)malloc(sizeof(table)*1);
	strcpy(t->nome,nome); // Inicia a estrutura de tabela com o nome da tabela.
	t->esquema = NULL; // Inicia o esquema da tabela com NULL.
	return t; // Retorna estrutura para criação de uma tabela.
}

table *adicionaCampo(table *t,char *nomeCampo, char tipoCampo, int tamanhoCampo, int pk, int fk, char *ref)
{
	if(t == NULL) // Se a estrutura passada for nula, retorna erro.
		return ERRO_ESTRUTURA_TABELA_NULA;

	tp_table *aux;  
	if(t->esquema == NULL) // Se o campo for o primeiro a ser adicionado, adiciona campo no esquema.
	{
		tp_table *e = (tp_table *)malloc(sizeof(tp_table)*1);
		e->next = NULL;
		strcpy(e->nome, nomeCampo); // Copia nome do campo passado para o esquema
		e->tipo = tipoCampo; // Copia tipo do campo passado para o esquema
		e->tam = tamanhoCampo; // Copia tamanho do campo passado para o esquema
		e->pk = pk;
		e->fk = fk;
		if(ref!=NULL && fk==1){
			strcat(ref,"\0");
			strcpy(e->ref,ref);
		}else
		memset(e->ref, 0, sizeof (e->ref));
		t->esquema = e; 
		return t; // Retorna a estrutura
	}
	else
	{ 
		for(aux = t->esquema; aux != NULL; aux = aux->next) // Anda até o final da estrutura de campos.
		{
			if(aux->next == NULL) // Adiciona um campo no final.
			{
				tp_table *e = (tp_table *)malloc(sizeof(tp_table)*1);
				e->next = NULL;
				strcpy(e->nome, nomeCampo);
				e->tipo = tipoCampo;
				e->tam = tamanhoCampo;
				e->pk = pk;
				e->fk = fk;
				if(ref!=NULL && fk==1){
					strcat(ref,"\0");
					strcpy(e->ref,ref);
				}else
				memset(e->ref, 0, sizeof (e->ref));
				aux->next = e; // Faz o campo anterior apontar para o campo inserido.
				return t;
			}
		}
	}

	return t; //Retorna estrutura atualizada.
}
int finalizaTabela(table *t)
{
	if(t == NULL)
		return ERRO_DE_PARAMETRO;

	FILE *esquema, *dicionario;
	tp_table *aux;
	int codTbl = quantidadeTabelas() + 1, qtdCampos = 0, flag=0; // Conta a quantidade de tabelas já no dicionario e soma 1 no codigo dessa nova tabela.
	char nomeArquivo[TAMANHO_NOME_ARQUIVO];

	if((esquema = fopen("fs_schema.dat","a+b")) == NULL)
		return ERRO_ABRIR_ARQUIVO;

	//jump=sizeof(codTbl)+sizeof(aux->nome)+sizeof(aux->tipo)+sizeof(aux->tam)+sizeof(aux->pk)+sizeof(aux->fk)+sizeof(aux->ref); //salto caso existe uma chave estrangeira que não referência nada
	
	for(aux = t->esquema; aux!=NULL; aux = aux->next) // Salva novos campos no esquema da tabela, fs_schema.dat
	{
		if(!aux->fk || (aux->fk && !verificaNomeTabela(t->esquema->nome))){ 
			fwrite(&codTbl,sizeof(codTbl),1,esquema);
			fwrite(&aux->nome,sizeof(aux->nome),1,esquema);
			fwrite(&aux->tipo,sizeof(aux->tipo),1,esquema);
			fwrite(&aux->tam,sizeof(aux->tam),1,esquema);
			fwrite(&aux->pk,sizeof(aux->pk),1,esquema);
			fwrite(&aux->fk,sizeof(aux->fk),1,esquema);
			fwrite(&aux->ref,sizeof(aux->ref),1,esquema);
			qtdCampos++; // Soma quantidade total de campos inseridos.
		}else{
			if (aux->fk){
				flag=1;
			}
		//	fseek(esquema, jump, SEEK_CUR);
		}
	}
	
	if (flag)
		return ERRO_A_FK_NAO_REFERENCIA_UMA_TABELA_OU_CHAVE_VALIDA;

	fclose(esquema);

	if((dicionario = fopen("fs_object.dat","a+b")) == NULL)
		return ERRO_ABRIR_ARQUIVO;

	strcpy(nomeArquivo, t->nome);
	strcat(nomeArquivo, ".dat\0");
	strcat(t->nome, "\0");
	// Salva dados sobre a tabela no dicionario.
	fwrite(&t->nome,sizeof(t->nome),1,dicionario);
	fwrite(&codTbl,sizeof(codTbl),1,dicionario);
	fwrite(&nomeArquivo,sizeof(nomeArquivo),1,dicionario);
	fwrite(&qtdCampos,sizeof(qtdCampos),1,dicionario);

	fclose(dicionario);
	return SUCCESS;
}
//-----------------------------------------
// INSERE NA TABELA
column *insereValor(column *c, char *nomeCampo, char *valorCampo)
{

	column *aux;
	if(c == NULL) // Se o valor a ser inserido é o primeiro, adiciona primeiro campo.
	{
		column *e = (column *)malloc(sizeof(column)*1);
		e->valorCampo = (char *)malloc(sizeof(char) * (sizeof(valorCampo)));
		strcpy(e->nomeCampo, nomeCampo); 
		strcpy(e->valorCampo, valorCampo);
		e->next = NULL;
		c = e;
		return c;
	} 
	else
	{
		for(aux = c; aux != NULL; aux = aux->next) // Anda até o final da lista de valores a serem inseridos e adiciona um novo valor.
		{
			if(aux->next == NULL)
			{
				column *e = (column *)malloc(sizeof(column)*1);
				e->valorCampo = (char *)malloc(sizeof(char) * (sizeof(valorCampo)));
				e->next = NULL;
				strcpy(e->nomeCampo, nomeCampo);
				strcpy(e->valorCampo, valorCampo);
				aux->next = e;
				return c;
			}
		}
	}

	return ERRO_INSERIR_VALOR;
}

int finalizaInsert(char *nome, column *c)
{
	column *auxC, *j;
	int i = 0, x = 0, t, k;
	FILE *dados;
	char *NomeAuxiliar;

	struct fs_objects dicio = leObjeto(nome); // Le dicionario
	tp_table *auxT = leSchema(dicio); // Le esquema
	
	if((dados = fopen(dicio.nArquivo,"a+b")) == NULL)
		return ERRO_ABRIR_ARQUIVO;
	
	
	for(auxC = c, t = 0; auxC != NULL; auxC = auxC->next, t++){ //valida os campos antes de inserção 
		if(t >= dicio.qtdCampos)
			t = 0;
			
		if (auxT[t].pk==1){// se for chave primária, executa esse if 
			for (j=c; auxC != j; j=j->next){ //verifica se existe um elemento que tem uma chave primaria igual a outro arquivo existente na lista
				if (strncmp(j->valorCampo, auxC->valorCampo, auxT[t].tam)==0)
					return ERRO_CAMPO_JA_EXISTENTE;
			}
			
			//printf("%s\n", nome);
			NomeAuxiliar=malloc(sizeof(char)*strlen(nome)+6);
			if (NomeAuxiliar==NULL)
				return ERRO_NAO_HA_ESPACO;
			strcpy(NomeAuxiliar, nome);
			strcat(NomeAuxiliar, ".dat");
			
			FILE *aux1=fopen(NomeAuxiliar, "r");
			if (aux1 != NULL){ //se existe um arquivo, verifica no arquivo; 
				fseek(aux1, 0, SEEK_END);
				if (ftell(aux1) > 1){
					fclose (aux1);
					k=ValidaCampos(nome, auxC->valorCampo, 1);
					//printf("%d\n", k);
					if (k==ERRO_CAMPO_JA_EXISTENTE){
						//printf("conflito\n");
						return ERRO_CAMPO_JA_EXISTENTE;	
					}
				}else{
					fclose(aux1);
				}
			}
		}else if (auxT[t].fk==1){
			k=ValidaCampos(auxT[t].ref, auxC->valorCampo, 0);
			if (k==ERRO_CAMPO_JA_EXISTENTE) //PESQUISOU, VIU QUE JÁ EXISTIA CHAVE PRIMARIA ENTÃO RETORNA SUCESSO
				continue;
			if (k==ERRO_A_FK_NAO_REFERENCIA_UMA_TABELA_OU_CHAVE_VALIDA){		
				return ERRO_A_FK_NAO_REFERENCIA_UMA_TABELA_OU_CHAVE_VALIDA;
			}
		}
	}
	//se os campos estão válidos, ele grava nos arquivos
	
	
	for(auxC = c, t = 0; auxC != NULL; auxC = auxC->next, t++)
	{
		if(t >= dicio.qtdCampos)
			t = 0;
		
		if(auxT[t].tipo == 'S'){ // Grava um dado do tipo string.
			if(strlen(auxC->valorCampo) > auxT[t].tam){   //A LINHA ESTAVA sizeof(auxC->valorCampo) > auxT[t].tam, sizeof(auxC->valorCampo) retorna o tamanho do ponteiro;
				return ERRO_NO_TAMANHO_STRING;
			}
			if(strcmp(auxC->nomeCampo, auxT[t].nome) != 0){
				printf("%s %s\n\n", auxC->nomeCampo, auxT[t].nome);
				return ERRO_NOME_CAMPO;
			}
			char valorCampo[auxT[t].tam];
			strcpy(valorCampo, auxC->valorCampo);
			strcat(valorCampo, "\0");
				
			fwrite(&valorCampo,sizeof(valorCampo),1,dados);
		}
		else if(auxT[t].tipo == 'I'){ // Grava um dado do tipo inteiro.
			i = 0;
			while (i < strlen(auxC->valorCampo))
			{
				if(auxC->valorCampo[i] < 48 || auxC->valorCampo[i] > 57){ 
					return ERRO_NO_TIPO_INTEIRO;
				}
				i++;
			}

			int valorInteiro = convertI(auxC->valorCampo);
			fwrite(&valorInteiro,sizeof(valorInteiro),1,dados);
		}
		else if(auxT[t].tipo == 'D'){ // Grava um dado do tipo double.
			x = 0;
			while (x < strlen(auxC->valorCampo))
			{
				if((auxC->valorCampo[x] < 48 || auxC->valorCampo[x] > 57) && (auxC->valorCampo[x] != 46)){ 
					return ERRO_NO_TIPO_DOUBLE;
				}
				x++;
			}

			double valorDouble = convertD(auxC->valorCampo);
			fwrite(&valorDouble,sizeof(valorDouble),1,dados);
		}
		else if(auxT[t].tipo == 'C'){ // Grava um dado do tipo char.

			if(strlen(auxC->valorCampo) > (sizeof(char)))
			{
				return ERRO_NO_TIPO_CHAR;
			}
			char valorChar = auxC->valorCampo[0];
			fwrite(&valorChar,sizeof(valorChar),1,dados);			
		}

	}
	fclose(dados);
	free(c); // Libera a memoria da estrutura.
	free(auxT); // Libera a memoria da estrutura.
	return SUCCESS;
}


//----------------------------------------
// EXCLUIR TUPLA BUFFER
column * excluirTuplaBuffer(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, int nTupla){
	column *colunas = (column *)malloc(sizeof(column)*objeto.qtdCampos);

	if(colunas == NULL)
		return ERRO_DE_ALOCACAO;

	if(buffer[page].nrec == 0) //Essa página não possui registros
		return ERRO_PARAMETRO;
	
	int i, tamTpl = tamTupla(campos, objeto), j=0, t=0;
	i = tamTpl*nTupla; //Calcula onde começa o registro

	while(i < tamTpl*nTupla+tamTpl){
		t=0;
		
		colunas[j].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam); //Aloca a quantidade necessária para cada campo
		colunas[j].tipoCampo = campos[j].tipo;	// Guarda o tipo do campo
		strcpy(colunas[j].nomeCampo, campos[j].nome); 	//Guarda o nome do campo
	
		while(t < campos[j].tam){
			colunas[j].valorCampo[t] = buffer[page].data[i];	//Copia os dados
			t++;
			i++;
		}
		j++;
	}
	j = i;
	i = tamTpl*nTupla;
	for(; i < buffer[page].position; i++, j++) //Desloca os bytes do buffer sobre a tupla excluida
		buffer[page].data[i] = buffer[page].data[j];

	buffer[page].position -= tamTpl;
	buffer[page].nrec--;

	return colunas; //Retorna a tupla excluida do buffer
}
//----------------------------------------
// RETORNA PAGINA DO BUFFER
column * getPage(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page){
	
	if(page > PAGES)
		return ERRO_PAGINA_INVALIDA;

	if(buffer[page].nrec == 0) //Essa página não possui registros
		return ERRO_PARAMETRO;
	
	column *colunas = (column *)malloc(sizeof(column)*objeto.qtdCampos*buffer[page].nrec); //Aloca a quantidade de campos necessária
	
	if(colunas == NULL)
		return ERRO_DE_ALOCACAO;
	
	int i=0, j=0, t=0, h=0;
	
	while(i < buffer[page].position){
		t=0;
		if(j >= objeto.qtdCampos)
			j=0;
		colunas[h].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam);
		colunas[h].tipoCampo = campos[j].tipo;	//Guarda tipo do campo
		strcpy(colunas[h].nomeCampo, campos[j].nome); //Guarda nome do campo
		
		while(t < campos[j].tam){
			colunas[h].valorCampo[t] = buffer[page].data[i]; //Copia os dados
			t++;
			i++;
		}
		h++;
		j++;
	}
	return colunas; //Retorna a 'page' do buffer
}
//----------------------------------------

tp_Object_Schema AbrirTabela(char * nomeTabela){
	
	if(!verificaNomeTabela(nomeTabela)){
		printf("Erro GRAVE! na função leObjeto(). Nome da tabela inválido.\nAbortando...\n");
		exit(1);
	}
	
	tp_Object_Schema Object_Schema;
	struct fs_objects objeto = leObjeto(nomeTabela);
	
	strcpy(Object_Schema.nome, objeto.nome);
	Object_Schema.cod = objeto.cod;
	strcpy(Object_Schema.nArquivo, objeto.nArquivo);
	Object_Schema.qtdCampos = objeto.qtdCampos;
	Object_Schema.Campos = leSchema(objeto);

	return Object_Schema;
}

int atualizaObjeto(char *nTabela){
	FILE *dicionario;
	FILE *auxiliar;
	int cod,qtdCampos,contador=1;
	char nometabela[TAMANHO_NOME_TABELA];
	char nomearquivotabela[TAMANHO_NOME_ARQUIVO];
	
	dicionario = fopen("fs_object.dat", "rwb+"); // Abre o dicionario de dados.

	if(!verificaNomeTabela(nTabela)){
		printf("Erro GRAVE! na função leObjeto(). Nome da tabela inválido.\nAbortando...\n");
		exit(1);
	}

	if (dicionario == NULL)	{
		printf("Erro GRAVE! na função leObjeto(). Arquivo não encontrado.\nAbortando...\n\n");
		exit(1);
	}
	auxiliar = fopen("auxiliar.dat","a+b");
	while(fgetc (dicionario) != EOF){
		memset(nometabela, 0, sizeof (nometabela));
		memset(nomearquivotabela, 0, sizeof (nomearquivotabela));
		fseek(dicionario, -1, 1);
		fread(&nometabela, sizeof(char), TAMANHO_NOME_TABELA , dicionario); //Lê somente o nome da tabela
		fread(&cod,sizeof(int),1,dicionario);	// Copia valores referentes a tabela pesquisada para a estrutura.
		fread(&nomearquivotabela,sizeof(char),TAMANHO_NOME_TABELA,dicionario);
		fread(&qtdCampos,sizeof(int),1,dicionario);
		if(strcmp(nometabela, nTabela) != 0){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
			strcat(nometabela,"\0");
			fwrite(&nometabela,sizeof(nometabela),1,auxiliar);
			fwrite(&contador,sizeof(contador),1,auxiliar);
			contador++;
			strcat(nomearquivotabela,"\0");
			fwrite(&nomearquivotabela,sizeof(nomearquivotabela),1,auxiliar);	
			fwrite(&qtdCampos,sizeof(qtdCampos),1,auxiliar);			
		}
	}
	fclose(auxiliar);
	fclose(dicionario);
	remove("fs_object.dat");
	system("mv auxiliar.dat fs_object.dat");
	
	return 0;
}
int atualizaSchema(int codigo){
	FILE *dicionario;
	FILE *schema;
	FILE *auxiliar;
	int i, cod,contador=1;
	char nometabela[TAMANHO_NOME_TABELA];
	struct fs_objects objeto;
	char tupla[TAMANHO_NOME_CAMPO];
	dicionario = fopen("fs_object.dat", "rwb+"); // Abre o dicionario de dados.
	
	if (dicionario == NULL)	{
		printf("Erro GRAVE! na função leObjeto(). Arquivo não encontrado.\nAbortando...\n\n");
		exit(1);
	}
	auxiliar = fopen("auxiliar.dat", "a+b");
	
	if (auxiliar == NULL){
		exit(1);
	}
	while(fgetc (dicionario) != EOF){
		memset(nometabela, 0, sizeof (nometabela));
		fseek(dicionario, -1, 1);
		fread(&nometabela, sizeof(char), TAMANHO_NOME_TABELA , dicionario); //Lê somente o nome da tabela
		fseek(dicionario,TAMANHO_NOME_ARQUIVO+sizeof(int)+sizeof(int),1);	
		objeto = leObjeto(nometabela);
		
		tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos); // Aloca esquema com a quantidade de campos necessarios.

		if(esquema == NULL)
			return 1;

		schema = fopen("fs_schema.dat", "a+b"); // Abre o arquivo de esquemas de tabelas.

		if (schema == NULL)
			return 1;
		i = 0;
		while((fgetc (schema) != EOF) && (i < objeto.qtdCampos)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
			fseek(schema, -1, 1);
			if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
				if(cod == objeto.cod){ // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.
					
					fread(&tupla, sizeof(char), TAMANHO_NOME_CAMPO, schema);
					strcpy(esquema[i].nome,tupla);					// Copia dados do campo para o esquema.
					fread(&esquema[i].tipo, sizeof(char),1,schema);
					fread(&esquema[i].tam, sizeof(int),1,schema);  
					fread(&esquema[i].pk, sizeof (int), 1, schema);
					fread(&esquema[i].fk, sizeof (int), 1, schema);
					fread(esquema[i].ref, sizeof (char), TAMANHO_NOME_TABELA, schema);
					
					if(cod != codigo){
						fwrite(&contador,sizeof(contador),1,auxiliar);
						fwrite(&tupla,sizeof(tupla),1,auxiliar);
						fwrite(&esquema[i].tipo,sizeof(esquema[i].tipo),1,auxiliar);
						fwrite(&esquema[i].tam,sizeof(esquema[i].tam),1,auxiliar);
						fwrite(&esquema[i].pk,sizeof(esquema[i].pk),1,auxiliar);
						fwrite(&esquema[i].fk,sizeof(esquema[i].fk),1,auxiliar);
						fwrite(esquema[i].ref,TAMANHO_NOME_TABELA,1,auxiliar);
					}
					i++; 
				}
			}
		}
		if(cod != codigo)
			contador++;
		
		free(esquema);
		fclose(schema);	

	}
	fclose(auxiliar);
	fclose(dicionario);
	remove("fs_schema.dat");
	system("mv auxiliar.dat fs_schema.dat");
	return 0;
}

int validaatualizacao(char *nTabela){
	FILE *dicionario;
	int x;
	char nometabela[TAMANHO_NOME_TABELA];
	tp_Object_Schema Object_Schema;
	dicionario = fopen("fs_object.dat", "rwb+"); // Abre o dicionario de dados.
	
	if (dicionario == NULL)	{
		printf("Erro GRAVE! na função leObjeto(). Arquivo não encontrado.\nAbortando...\n\n");
		exit(1);
	}
	
	while(fgetc (dicionario) != EOF){
		memset(nometabela, 0, sizeof (nometabela));
		fseek(dicionario, -1, 1);
		fread(&nometabela, sizeof(char), TAMANHO_NOME_TABELA , dicionario); //Lê somente o nome da tabela
		fseek(dicionario,TAMANHO_NOME_ARQUIVO+sizeof(int)+sizeof(int),1);	
		Object_Schema = AbrirTabela(nometabela);
		
		x=0;
		while (x < Object_Schema.qtdCampos){
			
			if(strcmp(nTabela, Object_Schema.Campos[x].ref)==0)
			return 1;
			x++;
		}	

	}
	fclose(dicionario);
	return SUCCESS;
}

int dropTable(char * nTabela){
	int teste;
	teste = validaatualizacao(nTabela); //verifica se na tabela a ser excluida nao tem ligação de chave estrangeira
	
	if (teste != SUCCESS)
	return ERRO_TABELA_LIGADA;
	
	struct fs_objects objeto = leObjeto(nTabela);
	atualizaSchema(objeto.cod);
	atualizaObjeto(nTabela);
	return SUCCESS;
}

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
	
	while (!feof(data)){
		for (i=EsquemaAcumu=0;i < objeto.qtdCampos; i++){
			fread(BUFFER, sizeof(char), esquema[i].tam, data);
			if (esquema[i].pk){
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
		}
	}
	
	fclose(data);
	if (!flag) 
		return ERRO_A_FK_NAO_REFERENCIA_UMA_TABELA_OU_CHAVE_VALIDA;
	
	return SUCCESS;
}

