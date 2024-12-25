#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int r = 10; // TODO : mettre random
    int p = fork();
    if (p > 0)
    {
        for (int i = 0; i < r; i++)
        {
            dprintf(1,"A table!\n");
            sleep(1);
            waitpid(0,NULL,WNOHANG);
        }
    }
    else if (p == 0)
    {
        sleep(r);
        dprintf(1,"Oui, voilà, c'est bon quoi!\n");
        exit(0);
    }
    else
    {
        perror("fork");
    }
    dprintf(1,"C'est pas trop tôt, tu n'es pas à l'auberge ici!\n");
}
