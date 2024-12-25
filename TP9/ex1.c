#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(void){
    //pipe avant le fork 
    //Le pere a pas besoin de lecture et le fils a pas besoin d'ecriture
    //donc faut fermer les descripteurs inutiles
    srandom(getpid());
    int p[2];
    pipe(p);
    int r = random()%101;

    switch(fork()){
        case -1:
        perror("fork");
        break;
        case 0 : 
            close(p[1]);
            int n;
            if(read(p[0],&n,sizeof(n))!=sizeof(n)){
                perror("read");
                exit(1);
            }
            printf("je suis le fils : %d\n",n);
            exit(0);
        default : 
            close(p[0]);
            if(write(p[1],&r,sizeof(r))!=sizeof(r)){
                perror("write");
                exit(1);
            }
            printf("je suis le père : %d\n",r);
            wait(NULL); //sans se wait rien on est pas garanti que l'invite ne se réaffiche pas avant que le fils fasse ses affaires
            exit(0);
    }
}