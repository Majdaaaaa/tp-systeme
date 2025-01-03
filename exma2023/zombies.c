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

int main()
{
    int r = fork();

    if (r == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (fork() == 0)
            {
                execvp("sleep", (char *[]){"sleep", "6", NULL});
                exit(0);
            }
        }
        sleep(6);
        exit(0);
    }
    else
    {
        waitpid(r, NULL, 0);
    }
    return 0;
}