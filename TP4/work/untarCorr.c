#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "tarutils.h"
#include <sys/stat.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

char buf[BLOCKSIZE];
struct posix_header hd;

/* extraie de l'archive dont fd est un descripteur le fichier décrit par
 * l'entête phd : crée un fichier de nom phd -> name
 * et de contenu constitué des filesize octets suivants de l'archive.
 * Attention à respecter les droits phd -> mode */
int extract(int fd, struct posix_header *phd, unsigned int filesize)
{

  /* TODO : vérification du type de fichier : seuls les REGTYPE et AREGTYPE
   * sont supportés */

  if (phd->typeflag != REGTYPE && phd->typeflag != AREGTYPE)
  {
    return 1;
  }
  unsigned int m;
  sscanf(phd->mode, "%o", &m);
  /* TODO : création du fichier avec les bons droits */
  int f = open(phd->name, O_WRONLY | O_CREAT | O_TRUNC, m);
  if (chmod(phd->name, m) < 0)
    return 1;
  if (f < 0)
  {
    return 1;
  }

  /* TODO : recopie du contenu; ATTENTION au dernier bloc! */
  unsigned int nblocks = (filesize + BLOCKSIZE - 1) / BLOCKSIZE;
  //lseek(fd, nblocks * BLOCKSIZE, SEEK_CUR);
  
   for (size_t i = 0; i < nblocks; i++)
  {
    int r = read(fd, buf, BLOCKSIZE);
    if (r < 0)
    {
      close(f);
      return 1;
    }
    if (r == 0)
    {
      break;
    }
    int w = write(f, buf, r);
    if (w < 0)
    {
      close(f);
      return 1;
    }
  }
  /* TODO : refermer le descripteur */
  close(f);

  return 0;
}

int main(int argc, char **argv)
{
  int ret = 0, all;
  char *filename = NULL;
  struct posix_header *phd = &hd;

  switch (argc)
  {
  case 2:
    all = 1;
    break;
  case 3:
    all = 0;
    filename = argv[2];
    break;
  default:
    fprintf(stderr, "Usage : %s file.tar [file]\n", argv[0]);
    exit(1);
  }

  /* TODO : ouverture du fichier tar */
  int fd_tar = open(argv[1], O_RDONLY);
  if (fd_tar < 0)
  {
    ret = 1;
    return 1;
  }

  /* boucle principale */
  while (1)
  {
    /* TODO : lecture de l'entête; vérifier que la lecture est bien complète */
    int r = read(fd_tar, phd, BLOCKSIZE);
    if (r < 0)
    {
      close(fd_tar);
      ret = 1;
      return 1;
    }

    /* TODO : la fin de l'archive est atteinte si le bloc est rempli de zéros,
     * et en particulier si name vaut "" */
    if (phd->name[0] == '\0')
    {
      break;
    }

    /* vérification de checksum (facultatif) */
    if (!check_checksum(phd))
    {
      fprintf(stderr, "Checksum erronée : %s\n", phd->chksum);
      ret = 1;
      break;
    }

    /* TODO : la taille et le nombre de blocs sont toujours utiles */
    unsigned int filesize;
    unsigned int nblocks;
    sscanf(phd->size, "%o", &filesize);
    nblocks = (filesize + BLOCKSIZE - 1) / BLOCKSIZE;

    /* TODO[1] s'il s'agit du fichier cherché, le désarchiver puis
     * quitter; sinon, sauter à l'entête suivant */
    if (all == 0)
    {
      if (strcmp(phd->name, filename) == 0)
      {
        if (extract(fd_tar, phd, filesize) != 0)
        {
          ret = 1;
        }
        break;
      }
      else
      {
        /* Sauter le fichier non désiré */
        lseek(fd_tar, nblocks * BLOCKSIZE, SEEK_CUR);
      }
    }

    /* TODO[2] s'il s'agit d'une extraction complète (all == 1),
     * désarchiver le fichier et continuer (sauf en cas d'erreur)*/
    /* Extraction complète */
    if (all == 1)
    {
      if (extract(fd_tar, phd, filesize) != 0)
      {
        ret = 1;
        break;
      }
    }
  }
  /* TODO : fermeture du fichier tar */
  close(fd_tar);

  exit(ret);
}
