#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 3

pthread_once_t once=PTHREAD_ONCE_INIT;

void fonction_init(){
    printf("fonction_init execute par thread %ld\n",pthread_self());
}

void * fonction(){
    for (int i=0;i<N;i++){
        pthread_once(&once,fonction_init);
        sleep(1);
    };
    return NULL;
}

int main(){
    pthread_t th1,th2;
    pthread_create(&th1,NULL,fonction,NULL);
    pthread_create(&th2,NULL,fonction,NULL);
    pthread_once(&once,fonction_init);
    pthread_exit(NULL);
    return 0;
}