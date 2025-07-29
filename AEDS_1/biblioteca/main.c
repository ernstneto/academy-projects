#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "buscar.h"
#include "inserir.h"
#include "remover.h"
#include "administrador.h"
#include "alterar.h"

void espaco_alunos(MYSQL *conn){
	int aluno = 1;
	while(aluno == 1){
		printf("espaco alunos\n\n");
		printf("Insira a opcao desejada no espaco alunos:\n\n");
		printf("Digite 1 para inserir novo aluno\n");
		printf("Digite 2 para excluir aluno\n");
		printf("Digite 3 para fazer busca de alunos\n");
		printf("Digite 4 para alterar dados do aluno\n");
		printf("Digite 0 para retornar ao menu da biblioteca\n\n");
		printf("Opcao: ");
		int opcao;
		scanf("%d",&opcao);
		//todas as etapas e preciso acesso do administrador para ter o acesso espaco aluno
		if(opcao == 1){
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				inserir_aluno(conn);
			}
			//caso contrario, falha no login ou incorreto
			else{
				printf("Inserir aluno abortado!\n");
			}
		}
		else if(opcao == 2){
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				remover_aluno(conn);
			}
			//caso contrario, falha no login ou incorreto
			else{
				printf("Remover aluno abortado!\n");
			}
		}
		else if(opcao == 3){
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Deseja ver todos os alunos, digite 1\n");
				printf("deseja ver um aluno, digite 2\n");
				int opcao_buscar;
				printf("Opcao: ");
				scanf("%d",&opcao_buscar);
				if(opcao_buscar == 1){
					buscar_todos_alunos(conn);
				}
				else if(opcao_buscar == 2){
					char matricula[15];
					printf("matricula: ");
					scanf("%s", matricula);
					int res = buscar_aluno(conn,matricula);
				}
			}
			//caso contrario, falha no login ou incorreto
			else{
				printf("Buscar aluno abortado!\n");
			}			
		}
		else if(opcao == 4){
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				alterar_aluno(conn);
			}
			//caso contrario, falha no login
			else{
				printf("Alterar aluno abortado!\n");
			}
		}
		else if(opcao == 0){
			printf("Voltando ao menu biblioteca\n");
			aluno = 0;
		}
		else{
			printf("Comando invalido!\n");
		}
	}
}

void espaco_livros(MYSQL *conn){
	int livro = 1;
	while(livro){
		printf("espaco livros\n");
		printf("Digite a opcao desejada:\n\n");
		printf("Digite 1 para inserir novo livro\n");
		printf("Digite 2 para remover um livro\n");
		printf("Digite 3 para buscar um livro\n");
		printf("Digite 4 para alterar estado do livro\n");
		printf("Digite 0 para voltar ao menu da biblioteca\n\n");
		int opcao;
		printf("Opcao: ");
		scanf("%d",&opcao);
		//todas as etapas e preciso acesso do administrador para ter o acesso espaco livro, exceto a busca por categoria
		if(opcao == 1){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				inserir_livro(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel inserir novo livro!\n");
			}
		}
		else if(opcao == 2){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				remover_livro(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel remover livro!\n");
			}
		}
		else if(opcao == 3){
			int loop = 1;
			int resposta = etapa_acesso_administrador(conn);
			if(resposta == 0){
				while(loop){
					printf("Digite a opcao desejada:\n\n");
					printf("Digite 1 para fazer busca por ID do livro.\n");
					printf("Digite 2 para fazer busca por categoria dos livros.\n");
					printf("Digite 3 para fazer busca de todos os livros.\n");
					printf("Digite 0 para voltar ao espaco livro.\n");
					int opcao_livro;
					printf("Opcao: ");
					scanf("%d",&opcao_livro);
					if(opcao_livro == 1){
						printf("busca por administrador, digite 1\n");
						printf("busca por usuario comum, digite 2\n");
						int resp;
						printf("Opcao: ");
						scanf("%d",&resp);
						if(resp == 1){
							int resposta = etapa_acesso_administrador(conn);
							//Caso 0, a resposta de login correto
							if(resposta == 0){
								printf("Login com sucesso!\n");
								char id_livro[100];
								printf("ID do livro: ");
								scanf("%s", id_livro);
								int res = buscar_livro(conn,id_livro, "id_livro");
							}
							//caso contrario, a resposta de login incorreto ou falha
							else{
								printf("Nao é possivel remover livro!\n");
							}
						}
						else if(resp == 2){
							char id_livro[100];
							printf("ID livro: ");
							scanf("%s",id_livro);
							int res = buscar_livro_2(conn,id_livro, "id_livro");
						}
						else{
							printf("comando invalido!\n");
						}
					}
					else if(opcao_livro == 2){
						char categoria[100];
						printf("Categoria: ");
						scanf("%s", categoria);
						buscar_livro(conn,categoria, "categoria");
					}
					else if(opcao_livro == 3){
						buscar_todos_livros(conn);
					}
					else if(opcao_livro == 0){
						loop = 0;
					}
					else{
						printf("Comando invalido!\n");
					}
				}
			}
			//caso contrario, falha no login
			else{
				printf("Busca de livros abortado!\n");
			}
		}
		else if(opcao == 4){
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				alterar_livro(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel alterar livro!\n");
			}
		}
		else if(opcao == 0){
			printf("voltando ao menu biblioteca\n");
			//livro = 0;
			return;
		}
		else{
			printf("Comando invalido, tente novamente!\n");
		}
	}
}

void espaco_salas(MYSQL *conn){
	int sala = 1;
	while(sala){
		printf("SALAS:\n");
		printf("Digite a opcao desejada:\n");
		printf("Digite 1 para inserir uma sala nova\n");
		printf("Digite 2 para remover uma sala\n");
		printf("Digite 3 para buscar a sala\n");
		printf("Digite 4 para alterar o estado da sala\n");
		printf("Digite 0 para voltar ao menu da biblioteca\n\n");
		printf("Opcao: ");
		int opcao;
		scanf("%d",&opcao);
		//todas as etapas e preciso acesso do administrador para ter o acesso espaco sala
		if(opcao == 1){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				inserir_sala(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel inserir nova sala!\n");
			}
		}
		else if (opcao == 2){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				remover_sala(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel remover sala!\n");
			}
		}
		else if(opcao == 3){
			int resposta = etapa_acesso_administrador(conn);
			if(resposta == 0){
				printf("Digite 1 para fazer busca de todas as salas.\n");
				printf("Digite 2 para fazer busca de id da sala.\n");
				printf("Opcao: ");
				int opcao_2;
				scanf("%d",&opcao_2);
				if(opcao_2 == 1){
					buscar_todos_salas(conn);
				}
				else if(opcao_2 == 2){
					char id_sala[100];
					printf("ID sala: ");
					scanf("%s",id_sala);
					//printf("%s\n",id_sala);
					int res = buscar_sala(conn,id_sala);
				}
			}
			//caso contrario, falha no login ou incorreto
			else{
				printf("Busca de sala abortada!\n");
			}
		}
		else if(opcao == 4){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				alterar_sala(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel alterar sala!\n");
			}
		}
		else if(opcao == 0){
			printf("Retornando ao menu da biblioteca!\n");
			//sala = 0;
			return;
		}
		else{
			printf("Comando invalido!Tente novamente!\n");
		}
	}
}

void espaco_armarios(MYSQL *conn){
	int armario = 1;
	while(armario){
		printf("ARMARIO:\n");
		printf("Digite a opcao desejada:\n");
		printf("Digite 1 para inserir um armario novo\n");
		printf("Digite 2 para remover um armario\n");
		printf("Digite 3 para buscar um armario\n");
		printf("Digite 4 para alterar um armario\n");
		printf("Digite 0 para voltar ao menu da biblioteca\n\n");
		printf("Opcao: ");
		int opcao;
		scanf("%d",&opcao);
		//todas as etapas e preciso acesso do administrador para ter o acesso espaco armario
		if(opcao == 1){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				inserir_armario(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel inserir novo armario!\n");
			}
		}
		else if (opcao == 2){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				remover_armario(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel remover armario!\n");
			}
		}
		else if(opcao == 3){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				printf("Digite 1 para buscar todos os armarios.\n");
				printf("Digite 2 para buscar por id do armario.\n");
				printf("Opcao: ");
				int opcao_2;
				scanf("%d",&opcao_2);
				if(opcao_2 == 1){
					buscar_todos_armarios(conn);
				}
				else if(opcao_2 == 2){
					char id_sala[100];
					printf("id sala: ");
					scanf("%s",id_sala);
					int res = buscar_sala(conn,id_sala);
				}
				else{
					printf("Comando invalido.\n");
				}
			}
			//caso contrario, falha no login ou incorreto
			else{
				printf("Acesso administrador foi abortado!\n");
			}
		}
		else if(opcao == 4){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				alterar_armario(conn);
			}
		}
		else if(opcao == 0){
			printf("Retornando ao menu da biblioteca!\n");
			//armario = 0;
			return;
		}
		else{
			printf("Comando invalido!Tente novamente!\n");
		}
	}
}

void espaco_computadores(MYSQL *conn){
	int computador = 1;
	while(computador){
		printf("COMPUTADOR:\n");
		printf("Digite a opcao desejada:\n");
		printf("Digite 1 para inserir um computador novo\n");
		printf("Digite 2 para remover um computador\n");
		printf("Digite 3 para buscar um computador\n");
		printf("Digite 4 para alterar o estado do computador\n");
		printf("Digite 0 para voltar ao menu da biblioteca\n\n");
		printf("Opcao: ");
		int opcao;
		scanf("%d",&opcao);
		//todas as etapas e preciso acesso do administrador para ter o acesso espaco computador
		if(opcao == 1){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				inserir_computador(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel inserir novo computador!\n");
			}
		}
		else if (opcao == 2){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				remover_computador(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao foi possivel remover computador!\n");
			}
		}
		else if(opcao == 3){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){	
				printf("Login com sucesso!\n");
				printf("Digite 1 para buscar todos os armarios.\n");
				printf("Digite 2 para buscar por id do armario.\n");
				printf("Opcao: ");
				int opcao_2;
				scanf("%d",&opcao_2);
				if(opcao_2 == 1){
					buscar_todos_computadores(conn);
				}
				else if(opcao_2 == 2){
					printf("ID do computador: ");
					char id_computador[100];
					scanf("%s",id_computador);
					int res = buscar_computador(conn,id_computador);
				}
				else{
					printf("Comando invalido.\n");
				}
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Nao é possivel buscar computador!\n");
			}
		}
		else if(opcao == 4){
			//etapa de login
			int resposta = etapa_acesso_administrador(conn);
			//Caso 0, a resposta de login correto
			if(resposta == 0){
				printf("Login com sucesso!\n");
				alterar_computador(conn);
			}
			//caso contrario, a resposta de login incorreto ou falha
			else{
				printf("Falha de acesso ao administrador!\n");
			}
		}
		else if(opcao == 0){
			printf("Retornando ao menu da biblioteca!\n");
			//computador = 0;
			return;
		}
		else{
			printf("Comando invalido!Tente novamente!\n");
		}
	}

}

int main(int argc, char *argv[]){
	MYSQL conn;
	mysql_init(&conn);
	int opcao;
	//menu principal da biblioteca
	printf("BEM VINDO A BIBLIOTECA DO CEFET/RJ UNED PETROPOLIS!!\n");
	while(1){
		printf("Escolha a opcao desejada:\n\n");
		printf("Digite 1 para alunos\n");
		printf("Digite 2 para livros\n");
		printf("Digite 3 para salas\n");
		printf("Digite 4 para armarios\n");
		printf("Digite 5 para computadores\n");
		printf("Digite 0 para sair do programa\n");
		int opcao;
		printf("\nOpcao: ");
		scanf("%d",&opcao);
		//acesso ao espaco aluno
		if(opcao == 1){
			espaco_alunos(&conn);
		}
		//acesso ao espaco livro
		else if(opcao == 2){
			espaco_livros(&conn);
		}
		//acesso ao espaco sala
		else if(opcao == 3){
			espaco_salas(&conn);
		}
		//acesso ao espaco armario
		else if(opcao == 4){
			espaco_armarios(&conn);
		}
		//acesso ao espaco computador
		else if(opcao == 5){
			espaco_computadores(&conn);
		}
		//fechar o sistema da biblioteca e finalizar do programa
		else if(opcao == 0){
			break;
		}
		//comando invalido ou fora dos intervalos
		else{
			printf("opcao invalida!Tente novamente!\n\n");
		}
	}
	printf("Fechando a biblioteca!\n");
	mysql_close(&conn);
	return EXIT_SUCCESS;
}
