#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sem.h>
#include <ipc.h>
#include <types.h>



int main(){
    // pid pour recuperer le fils
    int pid;
    union semun arg;
    arg.val = 1;

    // Creation des deux process
    switch (pid=fork()){
        case -1:
        printf("Erreur creation fils");
        exit(-1);
        break;

        case 0:
        // Cle pour la semaphore
        int key = 1;
        int sem;
        sem = semget((key_t)key,1,0666 | IPC_CREAT);
        // On est dans le fils
        
        default:
        // Cle pour la semaphore
        int key = 1;
        int sem;
        sem = semget((key_t)key,1,0666 | IPC_CREAT);
        // On initialise la semaphore
        if(semctl(sem,0,SETVAL,arg)){
            printf("Erreur init semaphore");
            exit(-1);
        }
        // On est dans le pere
    }

    return 0;
}