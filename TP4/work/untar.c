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

    int ret = 0;
    /* TODO : vérification du type de fichier : seuls les REGTYPE et AREGTYPE
     * sont supportés */
    if (phd->typeflag != REGTYPE && phd->typeflag != AREGTYPE)
    {
        return 1;
    }

    /* TODO : création du fichier avec les bons droits */
    int m;
    sscanf(phd->mode, "%o", &m);
    int ff = open(phd->name, O_CREAT | O_TRUNC | O_WRONLY, m);
    if (ff < 0)
    {
        fprintf(stderr, "erreur open %s\n", phd->name);
        return 1;
    }
    if (fchmod(ff, m) < 0)
    {
        close(ff);
        perror("erreur fchmod");
        return 1;
    }

    /* TODO : recopie du contenu; ATTENTION au dernier bloc! */
    int nblocks = (filesize + BLOCKSIZE - 1) / BLOCKSIZE;
    for (size_t i = 0; i < nblocks; i++)
    {
       // int to_read = MIN(filesize, BLOCKSIZE);
        int r = read(fd, buf, BLOCKSIZE/* to_read */);
        if (r < 0)
        {
            ret = 1;
            perror("erreur read extract");
            break;
        }
        if (r == 0)
        {
            break;
        }
        int w = write(ff, buf, r);
        if (w < 0)
        {
            perror("erreur write");
            ret = 1;
            break;
        }
        //filesize -= ret;
        //? au lieu de faire ftruncate, to_read
    }

    ftruncate(ff,MIN(nblocks*BLOCKSIZE,filesize)); //*Truncature pour pas garder un gros fichier rempli de vide

    /* TODO : refermer le descripteur */
    close(ff);

    return ret;
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
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        ret = 1;
        perror("open fichier tar");
        return 1;
    }

    /*boucle principale */
    while (1)
    {
        /* TODO : lecture de l'entête; vérifier que la lecture est bien complète */
        int re = read(fd, phd, BLOCKSIZE);
        if (re < 0)
        {
            perror("erreur lecture entete");
            ret = 1;
            break;
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
        int filesize;
        sscanf(phd->size, "%o", &filesize);
        int nblocks = (filesize + BLOCKSIZE - 1) / BLOCKSIZE;

        /* TODO[1] s'il s'agit du fichier cherché, le désarchiver puis
         * quitter; sinon, sauter à l'entête suivant */
        if (all == 0)
        {
            if (strcmp(phd->name, filename) == 0)
            {
                if (extract(fd, phd, filesize) != 0)
                {
                    ret = 1;
                }
                break;
            }
            else
            {
                lseek(fd, nblocks * BLOCKSIZE, SEEK_CUR);
            }
        }

        /* TODO[2] s'il s'agit d'une extraction complète (all == 1),
         * désarchiver le fichier et continuer (sauf en cas d'erreur)*/
        if (all == 1)
        {
            if (extract(fd, phd, filesize) != 0)
            {
                ret = 1;
                break;
            }
            //lseek(fd, nblocks * BLOCKSIZE, SEEK_CUR); //=> ca c'est NON
        }
    }

    /* TODO : fermeture du fichier tar */
    close(fd);

    exit(ret);
}
