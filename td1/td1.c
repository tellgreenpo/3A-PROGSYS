#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* fonction(void * arg){
    int i;
    //int *retour;
    //retour = malloc(sizeof(int));
    //*retour = 5;
    for (i=0;i<(*(int*)arg);i++){
        printf("et mon courroux\n");
        sleep(1);
    }
    free(arg);
    //pthread_exit((void*)retour);
    pthread_exit(NULL);
}

int main(void){
    system("clear");
    int i=0;
    //int *retour = NULL;
    char name[3];
    // creation de l'argument N
    int *argument=malloc(sizeof(int));
    printf("Veuillez rentrer N:\n");
    fgets(name,3,stdin);
    *argument = atoi(name);
    // Demande de M
    int *pM = malloc(sizeof(int));
    printf("Veuillez rentrer M:\n");
    fgets(name,3,stdin);
    *pM = atoi(name);
    //creation thrread
    pthread_t tid;
    if(pthread_create(&tid,NULL,fonction,(void*)argument)){
        printf("erreur create\n");
        exit(0);
    }
    for (i=0;i<*pM;i++){
        printf("Coucou\n");
        sleep(1);
    }
    //printf("avant join\n");
    //pthread_join(tid,(void**)&retour);
    //printf("apres join, retour : %d\n",*retour);
    //free(retour);
    pthread_cancel(tid);
    pthread_exit(NULL);
    return 0;
}