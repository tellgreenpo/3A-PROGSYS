#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define M 4

pthread_mutex_t mutexM = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int *px = NULL;
int x = M;
px = &x;

void * fonction1(void * arg){
    
    if(pthread_mutex_lock(&mutexM)){
        printf("erreur lock\n");
        exit(-1);
    }
    if(*(int*)arg > x){
        pthread_cond_wait(&cond,&mutexM);
    }
    printf("On utilise %i ressources\n",*(int*)arg);
    *px=(*px)- *((int*)arg);
    sleep(1);
    *px += *((int*)arg);
    pthread_cond_signal(&cond);
    if(pthread_mutex_unlock(&mutexM)){
        printf("erreur unlock\n");
        exit(-1);
    }
    free(arg);
    pthread_exit(NULL);
    return NULL;
}

int main(void){
    system("clear");
    int r1 = 3;
    int r2 = 2;
    pthread_t tid1;
    pthread_t tid2;
    for(int i =0; i<5;i++){

        if(pthread_create(&tid1,NULL,fonction,(void *)&r1)){
            printf("erreur create\n");
            exit(0);
        }
        if(pthread_create(&tid2,NULL,fonction,(void *)&r2)){
            printf("erreur create\n");
            exit(0);
        }
    
        if(pthread_mutex_lock(&mutex)){
            printf("erreur lock\n");
            exit(-1);
        }
        if(*(int*)arg > x){
            pthread_cond_wait(&cond,&mutexM);
        }
        *px = (*px)-1;
        sleep(1);
        *px = (*px)+1;
        pthread_cond_signal(&cond);
        if(pthread_mutex_unlock(&mutex)){
            printf("erreur unlock\n");
            exit(-1);
        }
    }


    pthread_exit(NULL);
    return NULL;
}