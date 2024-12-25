#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h> 

void ignore_hup(){
    write(1,"signal SIGHUP reçu et ignoré\n",strlen("signal SIGHUP reçu et ignoré\n"));
}
int main()
{
    /* c'est le signal SIGHUP qui est ignoré avec nohup 
    ! C'est qui nohup.out ????
    */
    struct sigaction no_hup = {0};
    no_hup.sa_handler = SIG_IGN;
    struct sigaction no_hp2 ={0};
    no_hp2.sa_handler = &ignore_hup;

    if(sigaction(SIGHUP,&no_hp2,NULL)==-1){
        perror("sigaction");
        exit(1);
    }

    while(1){
        printf("je suis en vie\n");
    }

    return 0;
    
}