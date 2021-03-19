// pthread_once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>




pthread_once_t once=PTHREAD_ONCE_INIT;
pthread_key_t key;


void coucou(){
    printf("coucou tid: %d\n",(int)pthread_self());
}

int incrementation(){
    int valeur;
    printf("test\n");
    valeur = *(int*)pthread_getspecific(key);
    valeur++;
    return valeur;
}

void* fonction_init(){
    printf("dans le thread tid: %d\n",(int)pthread_self());
    // Initialisation de la clé
    if(pthread_key_create(&key,NULL)){
        printf("Erreur create key");
        exit(-1);
    };
    // Allocation variable specifique
    printf("alloc...\n");
    int * globale = malloc(sizeof(int));
    printf("alloc fini\n");
    if(pthread_setspecific(key,(void*)globale)){
        printf("Erreur set specific");
        exit(-1);
    };
    //pthread_once(&once,coucou); Partie 1
    int i = incrementation();
    printf("i = %i\n",i);
    pthread_exit(NULL);
    return(NULL);
}



int main(){
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_create(&thread1, NULL,fonction_init,NULL);
    pthread_create(&thread2, NULL,fonction_init,NULL);
    pthread_create(&thread3,NULL,fonction_init,NULL);
    //pthread_once(&once,coucou); Pour la premiere partie
    incrementation();
    pthread_join(thread3,NULL);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    pthread_create(&thread1, NULL,fonction_init,NULL);
    pthread_join(thread1,NULL);

    printf("fin du main tid: %d\n",(int)pthread_self());
    return(0);
}
