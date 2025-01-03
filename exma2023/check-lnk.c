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
#include <Kernel/sys/stat.h>
void check(char *rep)
{
    DIR *dir = opendir(rep);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", rep, entry->d_name);

            struct stat s;
            if (lstat(path, &s) != 0)
            {
                perror("lstat");
                closedir(dir);
                exit(EXIT_FAILURE);
            }
            if (S_ISDIR(s.st_mode))
            {
                check(path);
            }
            else if (S_ISLNK(s.st_mode))
            {
                char buf[PATH_MAX];
                int len = readlink(path, buf, PATH_MAX);
                if (len != -1)
                {
                    buf[len] = '\0';
                    // lit le fichier vers lequel ca pointe et c retourne 1 ou -1 => ya un probleme = c'est corrompu
                    struct stat base;
                    if (stat(buf, &base) != 0)
                    {
                        printf("lien corrompu : %s -> %s\n", path, buf);
                    }
                }
                else
                {
                    perror("readlink");
                }
            }
        }
    }
    closedir(dir);
}

int main()
{
    check(".");
    return 0;
}