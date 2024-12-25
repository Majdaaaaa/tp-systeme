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
        printf("usage: ./l_ex3 <tube>\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    char buf[100];
    int r;
    while ((r = read(fd, buf, sizeof(buf) - 1)) > 0)
    {
        buf[r] = '\0'; // Assurez-vous que la chaîne est terminée par un caractère nul

        int pid;
        long length;
        char message[100];

        if (sscanf(buf, "pid %d, length %ld, message %[^\n]", &pid, &length, message) == 3)
        {
            printf("PID: %d\n", pid);
            printf("Length: %ld\n", length);
            printf("Message: %s\n", message);
        }
        else
        {
            printf("Format du message incorrect\n");
        }
    }

    if (r < 0)
    {
        perror("read");
    }

    close(fd);
    return 0;
}