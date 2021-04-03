#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>


void processus(int process){
    int t_ecriture, t_lecture;
    int ping;
    if(process==1){
        // P1
        // Creation de tube
        printf("Creation tube 1\n");
        if(mkfifo("./pipe_p1",S_IRUSR | S_IWUSR)){
            printf("Error creation tube\n");
            exit(-1);
        };
        //synchronisation
        printf("Synchro tube\n");
        if(!(t_ecriture = open("./pipe_p1",O_WRONLY))){
            printf("Error open\n");
            exit(-1);
        };
        printf("Fin synchro ecriture\n");
        if(!(t_lecture = open("./pipe_p2",O_RDONLY))){
            printf("Error open\n");
            exit(-1);
        };
        printf("Fin synchro lecture\n");
        // debut de boucle
        printf("Debut boucle\n");
        ping = 0;
        while(ping<1000){
            // write sur tube
            if(write(t_ecriture,&ping,sizeof(int))!=sizeof(int)){
                printf("Error write\n");
                unlink("./pipe_p1");
                exit(-1);
            };
            printf("ping %i\n",ping);
            // read sur tube
            if(read(t_lecture,&ping,sizeof(int))!=sizeof(int)){
                printf("Error read\n");
                unlink("./pipe_p1");
                exit(-1);
            }
            ping++;
        };
    }else{
        // P2
        // Creation de tube
        printf("Creation tube 2\n");
        if(mkfifo("./pipe_p2",S_IRUSR | S_IWUSR)){
            printf("Error creation tube\n");
            exit(-1);
        };
        //synchronisation
        printf("Synchro tube\n");
        if(!(t_lecture = open("./pipe_p1",O_RDONLY))){
            printf("Error open\n");
            exit(-1);
        };
        printf("Fin synchro lecture\n");
        if(!(t_ecriture = open("./pipe_p2",O_WRONLY))){
            printf("Error open\n");
            exit(-1);
        };
        printf("Fin synchro ecriture\n");
        // debut de boucle
        printf("Debut boucle\n");
        ping = 0;
        while(1){
            // write sur tube
            if(read(t_lecture,&ping,sizeof(int))!=sizeof(int)){
                printf("Error write\n");
                unlink("./pipe_p2");
                exit(-1);
            };
            printf("ping %i\n",ping);
            ping++;
            // read sur tube
            if(write(t_ecriture,&ping,sizeof(int))!=sizeof(int)){
                printf("Error read\n");
                unlink("./pipe_p2");
                exit(-1);
            }
        }
    }
}



int main(int argc, char * argv[]){
    int var = atoi(argv[argc-1]);
    processus(var);
    return 0;
}