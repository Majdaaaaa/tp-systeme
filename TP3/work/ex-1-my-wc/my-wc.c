#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>

#include <stdio.h>

#define BUFFER_SIZE 1024

int do_wc(int in_fd, int out_fd, int write_newlines, int write_bytes, const char *in_filename);
int print_size(int fd, size_t size);

/* ./my-wc [-lc] [fichier] compte le nombre de retours à la ligne et/ou
 * d'octets dans le fichier indiqué, ou à défaut sur l'entrée standard. */
int main(int argc, char *argv[])
{
  int result;
  char *in_filename = NULL;
  int write_newlines = 0;
  int write_bytes = 0;

  int in_fd = STDIN_FILENO;

  int opt;
  while ((opt = getopt(argc, argv, "cl")) != -1)
  {
    /* on gère les options -c et -l */
    switch (opt)
    {
    case 'l':
      write_newlines = 1;
      break;
    case 'c':
      write_bytes = 1;
      break;
    case '?':
      result = 1;
      goto cleanup_return;
      break;
    }
  }
  /* si ni -c ni -l n'a été donnée, on fait en sorte d'afficher les deux */
  if (!write_newlines && !write_bytes)
  {
    write_newlines = 1;
    write_bytes = 1;
  }

  /* on teste si un nom de fichier a été donné (cf. man getopt -> optind)
   * et dans ce cas, on le met dans in_filename */
  if (optind < argc)
    in_filename = argv[optind];

  /* TODO: si un nom de fichier autre que "-" a été donné, ouvrir le fichier
   * correspondant */
  if (in_filename && strcmp(in_filename, "-"))
    in_fd = open(in_filename, O_RDONLY);
  if (in_fd < 0)
  {
    result = 1;
    goto cleanup_return;
  }
  /* TODO: appeler do_wc avec les bons arguments */
  result = do_wc(in_fd, STDOUT_FILENO, write_newlines, write_bytes, in_filename);
cleanup_return:
  /* TODO: fermer le fichier d'entrée. Ne pas oublier de tester s'il y a eu
   * une erreur */
  if (in_fd > 0)
    close(in_fd);
  return result;
}

/* Lit in_fd et compte le nombre de lignes et/ou d'octets lus. Affiche le
 * résultat sur out_fd, suivi de in_filename si différent de NULL, le
 * tout sous formes de colonnes séparées par une tabulation. Renvoie 0 en
 * cas de succès et 1 en cas d'erreur. */
int do_wc(int in_fd, int out_fd, int write_newlines, int write_bytes, const char *in_filename)
{
  int result;
  char *buffer = NULL;
  size_t newlines_count = 0;
  size_t bytes_count = 0;
  int nr;
  /* TODO: allouer un buffer */
  buffer = malloc(BUFFER_SIZE);
  if (buffer == NULL)
  {
    result = 1;
    goto cleanup_return;
  }
  while (1)
  {
    /* TODO: lire l'entrée et compter le nombre de retours à la ligne et
     * d'octets */
    nr = read(in_fd, buffer, BUFFER_SIZE);
    if (nr < 0)
    {
      result = 1;
      goto cleanup_return;
    }
    else if (nr == 0)
    {
      break;
    }
    else
    {
      bytes_count += nr;
      for (int i = 0; i < nr; i++)
      {
        if (buffer[i] == '\n')
        {
          newlines_count += 1;
        }
      }
    }
  }

  /* TODO: afficher ce qu'il faut */
  if (write_newlines == 1 && write_bytes == 1)
  {
    /* Afficher le nombre de lignes et d'octets avec tabulation */
    if (in_filename != NULL)
    {
      dprintf(out_fd, "%ld\t%ld\t%s\n", newlines_count, bytes_count, in_filename);
    }
    else
    {
      dprintf(out_fd, "%ld\t%ld\n", newlines_count, bytes_count);
    }
  }
  else
  {
    if (write_newlines == 1)
    {
      /* Afficher uniquement le nombre de lignes */
      if (in_filename != NULL)
      {
        dprintf(out_fd, "%ld\t%s\n", newlines_count, in_filename);
      }
      else
      {
        dprintf(out_fd, "%ld\n", newlines_count);
      }
    }
    if (write_bytes == 1)
    {
      /* Afficher uniquement le nombre d'octets */
      if (in_filename != NULL)
      {
        dprintf(out_fd, "%ld\t%s\n", bytes_count, in_filename);
      }
      else
      {
        dprintf(out_fd, "%ld\n", bytes_count);
      }
    }
  }
  /* TODO: tout a fonctionné : mettre à jour result en conséquence */
  result = 0;
cleanup_return:
  /* TODO: libérer le buffer */
  if (buffer != NULL)
    free(buffer);
  return result;
}

/* Renvoie le nombre d'octets écrits, ou -1 en cas d'erreur. */
// int print_size(int fd, size_t size) {
/* TODO: Utiliser snprintf() avec le format %zu. */

//}
