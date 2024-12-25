#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void appeler_enfants(int nb_enfants)
{
    pid_t pids[nb_enfants];
    int i;

    // Initialiser le générateur de nombres aléatoires
    

    // Création des processus enfants
    for (i = 0; i < nb_enfants; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            perror("fork");
            exit(1);
        }
        else if (pids[i] == 0)
        {
            srand(getpid());
            // Code exécuté par les enfants
            sleep(rand() % 5 + 1); // Simuler un délai aléatoire
            printf("Voilà, voilà, j'arrive...\n");
            exit(0);
        }
    }

    // Code exécuté par le père
    char *enf = malloc(256);
    enf[0] = '\0';
    for (int i = 0; i < nb_enfants; i++)
    {
        char pid_str[16];
        sprintf(pid_str, "%d, ", pids[i]);
        strcat(enf, pid_str);
    }
    enf[strlen(enf) - 2] = '\0'; // Supprimer la dernière virgule et l'espace
    printf("%s à table!\n", enf);
    free(enf);

    int enfants_restants = nb_enfants;
    pid_t pid;
    int status;
    usleep(500000);
    while (enfants_restants > 0)
    {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid > 0)
        {
            enfants_restants--;
            if (enfants_restants > 1)
              {
                printf("Ah, voici %d! Les autres, à table!\n", pid);
            }
            else if (enfants_restants == 1)
            {
                for (int j = 0; j < nb_enfants; j++)
                {
                    if (pids[j] != pid)
                    {
                        printf("Ah, voici %d! %d, à table, on n'attend plus que toi!\n", pid, pids[j]);
                        break;
                    }
                }
            }
            else if (enfants_restants == 0)
            {
                printf("Ah, enfin! C'est pas trop tôt, tu n'es pas à l'auberge ici!\n");
            }
        }
        else if (pid == 0)
        {
            printf("À table!\n");
            sleep(1); // Attendre avant de répéter l'appel
        }
        else
        {
            perror("wait");
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <nombre d'enfants>\n", argv[0]);
        exit(1);
    }

    int nb_enfants = atoi(argv[1]);
    if (nb_enfants <= 0)
    {
        fprintf(stderr, "Le nombre d'enfants doit être supérieur à 0.\n");
        exit(1);
    }

    appeler_enfants(nb_enfants);
    return 0;
}