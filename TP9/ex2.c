#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#define MAX 2024

int main(void)
{
    int fd[2];
    // fd[0] = lecture
    // fd[1] = écriture
    pipe(fd);

    // Le fils fais le ls et le père le tail
    // Faire l'inverse est pas bien pourquoi?
    // les 2 se font en meme temps

    // Pas besoin de rétablir la sortie standard ou l'entrée psq le programme termine
    switch (fork())
    {
    case -1:
        perror("fork");
    case 0:
        close(fd[0]);
        int d = dup2(fd[1], 1); // la sortie standard deviens dans quoi on peut écrire
        if (d < 0)
        {
            perror("dup2");
            exit(1);
        }
        close(fd[1]);
        int r = execlp("ls", "ls", "-R", "/usr/bin", NULL);
        if (r < 0)
        {
            perror("execls");
            exit(1);
        }
        exit(0);
    default:

        close(fd[1]);
        int d1 = dup2(fd[0], 0); // l'entrée standard deviens la ou vas lire le fils
        if (d1 < 0)
        {
            perror("dup2");
            exit(1);
        }
        close(fd[0]);
        int r1 = execlp("tail", "tail", NULL);
        if (r1 < 0)
        {
            perror("execlp");
            exit(1);
        }
        exit(0);
    }
}
