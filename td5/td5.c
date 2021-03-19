#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#define MAX 6



pthread_mutex_t mutex;


void leon(void * arg){
    printf("ARghhhh\n");
    kill(pthread_self(),SIGKILL);
}

void* fonction(){
    pthread_cleanup_push(leon,NULL);
    //if(signal(SIGUSR1,handler)== SIG_ERR){
    //  printf("error in signal\n");
    //  exit(-1);
    //};
    if(pthread_mutex_lock(&mutex)){
        printf("Error mutext lock\n");
        exit(-1);
    }
    // section critique
    printf("J'ai le mutex\n");
    for (int i=0;i<MAX;i++){
        printf("TID : %ld\n",pthread_self());
        sleep(1);
    }
    if(pthread_mutex_unlock(&mutex)){
        printf("Error mutex unlock\n");
        exit(-1);
    }
    printf("Je libère le mutex\n");
    pthread_cleanup_pop(0);
    pthread_exit(NULL);

    /*
    int i;
    pthread_cleanup_push(leon,NULL);
    for (i=0;i<20;i++) sleep(1);
    printf("Contrat non realise\n");
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
    */
    return (NULL);
}

int main()
{
    sigset_t ens_signal;
    struct sigaction action;
    action.sa_handler = leon;
    for(int i=0;i<2;i++){
        pthread_t th;
        if(pthread_create(&th, NULL,fonction ,NULL)){
            printf("Error creation thread\n");
            exit(-1);
        }
        for(int j=0;j<(MAX/2);j++){
            printf("TID : %ld\n",pthread_self());
            sleep(1);
        }
        pthread_cancel(th);
        sleep(1);
    }
    pthread_exit(NULL);
    return(0);
}
