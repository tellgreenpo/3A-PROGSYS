#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 4

pthread_key_t key;
pthread_once_t once=PTHREAD_ONCE_INIT;

int incrementation(){
    static int i=0;
    i = *(int *)pthread_getspecific(key);
    i++;
    return i;
};

void * fonction(){
    int * pi;
    pi = malloc(sizeof(int));
    pthread_setspecific(key,(void*)pi);
    for (int j=0;j<N;j++){
        printf("execution incrementation par thread %ld...\n",pthread_self());
        *pi = incrementation();
        printf("post incrementation i : %i\n",*pi);
        sleep(2);
    };
    return NULL;
}

int main(){
    pthread_t th1,th2;
    pthread_key_create(&key,NULL);
    pthread_create(&th1,NULL,fonction,NULL);
    pthread_create(&th2,NULL,fonction,NULL);
    fonction();
    pthread_exit(NULL);
    return 0;
}