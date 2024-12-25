#define DEFAULT_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

#define SIZE 4*PIPE_BUF

int main(int argc, char *argv[]) {

  if (argc < 4) {
    printf("usage: ./ecrivain <tube> <nombre de caracteres> <caractere>\n");
    exit(EXIT_FAILURE);
  }
  
  //printf("Écriture par blocs de taille %d\n", SIZE);

  char buf[SIZE];
  char c = argv[3][0];
  int i, ecr, len = atoi(argv[2]);
  int fd = open(argv[1], O_WRONLY);

  if (fd < 0) exit(EXIT_FAILURE);
  else printf("ouverture du tube effectuée\n"); 

  if (len > SIZE) len = SIZE;
  for (i=0; i<len; i++) buf[i] = c;

  ecr = write(fd, buf, len);

  if (ecr <= 0)
    perror("pb write");
  else
    printf("écriture de %d caractères terminée : %c...\n", ecr, *(buf));

  close(fd);
  return 0;
}
