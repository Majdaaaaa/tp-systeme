#define DEFAULT_SOURCE
#include <fcntl.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 4 * PIPE_BUF

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("usage: ./lecteur <tube>\n");
    exit(EXIT_FAILURE);
  }

  char buf[SIZE];
  int i, cpt = 0, lus = 1, fd = open(argv[1], O_RDONLY);
  char last = '\0';
  if (fd < 0)
    exit(1);

  /* pour laisser le temps à tous les écrivains d'ouvrir le tube
   * (donc éviter un arrêt prématuré du lecteur faute d'écrivains) */
  sleep(1);

  while (lus > 0)
  {
    lus = read(fd, buf, SIZE);
    for (i = 0; i < lus; i++)
    {
      if (buf[i] == last)
        cpt++;
      else
      {
        if (cpt > 0)
          printf("%c : %d\n", last, cpt);
        cpt = 1;
        last = buf[i];
      }
    }
  }
  if (cpt > 0)
    printf("%c : %d\n", last, cpt);

  return 0;
}
