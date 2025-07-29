#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_MSG_SIZE 100
#define MSG_TYPE 1

struct message {
    long mtype;
    char mtext[MAX_MSG_SIZE];
};

int sem_id; // identificador do semaforo

// Variaveis Compartilhadas

int *memoria_compartilhada1;
int *memoria_compartilhada2;

// Define a operacao semaforo
void sem_operate(int sem_num, int sem_op) {
    struct sembuf sem_buf;
    sem_buf.sem_num = sem_num;
    sem_buf.sem_op = sem_op;
    sem_buf.sem_flg = 0;
    semop(sem_id, &sem_buf, 1);
}

// Funcao que executa a tarefa do processo
void executarTarefaProcesso(int pid){
    for(int i = 1; i <= 30; i++){
        sem_operate(0, -1); // Aguarda o semaforo
        int aux = *memoria_compartilhada1;
        aux--;
        sleep((200 + (rand() % 1801)) / 1000.0);
        *memoria_compartilhada1 = aux;
        (*memoria_compartilhada2)++;
        printf("PID: %d Iteracao: %d v1: %d v2: %d\n", pid, i, *memoria_compartilhada1, *memoria_compartilhada2);
        sem_operate(0, 1); // Libera o semaforo
        sleep((200 + (rand() % 1801)) / 1000.0);
    }
}

// Funcao da Tarefa a ser executada pela thread
void * executarTarefaThread(){
    long tid = pthread_self();
    for(int i = 1; i <= 30; i++){
        sem_operate(0, -1); // Aguarda o semaforo
        int aux = *memoria_compartilhada1;
        aux--;
        sleep((200 + (rand() % 1801)) / 1000.0);
        *memoria_compartilhada1 = aux;
        (*memoria_compartilhada2)++;
        printf("TID: %ld Iteracao: %d v1: %d v2: %d\n", tid, i, *memoria_compartilhada1, *memoria_compartilhada2);
        sem_operate(0, 1); // Libera o semaforo
        sleep((200 + (rand() % 1801)) / 1000.0);
    }
    printf("Thread TID: %ld finalizou\n", tid);
    return NULL;
}

int main(){

    // Criacao do semaforo e configurcao de troca de mensagens

    key_t sem_key = ftok(".", 'S');
    sem_id = semget(sem_key, 1, IPC_CREAT | 0666);

    key_t key;
    int msgid;
    struct message msg;

    // Cria a chave para a fila de mensagens
    key = ftok(".", 'm');
    
    // Cria a fila de mensagens
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Inicia o semaforo

    semctl(sem_id, 0, SETVAL, 1);

    int shm_id;
    shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    memoria_compartilhada1 = shmat(shm_id, NULL, 0);
    shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    memoria_compartilhada2 = shmat(shm_id, NULL, 0);

    // Inicia as memorias compartilhadas

    *memoria_compartilhada1 = 120;
    *memoria_compartilhada2 = 0;
    
    pthread_t taskA, taskB;

    for(int i = 1; i <= 3; i++){
        int pid = fork();

        if(pid == 0){
            
            printf("Processo %d iniciou as atividades\n", getpid());
            
            if(i == 1 || i == 2){
                executarTarefaProcesso(getpid());
            }else{
                // Criacao das threads
                pthread_create(&taskA, NULL, executarTarefaThread, NULL);
                pthread_create(&taskB, NULL, executarTarefaThread, NULL);
                // Execuacao das threads
                pthread_join(taskA, NULL);
                pthread_join(taskB, NULL);
            }

            msg.mtype = MSG_TYPE;
            sprintf(msg.mtext, "O processo %d finalizou", getpid());
            msgsnd(msgid, &msg, sizeof(struct message), getpid());

            return 0;
        }
    }

    // Aguarda as mensagens dos processos

    for (int i = 1; i <= 3; i++) { 
        msgrcv(msgid, &msg, sizeof(struct message), MSG_TYPE, 0);
        printf("Mensagem recebida: %s\n", msg.mtext);
    }
    
    // Exclui fila de troca de mensagens

    msgctl(msgid, IPC_RMID, NULL);

    // Desabilita o semaforo

    semctl(sem_id, 0, IPC_RMID);

    return 0;
}