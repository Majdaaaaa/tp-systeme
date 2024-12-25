#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t got_sigurs2 = 0, got_sigint = 0, got_sigalrm = 0;

// ça c'est un bon handler qui dis juste quel signal on a reçu
void handler(int sig)
{
    switch (sig)
    {
    case SIGUSR2:
        got_sigurs2 = 1;
        break;
    case SIGINT:
        got_sigint = 1;
        break;
    case SIGALRM:
        got_sigalrm = 1;
        break;
    }
}

int run()
{
    int n = 0;
    /*
    ?applique le masque vide pour autorisé la réception de tout les signaux
    * sigset_t empty;
    * sigemptyset(&empty);
    * sigsuspend(&empty); //prends en arg un masque de signaux et applique temporairement ce masque
    */
    /* On enlève que le masque de SIGUSR1 psq on veut pas faire débloqué sigsuspend par d'autre signaux */
    sigset_t allow_sigusr1;
    sigemptyset(&allow_sigusr1);
    sigaddset(&allow_sigusr1, SIGINT);
    sigaddset(&allow_sigusr1, SIGUSR2);
    sigaddset(&allow_sigusr1, SIGALRM);
    sigsuspend(&allow_sigusr1);

    sigset_t block_sigusr1;
    sigemptyset(&block_sigusr1);
    sigaddset(&block_sigusr1,SIGUSR1);
    // Si on reçoit SIGINT pendant ce sleep il vas rien se passé (avant ajout de block_usr1)
    // Pas ouf sleep psq on veut recevoir le signal pendant le sleep et pas avant
    // sigprocmask(SIG_SETMASK, &block_sigusr1, NULL);
    // sleep(1);
    srandom(getpid());
    // alarm envoie le signal SIGALRM qui tue le processus donc faut mettre un handler sur SIGALRM
    alarm(rand() % 3 + 1);

    // Si sigsuspend termine c'est qu'on a forcément reçu un signal
    // Le problème ici c'est qu'on sait pas quel signal on a reçu donc dans le handler on peut affecté affecté une valeur a une variable volatile sigatomic_t

    while (1)
    {
        sigsuspend(&block_sigusr1); //on appelle sigsuspend a chaque itération
        //Les signaux seront reçus que pendant sigsuspend
        if (got_sigint)
        {
            got_sigint = 0; // faut noté qu'on a fini de traité un signal
            printf("%d : %d\n", getpid(), n);
            //!continue; provoque un deadlock 
            //Psq si on reçoit SIGINT et SIGALRM on traite SIGINT mais pas SIGALRM et on fera pas alarm
            //psq quand on continue on reviens sur sigsuspend et on est bloqué si on reçoit pas un autre signal
        }
        if (got_sigurs2)
        {
            got_sigurs2 = 0;
            printf("%d : J'ai perdu ! \n", getpid());
            return 0;
        }
        if (got_sigalrm)
        {
            got_sigalrm = 0;
            n++;
            if (n >= 20)
            {
                printf("%d : Gnark, j'ai gagné ! \n", getpid());
                killpg(0, SIGUSR2);
                return 0;
            }
            alarm(rand() % 3 + 1); //! psq si on le met pas, on recevra plus jamais de SIGALRM
        }
    }
}

int main()
{
    // Masquage de signaux
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1); //[... +SIGINT, +SIGUSR2]
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGALRM);
    // Vu qu'on a ça on a pas besoin d'utilisé sigsuspend
    // Si sigsuspend est appelé avant killpg ok
    // Sinon le signal est envoyé mis dans la table des signaux pendant
    // sigsuspend est appelé et signal est sortie et ca se passe pareil que dans le cas précédent
    sigprocmask(SIG_SETMASK, &set, NULL);

    // Installé les gestionnaires de signaux
    struct sigaction act = {0};
    act.sa_handler = &handler;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGINT, &act, NULL);

    // Crée les fils
    for (int i = 0; i < 9; i++)
    {
        // Les fils héritent de l'id de groupe du père
        // Le père et ses fils sont les seuls membres de ce groupe psq =
        // A chaque commande lancé le shell crée un nouveau groupe donc le père sera le fondateur d'un nouveau groupe
        // Et ses fils feront partie de ce groupe après leur création
        pid_t child_pid = fork();
        if (child_pid < 0)
        {
            goto error;
        }
        else if (child_pid == 0)
        {
            return run();
        }
    }

    killpg(0, SIGUSR1);
    return run();

error:
    perror("main");
    return 1;
}