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
#include <sys/stat.h>
#include <dirent.h>
#include <Kernel/sys/stat.h>

int rec(char *rep, int cmp)
{
    DIR *dir = opendir(rep);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name) != 0)
        {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", rep, entry->d_name);

            struct stat s;
            if (stat(path, &s) == 0)
            {
                if (S_ISDIR(s.st_mode))
                {
                    cmp++;
                    cmp = rec(path, cmp);
                }
            }
        }
    }
    closedir(dir);
    return cmp;
}

int main()
{
    // Parcours le répertoire, compte + affiche le nombre de sous-rep
    // DIR *rep = opendir(".");
    // struct dirent *entry;
    // int cmp = 0;

    int cmp = rec(".", 1);
    //? exo 1
    // while ((entry = readdir(rep)) != NULL)
    // {

    //     // if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
    //     // {
    //     struct stat *s;
    //     int i = stat(entry->d_name, s);
    //     if (i == 0)
    //     {
    //         if (S_ISDIR(s->st_mode)) // & = et bit à bit
    //         {
    //             printf("nom du sous-rep %s\n", entry->d_name);
    //             cmp++;
    //         }
    //     }
    //     // }
    // }

    // closedir(rep);
    printf("Nombre de sous-rep %d\n", cmp);
    return 0;
}
