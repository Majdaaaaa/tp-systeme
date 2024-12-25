#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <wait.h>
// TODO : gestion d'erreur

int plus_proch(int r, int* stock,int *indice) {
    int proche = stock[0];
    int diff_min = abs(r - stock[0]);

    for (int i = 1; i < 10; i++) {
        int diff = abs(r - stock[i]);
        if (diff < diff_min) {
            diff_min = diff;
            proche = stock[i];
            *indice=i;
        }
    }
    return proche;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    int r = rand() % 100;
    int stock[10];
    int pid[10];

    printf("Tirage du père : %d\n",r);
    int i = 1;
    while (i <= 10)
    {
        int child;
        switch (child = fork())
        {
        case -1:
            perror("fork");
        case 0:
            srand(getpid());
            int n = rand() % 100;
            printf("je suis le fils n°%d et mon tirage est : %d\n", getpid(), n);

            return n;
        default:
            int status;
            pid_t child_pid = waitpid(child, &status, 0);
              if (child_pid == -1)
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            if (WIFEXITED(status))
            {
                int return_value = WEXITSTATUS(status);
                stock[i]=return_value;
                pid[i]=child_pid;
            }
            i++;
        }
    }
    int indice;
    int gagant = plus_proch(r,stock,&indice);
    printf("le gagant c'est %d avec la valeur : %d\n",pid[indice],gagant);

    return 0;
}

