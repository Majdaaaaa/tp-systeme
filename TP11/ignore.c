#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h> 

void handle_usr1(int sig){
}

int main(){

    struct sigaction action_ignore ={0};
    struct sigaction action_usr1 = {0};

    action_ignore.sa_handler = SIG_IGN;
    action_usr1.sa_handler = handle_usr1;

    for(int i =1 ; i<NSIG ;i++){
        if(i==SIGUSR1) continue; //pour pas ignoré SIGUSR1
        if(sigaction(i,&action_ignore,NULL)!=0){
            fprintf(stderr, "%d (%s) : %/n",i,strsignal(i),strerror(errno));
        }
    }

    if(sigaction(SIGUSR1,&action_usr1,NULL)!=0){

    }
    printf("je m'endors\n");
    pause(); //pas la peine de faire une boucle
    printf("HUmmmm...\n");
    pause();
    printf("je me reveille\n");

    return 0;

}

