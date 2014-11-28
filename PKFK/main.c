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

	t = adicionaCampo(t, "CPF", 'S', 10, 1, 0);
	t = adicionaCampo(t, "Nome", 'S', 20, 0, 0);
	t = adicionaCampo(t, "Idade", 'I', (sizeof(int)), 0, 0);
	t = adicionaCampo(t, "Sexo", 'C', (sizeof(char)), 0, 0);

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

	t = adicionaCampo(t, "Id", 'I', (sizeof(int)), 1, 0);
	t = adicionaCampo(t, "Nome", 'S', 20, 0, 0);
	t = adicionaCampo(t, "Especificacao", 'S', 50, 0, 0);

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

	t = adicionaCampo(t, "Id", 'I', (sizeof(int)), 1, 0);
	t = adicionaCampo(t, "CPFCliente", 'I', (sizeof(int)), 0, 1);
	t = adicionaCampo(t, "Comentario", 'S', 50, 0, 0);

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

	t = adicionaCampo(t, "IdNota", 'I', (sizeof(int)), 1, 1);
	t = adicionaCampo(t, "IdProduto", 'I', (sizeof(int)), 1, 1);
	t = adicionaCampo(t, "QtdProduto", 'I', (sizeof(int)), 0, 0);

	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	//##################################################################################
	return 0;
}
int inserirtupla(int tabela){
	int erro;
	column *c = NULL;
	char leitor[200];
	system("clear");
	if(tabela==1){
		printf("||Cliente||\n");
		memset(leitor, 0, sizeof (leitor));
		printf("Digite o CPF do cliente:\n");
		scanf("%s",leitor);
		c = insereValor(c, "CPF", leitor);
		
		memset(leitor, 0, sizeof (leitor));
		printf("Digite o nome do cliente:\n");
		scanf("%s",leitor);
		c = insereValor(c, "Nome", leitor);
		
		memset(leitor, 0, sizeof (leitor));
		printf("Digite a idade do cliente:\n");
		scanf("%s",leitor);
		c = insereValor(c, "Idade", leitor);
		
		memset(leitor, 0, sizeof (leitor));
		printf("Sexo do cliente F/M:\n");
		scanf("%s",leitor);
		c = insereValor(c, "Sexo", leitor);

		erro = finalizaInsert("Cliente", c);
		
		if(erro != SUCCESS){
			printf("Erro %d: na função finalizaInsert()\n", erro);
			return 0;
		}
	}else
		if(tabela==2){
			printf("||Produto||\n");
			memset(leitor, 0, sizeof (leitor));
			printf("Digite o Id do produto:\n");
			scanf("%s",leitor);
			c = insereValor(c, "Id", leitor);
			
			memset(leitor, 0, sizeof (leitor));
			printf("Digite o nome do produto:\n");
			scanf("%s",leitor);
			c = insereValor(c, "Nome", leitor);
			
			memset(leitor, 0, sizeof (leitor));
			printf("Especificacao do produto:\n");
			scanf("%s",leitor);
			c = insereValor(c, "Especificacao", leitor);
			
			erro = finalizaInsert("Produto", c);
			
			if(erro != SUCCESS){
				printf("Erro %d: na função finalizaInsert()\n", erro);
				return 0;
			}
		}else
			if(tabela==3){
				printf("||NotaFiscal||\n");
				memset(leitor, 0, sizeof (leitor));
				printf("Digite o Id da NotaFiscal:\n");
				scanf("%s",leitor);
				c = insereValor(c, "Id", leitor);
				
				memset(leitor, 0, sizeof (leitor));
				printf("Digite o CPF do cliente que efetuou a compra:\n");
				scanf("%s",leitor);
				c = insereValor(c, "IdCliente", leitor);
				
				memset(leitor, 0, sizeof (leitor));
				printf("Comentario sobre a compra:\n");
				scanf("%s",leitor);
				c = insereValor(c, "Comentario", leitor);
				
				erro = finalizaInsert("NotaFiscal", c);
				
				if(erro != SUCCESS){
					printf("Erro %d: na função finalizaInsert()\n", erro);
					return 0;
				}
				}else
					if(tabela==4){
						printf("||ProdsNota||\n");
						memset(leitor, 0, sizeof (leitor));
						printf("Digite o Id que faz referencia a NotaFiscal:\n");
						scanf("%s",leitor);
						c = insereValor(c, "IdNota", leitor);
						
						memset(leitor, 0, sizeof (leitor));
						printf("Digite o Id do Produto que faz referencia a Produto:\n");
						scanf("%s",leitor);
						c = insereValor(c, "IdProduto", leitor);
						
						memset(leitor, 0, sizeof (leitor));
						printf("Digite a quantidade de itens:\n");
						scanf("%s",leitor);
						c = insereValor(c, "QtdProduto", leitor);
						
						erro = finalizaInsert("ProdsNota", c);
						
						if(erro != SUCCESS){
							printf("Erro %d: na função finalizaInsert()\n", erro);
							return 0;
						}
						}else
						printf("Tabela nao encontrada!\n");
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
