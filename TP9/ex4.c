#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    // Le processus qui gagne affiche et exit direct
    int fd1[2];
    pipe(fd1);
    int fd2[2];
    pipe(fd2);
    // fd[0] = lecture
    // fd[1] = écriture

    switch (fork())
    {
    case -1:
        perror("fork");
        exit(1);
    case 0:
        close(fd1[1]); // close  l'écriture de fd1
        close(fd2[0]); // close la lecture de fd2
        srandom(getpid());
        int r = random() % 10;
        while (1)
        {
            char c = 2;
            int re2=read(fd1[0], &c, 1) ;
            if (re2 == -1)
            {
                perror("read fils");
                exit(1);
            }else if(re2==0){
                printf("Bravo... une petite revanche? fils\n");
                exit(0);
            }

            r--;
            if (r<=0) {break;} // bien placé

            printf("pong\n");
            if (write(fd2[1], &c, 1) == -1)
            {
                perror("write fils");
                exit(1);
            }
     
            //if (r<=0) {break;} // mal placé
        }
        printf("Dehors, j'ai gagné! fils \n");
        exit(0);
    default:
        close(fd1[0]); //close la lecture de fd1
        close(fd2[1]);//close l'écriture de fd2
        srandom(getpid());
        int r1 = random() % 10;
        while (r1 > 1)
        {
            char c = 2;
            printf("ping\n");
            if (write(fd1[1], &c, 1) == -1)
            {
                perror("write père");
                exit(1);
            }
            r1--;
            int re = read(fd2[0],&c,1);
            if(re==-1){
                perror("read père");
                exit(1);
            }else if(re==0){
                printf("Bravo... une petite revanche? père\n");
                exit(0);
            }
        }
        printf("Dehors, j'ai gagné! père\n");
        exit(0);
    }
}
