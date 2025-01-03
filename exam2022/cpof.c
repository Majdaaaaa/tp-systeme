#define DEFAULT_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    // utilisé wc -l pour compté le nombre de ligne
    if (argc < 2)
    {
        fprintf(stderr, "usage : %s <fic>\n", argv[0]);
        exit(0);
    }
    errno = 0;
    // avoir un fils et un tube anonyme
    // Le fils écrit le résultat de lsof -t argv[1] dans le tube
    // le pere fait wc -l sur le tube et affiche le résulat

    //? ouvrir avant pour que ce soit partagé entre père et fils
    int t[2]; // 0 = lecture  1 = ecriture
    if (pipe(t) == -1)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    switch (fork())
    {
    case -1:
        perror("fork");
        return EXIT_FAILURE;
    case 0:
        // je vais écrire le résulat de lsof -t argv[1] dans le tube
        close(t[0]); // fermeture de la lecture
        dup2(t[1], STDOUT_FILENO);
        close(t[1]);
        char *argl[] = {"lsof", "-t", argv[1], NULL};
        execvp("lsof", argl);
        perror("execvp");
        exit(EXIT_FAILURE);
    default:
        wait(NULL);
        close(t[1]);
        dup2(t[0], STDIN_FILENO);
        close(t[0]);
        char *arg[] = {"wc", "-l", NULL};
        execvp("wc", arg);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}