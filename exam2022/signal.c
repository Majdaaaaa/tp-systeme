#define DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_recu = 0;

void handler(int sig)
{
    if (sig == SIGUSR1)
    {
        signal_recu = 1;
    }
}

int main()
{
    struct sigaction s = {0};
    s.sa_handler = &handler;

    sigaction(SIGUSR1, &s, NULL);
    int pid = fork();

    if (pid == 0)
    {
        perror("fils");
        printf("ping ");
        fflush(stdout);
        kill(getppid(), SIGUSR1);
        pause();
        if (signal_recu == 1)
        {
            signal_recu = 0;
            printf("pang ");
            fflush(stdout);
        }
        exit(0);
    }
    else if (pid != 0)
    {
        perror("pere");
        pause();
        if (signal_recu == 1)
        {
            signal_recu = 0;
            printf("pong ");
            fflush(stdout);
        }
        kill(pid, SIGUSR1);
        wait(NULL);
        exit(0);
    }
}