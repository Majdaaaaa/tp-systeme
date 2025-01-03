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

int main()
{
    int t[2];
    pipe(t);
    int t2[2];
    pipe(t2);
    // 0 : lecture
    // 1 : écriture
    // ping pong pang
    // ! pas utilise de buffer mais des char c'est mieux 
    char buf[32];
    switch (fork())
    {
    case -1:
        perror("fork");
    case 0: // fait ping  pang
        close(t[0]);
        close(t2[1]);
        printf("ping ");
        fflush(stdout);
        write(t[1], "e", 1);
        read(t2[0], buf, sizeof(buf));
        printf("pang ");
        fflush(stdout);
        exit(0);

    default: // fait pong
        close(t[1]);
        close(t2[0]);
        read(t[0], buf, 1);
        printf("pong ");
        fflush(stdout);
        write(t2[1], "e", 1);
        wait(NULL);
        exit(0);
    }
}