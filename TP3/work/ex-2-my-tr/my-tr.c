#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>

#include <stdio.h>

#define BUFFER_SIZE 1024

char usage[] = " prend deux paramètres\n";

int do_tr(int del_char, char car1, char car2);

/* ./my-tr car1 car2 recopie l'entrée standard sur la sortie standard en
 * remplaçant chaque occurrence du caractère car1 par car2.
 * ./my-tr -d car1 recopie l'entrée standard sur la sortie standard en
 * supprimant les occurrences du caractère car1. */
int main(int argc, char *argv[])
{
  int result;
  int del_char = 0;
  char car1, car2 = '\0';

  if (argc != 3)
  {
    write(STDERR_FILENO, argv[0], strlen(argv[0]));
    write(STDERR_FILENO, usage, strlen(usage));
    result = 1;
    goto cleanup_return;
  }

  int opt;
  while ((opt = getopt(argc, argv, "d")) != -1)
  {
    if (opt == 'd')
    {
      del_char = 1;
    }
    else
    {
      write(STDERR_FILENO, usage, strlen(usage));
      return 1;
    }
  }

  // Determine characters from command line
  if (optind < argc)
  {
    car1 = argv[optind][0];
    if (optind + 1 < argc)
    {
      car2 = argv[optind + 1][0];
    }
  }
  else
  {
    write(STDERR_FILENO, usage, strlen(usage));
    return 1;
  }

  /* TODO: appeler do_tr avec les bons arguments */
  result = do_tr(del_char, car1, car2);

  cleanup_return:
    return result;
}

/* recopie l'entrée standard sur la sortie standard en :
   - remplaçant chaque occurrence du caractère car1 par car2 si del_char = 0,
   - supprimant les occurrences du caractère car1 si del_char = 1 */
int do_tr(int del_char, char car1, char car2)
{
  int result = 0;
  char *buf_in = NULL, *buf_out = NULL; /* buffers de lecture et d'écriture */

  /* TODO: allouer les buffers de lecture et d'écriture */
  buf_in = malloc(BUFFER_SIZE);
  if (buf_in == NULL)
  {
    result = 1;
    goto cleanup_return;
  }
  buf_out = malloc(BUFFER_SIZE);
  if (buf_out == NULL)
  {
    result = 1;
    goto cleanup_return;
  }
  while (1)
  {
    /* TODO: lire l'entrée (tant qu'il y a quelque chose à lire) */
    int r = read(0, buf_in, BUFFER_SIZE);
    if (r < 0)
    {
      result = 1;
      goto cleanup_return;
    }
    if (r == 0)
      break;
    /* TODO: écrire dans le buffer d'écriture l'entrée modifiée
     * selon la valeur de del_char */
    int j = 0;
    for (int i = 0; i < r; i++)
    {
      if (del_char == 0)
      {
        /* Remplacer car1 par car2 */
        if (buf_in[i] == car1)
        {
          buf_out[j++] = car2;
        }
        else
        {
          buf_out[j++] = buf_in[i];
        }
      }
      else
      {
        /* Supprimer les occurrences de car1 */
        if (buf_in[i] != car1)
        {
          buf_out[j++] = buf_in[i];
        }
      }
    }

    /* TODO: écrire le résultat sur la sortie standard */
    ssize_t w = write(STDOUT_FILENO, buf_out, j);
    if (w < 0)
    {
      result = 1; /* Erreur d'écriture */
      goto cleanup_return;
    }
  }

  cleanup_return:
    /* TODO: libérer les buffers */
    free(buf_in);
    free(buf_out);
  return result;
}
