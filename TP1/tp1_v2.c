#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int p[2];
int f[2];
// Creation handler pour SIGUSR1
void handler_sigusr1(int sig){
    printf("Reception de SIGUSR1\n");
    if(kill(getppid(),SIGCHLD)){
        printf("Error kill a pere\n");
    };
    exit(0);
}

// Creation handler pour SIGCHLD
void handler_chld(int sig){
    printf("Reception de SIGCHLD\n");
    exit(0);
}

//Creation programme pere
int pere(int pid,int * p,int * f){
    /* partie 1
    //Attente 10 secondes
    sleep(10);
    //Envoi signal SIGUSR1 au fils
    printf("Envoi kill a fils\n");
    if(kill(pid,10)){
        printf("Error kill fils\n");
    } */
    //Boucle infinie
    int ping = 0;
    printf("Envoi du premier ping\n");
    if(write(p[1],&ping,sizeof(int))!=sizeof(int)){
        printf("Error write father\n");
        exit(-1);
    }
    while(1){
        if (ping<1000){
            // read sur tube fils
            if(read(f[0],&ping,sizeof(int))!=sizeof(int)){
                printf("Error read fils\n");
                exit(-1);
            };
            ping++;
            printf("ping %i\n",ping);
            // write sur tube pere
            if(write(p[1],&ping,sizeof(int))!=sizeof(int)){
                printf("Error write father\n");
                exit(-1);
            };
        }else{
            //Envoi signal SIGUSR1 au fils une fois l'echange fini
            printf("Envoi kill a fils\n");
            if(kill(pid,10)){
                printf("Error kill fils\n");
            }
            sleep(1);
        }
    }
    return 0;
}

//Creation programme fils
int fils(int * p,int * f){
    // Boucle infinie
    int pong = 0;
    if(read(p[0],&pong,sizeof(int))!=sizeof(int)){
        printf("Error read fils\n");
        exit(-1);
    }
    printf("Reception du premier ping\n");
    pong++;
    printf("Envoi du premier pong\n");
    if(write(f[1],&pong,sizeof(int))!=sizeof(int)){
        printf("Error write father\n");
        exit(-1);
    };
    while(1){
        // read sur tube pere
        if(read(p[0],&pong,sizeof(int))!=sizeof(int)){
            printf("Error read fils\n");
            exit(-1);
        };
        pong++;
        printf("pong %i\n",pong);
        // write sur tube pere
        if(write(f[1],&pong,sizeof(int))!=sizeof(int)){
            printf("Error write father\n");
            exit(-1);
        };
    };
    return 0;
}



int main(){
    int pid=0;
    // Declaration pour tubes non-nommes
    if(pipe(p)){
        printf("Error pipe\n");
        exit(-1);
    };
    if(pipe(f)){
        printf("Error pipe\n");
        exit(-1);
    };
    // Structure de mise en place du handler SIGUSR1
    struct sigaction action;
    action.sa_handler = handler_sigusr1;
    if(sigaction(SIGUSR1,&action,NULL)){
        printf("Error sigaction SIGUSR1\n");
    };
    // Creation processus fils
    switch (pid = fork()){
        case -1:
            printf("Error fork\n");
            exit(-1);
        case 0:
            printf("Processus fils\n");
            fils(p,f);
            break;
        default:
            printf("Processus pere\n");
            struct sigaction father;
            father.sa_handler = handler_chld;
            if(sigaction(SIGCHLD,&father,NULL)){
                printf("Error sigaction SIGCHLD\n");
            };
            pere(pid,p,f);
            break;
    };
    return 0;
}