#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int *px = NULL;
int max = 10;

void * fonction(void * arg){
    
    int i;
    for (i=0;i<max;i++){
        if(pthread_mutex_lock(&mutex)){
            printf("erreur lock\n");
            exit(-1);
        }
        printf("variable globale : %i\n",(*px));
        if(pthread_mutex_unlock(&mutex)){
            printf("erreur unlock\n");
            exit(-1);
        }
    }
    free(arg);
    pthread_exit(NULL);
    return NULL;
}

int main(void){
    system("clear");
    int i;
    int x = 1;
    px = &x;
    pthread_t tid;
    if(pthread_create(&tid,NULL,fonction,NULL)){
        printf("erreur create\n");
        exit(0);
    }
    for (i=0;i<max;i++){
        if(pthread_mutex_lock(&mutex)){
            printf("erreur lock\n");
            exit(-1);
        }
        px = NULL;
        printf("itération : %d\n",i);
        px = &x;
        if(pthread_mutex_unlock(&mutex)){
            printf("erreur unlock\n");
            exit(-1);
        }
    }

    pthread_exit(NULL);
    return NULL;
}