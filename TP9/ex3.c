#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(void)
{
    int fd[2];
    pipe(fd);
    // fd[0] = lecture
    // fd[1] = écriture
    // le fils bloque, le père le débloque et se bloque lui meme, le fils le débloque
    switch (fork())
    {
    case -1:
        perror("fork");
        exit(1);
    case 0:
        close(fd[1]);
        char *c = malloc(1); //pas ouf d'envoyé des données non-initialisés
        int r = read(fd[0], c, 1);
        if (r < 0)
        {
            perror("read");
            exit(1);
        }
        printf("Bonne journée, papa!\n");
        // write(fd[1], c, 1);
        free(c);
        exit(0);

    default:
        sleep(2);
        close(fd[0]);
        printf("J'y vais, à ce soir!\n");
        int w = write(fd[1], "p", 1);
        if (w < 0)
        {
            perror("write");
            exit(1);
        }
        wait(NULL); // C'est le père qui attends le fils donc wait est ok
        // Mais pour le fils qui attensds le père on a besoin des pipes
        printf("Merci, firston!\n");
        exit(0);
    }
}