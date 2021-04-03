#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#define MAX 20
#define KEY 255

// Avec processus fils et pere
// Autre test pour processus differents

void processus(int sem){
    // Creation semaphore
    int index;
    struct sembuf p = {0,-1,SEM_UNDO};
    struct sembuf v = {0,+1,SEM_UNDO};
    char * string = malloc(sizeof(char)*20);
    index = 0;
    while(index<MAX){
        //attente semaphore
        // Met process en attente si sem pas libre
        printf("Check semaphore\n");
        if(semop(sem,&p,1)<0){
            printf("Error semop take\n");
            exit(-1);
        }
        //saisie chaine
        printf("Please enter text for proc %i...\n",getpid());
        scanf("%s",string);
        //liberation semaphore
        printf("Liberation semaphore\n");
        if(semop(sem,&v,1)<0){
            printf("Error semop free\n");
            exit(-1);
        }
        index++;
    };
    free(string);
    if(semctl(sem,0,IPC_RMID,0)==-1){
        printf("Error destruction semaphore\n");
        exit(-1);
    }
};




int main(){
    int pid,sem;
    sem = semget((key_t)KEY,1,0666 | IPC_CREAT);
    if (sem == -1){
        printf("Error creation semaphore\n");
        exit(-1);
    };
    // Initilaisation semaphore
    printf("Initialisation semaphore\n");
    if(semctl(sem,0,SETVAL,1)==-1){
        printf("Error initialisation semaphore\n");
        exit(-1);
    };
    switch(pid=fork()){
        case -1:
            printf("Error fork\n");
            break;
        case 0:
            printf("processus fils\n");
            processus(sem);
            break;
        default:
            printf("Processus pere\n");
            processus(sem);
            break;
    }
    return 0;
}