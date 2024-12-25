#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>

#include <stdio.h>

#define BUFFER_SIZE 10

char usage[] = " prend trois paramètres\n";

int do_sed(int fd, int del_char, char car1, char car2);

/* ./my-sed car1 car2 filename remplace chaque occurrence du caractère
 * car1 par car2 dans le fichier de référence filename.
 * ./my-sed -d car1 filename supprime les occurrences du caractère car1
 * dans le fichier de référence filename. */
int main(int argc, char *argv[])
{
  int result;
  int del_char = 0;
  char car1, car2 = '\0';
  int fd;

  if (argc != 4)
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
      result = 1;
      goto cleanup_return;
    }
  }
  if (del_char)
  {
    // Cas avec option -d (suppression du caractère car1)
    if (argc - optind != 2)
    {
      result = 1;
      goto cleanup_return;
    }
    car1 = argv[optind][0];              // Caractère à supprimer
    fd = open(argv[optind + 1], O_RDWR); // Fichier à modifier
  }
  else
  {
    // Cas sans option -d (remplacement de car1 par car2)
    if (argc - optind != 3)
    {
      result = 1;
      goto cleanup_return;
    }
    car1 = argv[optind][0];              // Caractère à remplacer
    car2 = argv[optind + 1][0];          // Caractère de remplacement
    fd = open(argv[optind + 2], O_RDWR); // Fichier à modifier
  }

  /* TODO: ouvrir en lecture/écriture le fichier dont la référence est
   * passée en paramètre. L'ouverture échoue si le fichier n'est pas
   * accessible */
  if (fd < 0)
  {
    result = 1;
    goto cleanup_return;
  }
  /* TODO: appeler do_sed avec les bons arguments */
  result = do_sed(fd, del_char, car1, car2);
  /* TODO: fermer le fichier */
  close(fd);

cleanup_return:
  close(fd);
  return result;
}

int do_sed(int fd, int del_char, char car1, char car2)
{
  int result = 0;
  char *buf_in = NULL, *buf_out = NULL; /* buffers de lecture et d'écriture */
  int lect = 0, ecr = 0;                /* respectivement nombre d'octets lus et nombre
                                           d'octets écrits dans le fichier */

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
    /* TODO: placer la tête de lecture au bon endroit : pos actuelle - ce que j'ai écrit*/
    lseek(fd, ecr, SEEK_SET);

    /* TODO:  lire dans le fichier (tant qu'il y a quelque chose à lire) */
    int r = read(fd, buf_in, BUFFER_SIZE);
    if (r < 0)
    {
      result = 1;
      goto cleanup_return;
    }
    if (r == 0)
      break;
    /* TODO: mettre à jour la variable lect */
    lect += r;

    /* TODO: écrire dans le buffer d'écriture l'entrée modifiée selon la
     * valeur de del_char */
    int j = 0;
    for (int i = 0; i < r; i++)
    {
      if (del_char == 1)
      {
        if (buf_in[i] != car1)
        {
          buf_out[j++] = buf_in[i];
        }
      }
      else
      {
        if (buf_in[i] == car1)
        {
          buf_out[j++] = car2;
        }
        else
        {
          buf_out[j++] = buf_in[i];
        }
      }
    }

    /* TODO: placer la tête d'écriture au bon endroit : pos actuelle - ce que j'ai lu*/
    lseek(fd, lect-r, SEEK_SET);

    /* TODO: écrire le résultat dans le fichier */
    int w = write(fd, buf_out, j);
    if (w < 0)
    {
      result = 1;
      goto cleanup_return;
    }

    /* TODO: mettre à jour la variable ecr */
    ecr += w;
  }

  if (del_char && ecr < lect)
  {
    /* TODO: tronquer la fin du fichier */
    int f = ftruncate(fd, ecr);
    if (f < 0)
    {
      result = 1;
      goto cleanup_return;
    }
  }

cleanup_return:
  /* TODO: libérer les buffers */
  if (buf_in != NULL)
    free(buf_in);
  if (buf_out != NULL)
    free(buf_out);
  return result;
}
