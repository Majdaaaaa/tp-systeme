#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("usage: ./compare <fic1> <fic2>\n");
        exit(EXIT_FAILURE);
    }

    char tmp1[] = "/tmp/toto";
    char tmp2[] = "/tmp/tutu";
    //Les faire en tubes nommés
    int fd1 = open(tmp1, O_RDWR | O_CREAT, 0666);
    int fd2 = open(tmp2, O_RDWR | O_CREAT, 0666);

    if (fd1 == -1 || fd2 == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int r = fork();
    switch (r)
    {
    case -1:
        perror("fork");
        exit(1);
    case 0:
    { // FILS 1 fait sort <fic1> | uniq > tmp1
        int t[2];
        pipe(t);
        int pf = fork();
        switch (pf)
        {
        case -1:
            perror("fork");
            exit(1);
        case 0: // petit fils fais sort
            close(t[0]);
            dup2(t[1], STDOUT_FILENO);
            close(t[1]);
            execlp("sort", "sort", argv[1], NULL);
            perror("execlp sort");
            exit(1);
        default: // fils fais uniq
            close(t[1]);
            dup2(t[0], STDIN_FILENO);
            close(t[0]);
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
            wait(NULL);
            execlp("uniq", "uniq", NULL);
            perror("execlp uniq");
            exit(1);
        }
    }
    default: //PERE
    {
        int r2 = fork();
        switch (r2)
        {
        case -1:
            perror("fork");
            exit(1);
        case 0:
        { // FILS 2 fait sort <fic2> | uniq > tmp2
            int t2[2];
            pipe(t2);
            int pf2 = fork();
            switch (pf2)
            {
            case -1:
                perror("fork");
                exit(1);
            case 0: // petit fils fais sort
                close(t2[0]);
                dup2(t2[1], STDOUT_FILENO);
                close(t2[1]);
                execlp("sort", "sort", argv[2], NULL);
                perror("execlp sort");
                exit(1);
            default: // fils fais uniq
                close(t2[1]);
                dup2(t2[0], STDIN_FILENO);
                close(t2[0]);
                dup2(fd2, STDOUT_FILENO);
                close(fd2);
                wait(NULL);
                execlp("uniq", "uniq", NULL);
                perror("execlp uniq");
                exit(1);
            }
        }
        default:        // Le père doit faire cmp
            wait(NULL); // Attendre la fin des processus fils
            wait(NULL);
            execlp("cmp", "cmp", tmp1, tmp2, NULL);
            perror("execlp cmp");
            exit(1);
        }
    }
    }
    close(fd1);
    close(fd2);
    exit(0);
}