#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

char usage[] = " prend deux paramètres\n";

int main(int argc, char **argv) {
  int fd, nb;

  if (argc <= 2) {
    write(STDERR_FILENO, argv[0], strlen(argv[0]));
    write(STDERR_FILENO, usage, strlen(usage));
    return 1;
  }

  /* création et ouverture en écriture d'un nouveau fichier de nom
   * argv[2]; le programme doit terminer avec la valeur de retour 2 si
   * argv[2] existe, et avec la valeur de retour 1 dans le cas d'une
   * autre erreur.  */

 // TODO : compléter
fd=open(argv[2],O_RDWR | O_CREAT | O_EXCL, 0666);
if(fd < 0){
	if (errno == EEXIST ) {return 2;}
	else {return 1;}
}
  /* écriture de la chaîne argv[1] dans argv[2]; terminer avec la valeur
   * de retour 1 en cas d'erreur. */
nb=write(fd,argv[1],strlen(argv[1]));
if(nb < 0){return 1;}
close(fd); //appel qui échoue pas

  // TODO : complété

  /* fermeture du descripteur */

  // TODO : compléter

  return 0;
}
