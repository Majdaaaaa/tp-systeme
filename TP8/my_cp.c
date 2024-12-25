#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

//TODO : gestion d'erreur

int main(int argc, char** argv){
    int fd1, fd2;

    if(argc<2){
        dprintf(2,"Usage : my_cp fic1 fic2\n");
        return 1;
    }

    fd1=open(argv[2],O_RDONLY);
    fd2=open(argv[3],O_WRONLY|O_CREAT,0666);

    dup2(fd1,0);
    dup2(fd2,1);
    close(fd1);
    close(fd2);
    execlp("cat"/* nom du programme */,"cat",NULL/* argv */); //utilisation de execlp psq pas d'arguments variable
                //execvp("cat"/* programme */, {"cat",NULL}/* argv */);
    //cat vas lire son entrée standard (qui est maintenant fd1) et ecrire sur sa sortie standard(fd2) 
    //Pas de fork psq apres le execlp on a rien a faire (c'est pas sur le processus du shell que ca écrit vu que le shell fais un fork a l'appel du programme )
}