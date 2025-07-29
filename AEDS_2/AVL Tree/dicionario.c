90% de armazenamento usado … Caso atinja o limite de armazenamento, não será possível criar, editar ou fazer upload de arquivos. Aproveite 100 GB de armazenamento por R$ 6,99 R$ 1,69 por mês, durante 3 meses.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>

//lista duplamente encadeada
struct lista{
	char nome[105];
	char tipo[5];
	int inicio;
	int pos;
	int total_block;
	struct lista *prox;
	struct lista *ant;
};


int verificar_espaco(struct lista *cab){
	struct lista *p = cab->prox;
	int total_block = 0;
	while(p!=NULL){
		//printf("nome: %s\n",p->nome);
		total_block += 1;
		p = p->prox;
	}
	return total_block;
}

void armazenar(struct lista *cab,char nome[100], char tipo[5]){
	struct lista *p = cab->prox;
	struct lista *ant = cab;
	int inicio = 0;
	while(p != NULL){
		inicio += 1;
		ant = p;
		p = p->prox;
	}
	//printf("ok\n");
	if(strcmp(tipo,"txt")==0){
		//printf("ok txt\n");
		char arquivo[105];
		strcat(arquivo,nome);
		strcat(arquivo,".");
		strcat(arquivo,tipo);
		//printf("arquivo: %s\n",arquivo);
		//int t = inicio;
		for(int i = 1; i <= 5; i++){
			p = (struct lista*)malloc(sizeof(struct lista));
			strcpy(p->nome,arquivo);
			strcpy(p->tipo,tipo);
			p->inicio = inicio;
			p->pos = i;
			p->total_block = 5;
			p->prox = NULL;
			p->ant = ant;
			//printf("p->nome: %s p->tipo: %s p->pos: %d p->total_block: %d\n",p->nome,p->tipo,p->pos,p->total_block);
			ant->prox = p;
			ant = p;
			if(i < 5){
				p = p->prox;
			}
		}
		//printf("ok txt final\n");
	}
	else if(strcmp(tipo,"doc")==0){
		//printf("ok doc\n");
		char arquivo[105];
		strcat(arquivo,nome);
		strcat(arquivo,".");
		strcat(arquivo,tipo);
		//printf("arquivo: %s\n",arquivo);
		for(int i = 1; i <= 10; i++){
			p = (struct lista*)malloc(sizeof(struct lista));
			strcpy(p->nome,arquivo);
			strcpy(p->tipo,tipo);
			p->inicio = inicio;
			p->pos = i;
			p->total_block = 10;
			p->prox = NULL;
			p->ant = ant;
			//printf("p->nome: %s p->tipo: %s p->pos: %d p->total_block: %d\n",p->nome,p->tipo,p->pos,p->total_block);
			ant->prox = p;
			ant = p;
			if(i < 10)
				p = p->prox;
		}
		//printf("ok doc final\n");		
	}
	else if(strcmp(tipo,"pdf")==0){
		//printf("ok pdf\n");
		char arquivo[105];
		strcat(arquivo,nome);
		strcat(arquivo,".");
		strcat(arquivo,tipo);
		//printf("arquivo: %s\n",arquivo);
		for(int i = 1; i <= 15; i++){
			p = (struct lista*)malloc(sizeof(struct lista));
			strcpy(p->nome,arquivo);
			strcpy(p->tipo,tipo);
			p->inicio = inicio;
			p->pos = i;
			p->total_block = 15;
			p->prox = NULL;
			p->ant = ant;
			//printf("p->nome: %s p->tipo: %s p->pos: %d p->total_block: %d\n",p->nome,p->tipo,p->pos,p->total_block);
			ant->prox = p;
			ant = p;
			if(i < 15)
				p = p->prox;
		}
		//printf("ok pdf final\n");
	}
	printf("nome: %s tipo: %s total block: %d\n",p->nome,p->tipo,p->total_block);
}

struct lista *buscar_existente(struct lista *cab, char arquivo[105]){
	struct lista *p = cab->prox;
	while(p != NULL){
		//printf("nome: %s\n",p->nome);
		if(strcmp(p->nome,arquivo)==0){
			return p;
		}
		p = p->prox;
	}
	return NULL;
}

void inserir_file(struct lista *cab,int tam){
	int block_txt = 5,block_doc = 10, block_pdf = 15;
	//limitar os arquivos e saber o espaco disponivel
	char nome[100];
	char tipo[5];
	int criar = 1;
	int espaco_ocupado = verificar_espaco(cab);
	int espaco_livre = tam - espaco_ocupado;
	printf("Espaco total: %d\nEspaco livre: %d\nEspaco ocupado: %d\n\n",tam,espaco_livre,espaco_ocupado);
	if(espaco_livre < 5){
		printf("Espaco livre indisponivel para o armazenamento de qualquer arquivo permitido!\n");
		return;
	}
	while(criar){
		printf("Digite o nome do arquivo: ");
		scanf(" %s%*c",nome);
		printf("nome: %s\n",nome);
		int tipo_de_arquivo = 1;
		while(tipo_de_arquivo){
			printf("qual tipo de arquivo vc deseja inserir: pdf,doc,txt?\n");
			printf("Opcao: ");
			scanf(" %s%*c",tipo);
			printf("tipo: %s\n",tipo);
			if((strcmp(tipo,"pdf")==0 && espaco_livre >= 15) || (strcmp(tipo,"doc")==0 && espaco_livre >= 10) || (strcmp(tipo,"txt")==0 && espaco_livre >= 5)){
				tipo_de_arquivo = 0;
			}
		}
		char arquivo[105];
		strcat(arquivo,nome);
		strcat(arquivo,".");
		strcat(arquivo,tipo);
		struct lista *p = buscar_existente(cab,arquivo);
		if(p != NULL){
			printf("arquivo existe no sistema! abortando o processo!\n");
			return;
		}
		else if(p == NULL){
			if((strcmp(tipo,"pdf")==0 && espaco_livre < 15) || (strcmp(tipo,"doc")==0 && espaco_livre < 10) || (strcmp(tipo,"txt")==0 && espaco_livre < 5)){
				printf("Espaco indisponivel para o armazenamento!\n");
				return;
			}
			else if(strcmp(tipo,"txt")==0){
			printf("arquivo txt aloca espaco de %d de blocos no espaco\n",block_txt);
			}
			else if(strcmp(tipo,"doc")==0){
				printf("arquivo txt aloca espaco de %d de blocos no espaco\n",block_doc);
			}
			else if(strcmp(tipo,"pdf")==0){
				printf("arquivo txt aloca espaco de %d de blocos no espaco\n",block_pdf);
			}	
		}
		int gravar = 1;
		while(gravar){
			printf("Deseja continuar com essas configuracoes?\n1-sim\n2-nao\n\n");
			int op_armazenar;
			printf("Opcao: ");
			scanf("%d",&op_armazenar);
			if(op_armazenar==1){
				armazenar(cab,nome,tipo);
				printf("armazenado com sucesso!\n");
				criar = 0;
				gravar = 0;
				//return;
			}
			else if(op_armazenar==2)
				inserir_file(cab,tam);
				//return;
			else{
				printf("comando invalido!\n");
			}
		}
	}
}

void inserir_main(struct lista *cab,int tam){
	while(1){
		printf("----menu de inserir----\n");
		printf("Digite a opcao desejada\n1-inserir novo arquivo\n2-verificar espaco disponivel\n0-voltar ao menu principal\n");
		int op_inserir;
		printf("Opcao: ");
		scanf("%d",&op_inserir);
		if(op_inserir==0){
			break;
		}
		else if(op_inserir==1){
			inserir_file(cab,tam);
		}
		else if(op_inserir==2){
			int espaco_ocupado = verificar_espaco(cab);
			int espaco_livre = tam - espaco_ocupado;
			printf("Espaco total: %d\nEspaco ocupado: %d\nEspaco livre: %d\n",tam,espaco_ocupado,espaco_livre);
		}
		else{
			printf("comando invalido\n");
		}
	}
	printf("voltando ao menu principal\n");
}

struct lista *buscar_um_arquivo(struct lista *cab,char arquivo[105]){
	struct lista *p = cab->prox;
	while(p != NULL){
		if(strcmp(p->nome,arquivo)==0){
			return p;
		}
		p = p->prox;
	}
	return NULL;
}

void buscar_todos_arquivos(struct lista *cab){
	struct lista *p = cab->prox;
	//struct lista *ant = cab;
	int total_arquivos = 0;
	if(p == NULL){
		printf("Nao contem nenhum arquivo!\n");
		return;
	}
	while(p != NULL){
		if(strcmp(p->nome,p->ant->nome)!=0){
			total_arquivos += 1;
			printf("arquivo: %s\n",p->nome);
		}
		//ant = p;
		p = p->prox;
	}
	printf("total de arquivos: %d\n",total_arquivos);
}

void buscar_main(struct lista *cab){
	while(1){
		printf("===menu de busca===\n");
		printf("Digite a opcao desejada:\n1-buscar um arquivo\n2-fazer a buscar de todos os arquivos\n0-sair\n");
		int op_buscar;
		printf("Opcao: ");
		scanf("%d",&op_buscar);
		if(op_buscar == 0){
			break;
		}
		else if(op_buscar == 1){
			printf("Qual nome do arquivo deseja procurar: ");
			char arquivo [105];
			scanf(" %s%*c",arquivo);
			//struct lista *ant;
			struct lista *p = buscar_um_arquivo(cab,arquivo);
			if(p == NULL){
				printf("O arquivo %s nao foi encontrado no sistema!\n",arquivo);
			}
			else if(p != NULL){
				printf("arquivo encontrado no sistema!\n");
				printf("nome do arquivo: %s\ntipo: %s\ntotal de blocos ocupados: %d\nposicao do primeiro arquivo: %d\nposicao do endereco do primeiro arquivo: %p\n",p->nome,p->tipo,p->total_block,p->inicio,p);
			}
		}
		else if(op_buscar == 2){
			buscar_todos_arquivos(cab);
		}
	}
	printf("voltando ao menu principal\n");
}

void apagar_arquivo(struct lista *p, char arquivo[105]){
	printf("ok\n");
	struct lista *ant = p->ant;
	struct lista *dep = p->prox;
	printf("ok\n");
	while(strcmp(p->nome,arquivo)==0){
		printf("ok1\n");
		struct lista *temp = p;
		p = p->prox;
		p->ant = ant;
		ant->prox = p;
		dep = p;
		free(temp);
	}
	ant->prox = dep;
	dep->ant = ant;
}

void remover_arquivo(struct lista *cab,char nome[105]){
	//struct lista *ant;
	struct lista *p = buscar_um_arquivo(cab,nome);
	if(p == NULL){
		printf("Arquivo %s nao encontrado\n",nome);
		return;
	}
	else if(p != NULL){
		printf("arquivo %s encontrado!\n",p->nome);
		char arquivo[105];
		strcpy(arquivo,p->nome);
		apagar_arquivo(p,arquivo);
	}
}

void remover_main(struct lista *cab){
	while(1){
		printf("===menu de remocao===\n");
		printf("Digite a opcao desejada:\n1-remover um arquivo\n0-sair\n");
		int op_remover;
		printf("Opcao: ");
		scanf("%d",&op_remover); 
		if(op_remover==0){
			break;
		}
		else if(op_remover == 1){
			printf("digite o nome do arquivo a remover: ");
			char arquivo[105];
			scanf(" %s%*c",arquivo);
			remover_arquivo(cab,arquivo);
		}
		else{
			printf("comando invalido!\n");
		}
	}
}

int main(){
	int tam = 100;
	struct lista *cab = (struct lista*)malloc(sizeof(struct lista));
	cab->prox = NULL;
	cab->ant = cab;
	//cab->ant = NULL;
	//printf("%p\n",cab);
	while(1){
		printf("===MENU PRINCIPAL===\n");
		printf("Digite a opcao desejada:\n1-inserir\n2-buscar\n3-remover\n0-sair\n");
		int op_main;
		printf("Opcao: ");
		scanf("%d",&op_main);
		if(op_main==0){
			break;
		}
		else if(op_main==1){
			inserir_main(cab,tam);
		}
		else if(op_main==2){
			buscar_main(cab);
		}
		else if(op_main==3){
			remover_main(cab);
		}
		else{
			printf("comando invalido!\n");
		}
	}
	printf("Fechando programa com sucesso!\n");
	free(cab);
	return EXIT_SUCCESS;
}
