#include <stdlib.h>
#include <unistd.h>

/* ./garfield lit l'entrée standard et la recopie sur la sortie standard,
 * caractère par caractère.
 * Termine avec le code 1 si une erreur se produit, et le code 0 sinon. */

int main() {
  char mem;
  int fd,nb;
  while(1) {
    /* TODO : lire un caractère sur stdin,
     * *en utilisant l'appel système read()*.
     * En cas d'erreur de lecture, terminer sur une erreur (exit code : 1).
     * En cas de fin de fichier, sortir de la boucle. */
	fd=read(0,&mem,1);
	if(fd < 0){
	 return 1;
	}
	if(fd == 0){
	 break;
	}
    /* TODO : écrire sur stdout le caractère qui vient d'être lu, *en
     * utilisant l'appel système write()*. En cas d'erreur d'écriture,
     * terminer sur une erreur (exit code : 1) */
	nb=write(1,&mem,fd);
	if(nb < 0){
	 return 1;
	}
  }
  /* TODO : Tout stdin a été correctement copié sur stdout :
   * terminer avec l'exit code 0 */
	return 0;
}
