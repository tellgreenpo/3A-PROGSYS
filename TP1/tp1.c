#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>



static int running=1;
/* handler pour le signal sigusr1 */
void handler(int sig){
    // remettre le signal par default ==> 
    (void)signal(SIGUSR1,SIG_DFL);
    printf("On a reçu le signal et on kill le process\n");
    // Mettre le boolean a False;
    running = 0;
}

//====================================================================
// A FAIRE
// Refaire un handler pour SIGCHILD seulement pour le pere
//========================================================================
int main()
{
    sigset_t ens_signal; // ensemble des signaux captures
    struct sigaction action; // structure de mise en place du handler
    action.sa_handler = handler;
    action.sa_flags = 0;



    // ca marche pas exactement ça
    if(sigfillset(&ens_signal)){
        printf("Erreur mise a vide des signaux capturés\n");
        exit(-1);
    }
    

    if (sigdelset(&ens_signal,SIGUSR1)){
        printf("Erreur ajout SIGUSR1 a l'ensemble des signaux capturés\n");
        exit(-1);
    }

    // Il faut parametrer


// On doit affecter le SIGUSR1


  int pid =0;
  int status =0;
  // Ce qui est avant le fork est commun au deux
  // On associe le SiGUSER1 au handler

//====================================================================================
  // Refaire ça que pour le pere quand le fils il meurt A FAIRE ======================
  //=================================================================================



  if(signal(SIGUSR1,handler)== SIG_ERR){
      printf("error in signal\n");
      exit(-1);
  };

// creation pour le pipe
    int p[2];
    int f[2];

    // Creation du pipe pour le pere
    if(pipe(p)){
        printf("Erreur creation pipe\n");
        exit(-1);
    }
    // Creation du pipe pour le fils
    if(pipe(f)){
        printf("Erreur creation pipe\n");
        exit(-1);
    }

// Création du fils avec un fork  
  switch (pid=fork()){ // separer le fork pour pouvoir mettre le code dans les deux
  case -1: 
    printf("erreur au fork\n");
    exit(-1);
    break;

  case 0: // Retourne 0 quand on est sur le fils et pid prend la valeur du pid du fils
    
    if(sigaction(SIGUSR1,&action,NULL)){
        printf("Erreur sigaction");
        exit(-1);
    } // Pour utiliser
    
    printf("%d on est chez le fils et on attend\n",getpid());
    printf("On entre en boucle pour incrémenter\n");
    // pingpong time dans le While car c'est au pere de tuer le fils
    int pong = 0; // Variable qu'on va incrémenter pour renvoyer

    // fermer les pipes pourle fils et le pere dans les deux sens
    // sur le fils clore indice 0 et le pere indice 1
    // Pour etre sur de ne pas faire d'erreur

    if(write(f[1],&pong,sizeof(int))!=sizeof(int)){
            printf("Erreur écriture\n");
            exit(-1);
    };
    while(running){
        //pause() // pour pas tout réevaluer h24
        // on va lire
        sleep(1);
        if(read(p[0],&pong,sizeof(int))!=sizeof(int)){
            printf("Erreur lecture\n");
            exit(-1);
        }
        // incrémenter
        pong++;
        printf("Pong : %i\n",pong);
        // renvoyer au pere
        if(write(f[1],&pong,sizeof(int))!=sizeof(int)){
            printf("Erreur écriture\n");
            exit(-1);
        }
    };

    


    printf("%d le fils se reveil et meurt avec un code 2\n",getpid());
    exit(2);
  
  
  default: // Autre quand on est sur le père
    //printf("%d le pere a cree un fils, il doit le tuer au bout de 10 secondes\n",getpid());
    
    // On doit refaire le action pour notre ami SIGCHLD

    // On va limiter les échanges du pingpong de la variable

    if(sigaction(SIGUSR1,&action,NULL)){
        printf("Erreur sigaction");
        exit(-1);
    } // Pour utiliser

    printf("On est chez le pere\n");
    int ping;
    ping = 0;
    if(write(p[1],&ping,sizeof(int))!=sizeof(int)){
            printf("Erreur écriture\n");
            exit(-1);
    };
    printf("On a écris");
    while(ping < 20){
        if(read(f[0],&ping,sizeof(int))!=sizeof(int)){
            printf("Erreur lecture\n");
            exit(-1);
        };
        // incrémenter
        ping++;
        printf("Ping : %i\n",ping);
        // renvoyer au pere
        if(write(p[1],&ping,sizeof(int))!=sizeof(int)){
            printf("Erreur écriture\n");
            exit(-1);
        };
    };
    // Quand on atteint la limite on tue le fils
    printf("On signale au fils de mourrir....\n");
    sleep(3);
    printf("PID du fils  : %i\n",pid);
    if(kill(pid,SIGUSR1)){
        printf("Erreur sur le kill du fils depuis le pere\n");
        exit(-1);
    }
    printf("%d l'attente du pere se termine avec le code %d\n",getpid(),waitpid(-1,&status,0));
    printf("%d le status donne comme code %d\n",getpid(),WEXITSTATUS(status));
  }




  printf("%d fin\n",getpid());
  return 0;
}
