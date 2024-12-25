#define DEFAULT_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: ./e_ex3 <tube>\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    char to_write[100];
    char* message = "Ceci est un message de moi ecrivain\n";
    snprintf(to_write,sizeof(to_write), "pid %d, length %ld, message %s", getpid(),strlen(message),message);
    int w = write(fd, to_write, strlen(to_write));
    if (w < 0)
    {
        perror("write");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}