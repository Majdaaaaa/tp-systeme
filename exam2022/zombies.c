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
    if (fork())
    {
        if (fork())
        {
            if (fork())
            {
                sleep(2);
                // exit(0);
            }
            else
            {
                // sleep(0);
                // exit(0);
                execvp("sleep", (char *[]){"sleep", "1", NULL});
                exit(0);
            }
        }
        else
        {
            execvp("sleep", (char *[]){"sleep", "1", NULL});
            exit(0);
        }
    }
    else
    {
        execvp("sleep", (char *[]){"sleep", "1", NULL});
        exit(0);
    }
    exit(0);
}
