#define DEFAULT_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>

volatile sig_atomic_t signal_recu = 0;

void handler()
{
    signal_recu += 1;
}

int main()
{
    struct sigaction s = {0};
    s.sa_handler = &handler;
    int pid[4];
    if (sigaction(SIGUSR1, &s, NULL) != 0)
    {
        perror("sigaction");
    }

    printf("%d\n", signal_recu);
    for (int i = 0; i < 4; i++)
    {
        int r = fork();
        if (r == 0)
        {
            pause();
            printf("%d\n", signal_recu);
            if (signal_recu == 1)
            {
                //
                signal_recu = 0;
                printf("%d , %d , Oui papa j'arrive\n", getpid(), i);
                exit(0);
            }
        }
        else
        {
            pid[i] = r;
        }
    }

    int r = rand() % 5;
    printf("je vais sleep pendant %d\n", r);
    sleep(r);
    // QUESTION 2
    for (int i = 0; i < 4; i++)
    {
        sleep(1);
        kill(pid[i], SIGUSR1);
    }

    // QUESTION 1
    // for (int i = 0; i < 4; i++)
    // {
    //     kill(pid[i], SIGUSR1);
    // }

    exit(0);
}