#define DEFAULT_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: ./x4 <cmd>\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", argv[1]);

    int t[2]; // 0 : écritrue 1 : lecture
    if (pipe(t) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int r = fork();
    switch (r)
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
    {
        close(t[0]);
        dup2(t[1], STDERR_FILENO);
        close(t[1]);

        int nul = open("/dev/null", O_WRONLY);
        if (nul == -1)
        {
            perror("open /dev/null");
            exit(EXIT_FAILURE);
        }
        dup2(nul, STDOUT_FILENO);
        close(nul);

        execvp("strace", (char *[]){"strace", argv[1], NULL});
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    default:
        close(t[1]);
        wait(NULL);

        int cmp = 0;
        char buf[BUFSIZ];
        size_t n;
        while ((n = read(t[0], buf, sizeof(buf))) > 0)
        {
            // Utilisé strchr pas ouf psq si le buf est pas initilisé la fonction fait du caca
            // n est la taille de ce qu'on a lu, pas faire de boucle sur BUFSIZ
            for (size_t i = 0; i < n; i++)
            {
                if (buf[i] == '\n')
                {
                    cmp++;
                }
            }
        }
        close(t[0]);

        printf("Nombre de lignes : %d\n", cmp);
        exit(0);
    }

    return 0;
}