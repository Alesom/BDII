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

	t = adicionaCampo(t, "CPF", 'S', 10, 1, 0, NULL);
	t = adicionaCampo(t, "Nome", 'S', 20, 0, 0, NULL);
	t = adicionaCampo(t, "Idade", 'I', (sizeof(int)), 0, 0, NULL);
	t = adicionaCampo(t, "Sexo", 'C', (sizeof(char)), 0, 0, NULL);

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	//##################################################################################
	//#################CRIAÇAO DA TABELA PRODUTO########################################
	
	t = iniciaTabela("Produto");

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}

	t = adicionaCampo(t, "Id", 'I', (sizeof(int)), 1, 0, NULL);
	t = adicionaCampo(t, "Nome", 'S', 20, 0, 0, NULL);
	t = adicionaCampo(t, "Especificacao", 'S', 50, 0, 0, NULL);

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	//##################################################################################
	//#################CRIAÇAO DA TABELA NOTAFISCAL#####################################
	
	t = iniciaTabela("NotaFiscal");

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}

	t = adicionaCampo(t, "Id", 'I', (sizeof(int)), 1, 0, NULL);
	t = adicionaCampo(t, "CPFCliente", 'I', (sizeof(int)), 0, 1, "Cliente");
	t = adicionaCampo(t, "Comentario", 'S', 50, 0, 0, NULL);

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	//##################################################################################
		//#################CRIAÇAO DA TABELA PRODUTOSNOTAFISCAL#########################
	
	t = iniciaTabela("ProdsNota");

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}

	t = adicionaCampo(t, "IdNota", 'I', (sizeof(int)), 1, 1, "NotaFiscal");
	t = adicionaCampo(t, "IdProduto", 'I', (sizeof(int)), 1, 1, "Produto");
	t = adicionaCampo(t, "QtdProduto", 'I', (sizeof(int)), 0, 0, NULL);

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	//##################################################################################
	return 0;
}
int inserirtupla(int tabela){
	int erro,inteiro;
	column *c = NULL;
	char leitor[100];
	system("clear");
	if(tabela==1){
		printf("||Cliente||\n");
		memset(leitor, 0, sizeof (leitor));
		printf("Digite o CPF do cliente:\n");
		scanf("%s",leitor);
		sprintf(leitor, "%s", leitor);
		c = insereValor(c, "CPF", leitor);
		
		memset(leitor, 0, sizeof (leitor));
		printf("Digite o nome do cliente:\n");
		scanf("%s",leitor);
		sprintf(leitor, "%s", leitor);
		c = insereValor(c, "Nome", leitor);
		
		memset(leitor, 0, sizeof (leitor));
		printf("Digite a idade do cliente:\n");
		scanf("%i",&inteiro);
		sprintf(leitor, "%d", inteiro);
		c = insereValor(c, "Idade", leitor);
		
		memset(leitor, 0, sizeof (leitor));
		printf("Sexo do cliente F/M:\n");
		scanf("%s",leitor);
		sprintf(leitor, "%s", leitor);
		c = insereValor(c, "Sexo", leitor);

		erro = finalizaInsert("Cliente", c);
	}else
		if(tabela==2){
			printf("||Produto||\n");
			memset(leitor, 0, sizeof (leitor));
			printf("Digite o Id do produto:\n");
			scanf("%d",&inteiro);
			sprintf(leitor, "%d", inteiro);
			c = insereValor(c, "Id", leitor);
			
			memset(leitor, 0, sizeof (leitor));
			printf("Digite o nome do produto:\n");
			scanf("%s",leitor);
			sprintf(leitor, "%s", leitor);
			c = insereValor(c, "Nome", leitor);
			
			memset(leitor, 0, sizeof (leitor));
			printf("Especificacao do produto:\n");
			scanf("%s",leitor);
			sprintf(leitor, "%s", leitor);
			c = insereValor(c, "Especificacao", leitor);
			
			erro = finalizaInsert("Produto", c);
		}else
			if(tabela==3){
				printf("||NotaFiscal||\n");
				memset(leitor, 0, sizeof (leitor));
				printf("Digite o Id da NotaFiscal:\n");
				scanf("%d",&inteiro);
				sprintf(leitor, "%d", inteiro);
				c = insereValor(c, "Id", leitor);
				
				memset(leitor, 0, sizeof (leitor));
				printf("Digite o CPF do cliente que efetuou a compra:\n");
				scanf("%d",&inteiro);
				sprintf(leitor, "%d", inteiro);
				c = insereValor(c, "IdCliente", leitor);
				
				memset(leitor, 0, sizeof (leitor));
				printf("Comentario sobre a compra:\n");
				scanf("%s",leitor);
				sprintf(leitor, "%s", leitor);
				c = insereValor(c, "Comentario", leitor);
				
				erro = finalizaInsert("NotaFiscal", c);
				}else
					if(tabela==4){
						printf("||ProdsNota||\n");
						memset(leitor, 0, sizeof (leitor));
						printf("Digite o Id que faz referencia a NotaFiscal:\n");
						scanf("%d",&inteiro);
						sprintf(leitor, "%d", inteiro);
						c = insereValor(c, "IdNota", leitor);
						
						memset(leitor, 0, sizeof (leitor));
						printf("Digite o Id do Produto que faz referencia a Produto:\n");
						scanf("%d",&inteiro);
						sprintf(leitor, "%d", inteiro);
						c = insereValor(c, "IdProduto", leitor);
						
						memset(leitor, 0, sizeof (leitor));
						printf("Digite a quantidade de itens:\n");
						scanf("%d",&inteiro);
						sprintf(leitor, "%d", inteiro);
						c = insereValor(c, "QtdProduto", leitor);
						
						erro = finalizaInsert("ProdsNota", c);
						}else
						printf("Tabela nao encontrada!\n");
						
								
		if(erro != SUCCESS)
			printf("Erro %d: na função finalizaInsert()\n", erro);
	return 0;
}
int mostrartabelasetuplas(){
	printf("TABELAS\n");
	int erro,x,y,flag;
	char *tabelas[4];
	tabelas[0] = "Cliente";
	tabelas[1] = "Produto";
	tabelas[2] = "NotaFiscal";
	tabelas[3] = "ProdsNota";
	struct fs_objects objeto[4];
	tp_table *esquema[4];
	tp_buffer *bufferpoll[4];
	column *pagina[4];
	for(x=0;x<4;x++){
		flag=0;
		esquema[x]=NULL;
		bufferpoll[x]=NULL;
		pagina[x]=NULL;
		printf("%s\n",tabelas[x]);
		objeto[x] = leObjeto(tabelas[x]);	
		
		esquema[x] = leSchema(objeto[x]);
	
		if(esquema[x] == ERRO_ABRIR_ESQUEMA){
			printf("Erro ao criar o esquema.\n");
			return 0;
		}

		bufferpoll[x] = initbuffer();

		if(bufferpoll[x] == ERRO_DE_ALOCACAO){
			printf("Erro ao alocar memória para o buffer.\n");
			return 0;
		}
		y=0;
		while(y>=0){
			erro = colocaTuplaBuffer(bufferpoll[x], y, esquema[x], objeto[x]);
			if(erro != SUCCESS){
				flag++;
				y=-2;
				break;
				//printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
				//return 0;
			}else
			y++;
		}
		if(y>=0){
			pagina[x] = getPage(bufferpoll[x], esquema[x], objeto[x], x);
			if(pagina[x] == ERRO_PARAMETRO){
				printf("Erro, na função getPage(), problemas no parametro.\n");
				//return 0;
			}
		}
	
		if(flag!=0){
			erro = printbufferpoll(bufferpoll[x], esquema[x], objeto[x], 0);
			if(erro != SUCCESS){
				printf("Tabela vazia!!\n");
				//printf("Erro %d: na função printbufferpoll().\n", erro);
				//return 0;
			}
		}else
		printf("Tabela vazia!\n");
	}
	return 0;
}
int main(){
	int x=0,tabela;
	char excluir;
	criatabelas();
	while(x!=4){
		system("clear");
		printf("MENU\n");
		printf("1-Inserir tupla\n");
		printf("2-Mostrar tabelas\n");
		printf("3-Deletar o Banco de Dados e sair\n");
		printf("4-Sair\n");
		scanf("%d",&x);
		getchar();
		if(x == 1){
			system("clear");
			printf("(1)Cliente  ||  (2)Produto  ||  (3)NotaFiscal  ||  (4)ProdsNota\n");
			printf("Entre com o numero da tabela na qual deseja adicionar: ");
			scanf("%d",&tabela);
			inserirtupla(tabela);
			getchar();
		}else if(x == 2){
			system("clear");
			mostrartabelasetuplas();
			printf("Pressione uma tecla para voltar ao MENU\n");
			getchar();
		}else if (x==3){
			printf("Tem certeza que deseja excluir o banco? [s/n]\n");
			scanf("%c", &excluir);
			getchar();
			if (excluir=='s'){
				system("rm -f *.dat");
				x=4;
			}
			else
			x=0;
		}
	}
	printf("\n");
	
	return 0;
}
