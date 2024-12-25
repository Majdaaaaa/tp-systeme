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

    /* TODO : création du fichier avec les bons droits */
    unsigned int mode;
    sscanf(phd->mode, "%o", &mode);
    int fdf = open(phd->name, O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (fdf < 0)
    {
        perror("Erreur lors de la création du fichier");
        return 1;
    }

    fchmod(fdf, mode);
    int result;
    /* TODO : recopie du contenu; ATTENTION au dernier bloc! */
    // int r=read(fd,buf,(nblocks-1)*BLOCKSIZE);
    int nblocks = (filesize + BLOCKSIZE - 1) / BLOCKSIZE;
    for (int i = 0; i < (nblocks); i++)
    {
        int r = read(fd, buf, BLOCKSIZE);
        if (r < 0)
        {
            perror("Erreur lors de la lecture du fichier grosminet");
            close(fdf);
            return 1;
        }
        if (r == 0)
        {
            break;
        }
        int result = write(fdf, buf, r);
        if (result < 0)
        {
            perror("Erreur lors de l'écriture du fichier grosminet");
            close(fdf);
            return 1;
        }
    }

    int inter;
    sscanf(phd->size, "%o", &inter);

    //ftruncate(fdf, MIN((nblocks - 1) * BLOCKSIZE, inter));

    /* TODO : refermer le descripteur */
    result = close(fdf);
    if (result < 0)
    {
        perror("Erreur lors de la fermeture du fichier");
        return 1;
    }
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
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("Erreur lors de l'ouverture du fichier tar");
        fprintf(stderr, "Nom du fichier : %s\n", filename);
        exit(1);
    }

    /* boucle principale */
    while (1)
    {
        /* TODO : lecture de l'entête; vérifier que la lecture est bien complète */
        int n = read(fd, phd, BLOCKSIZE);
        if (n < 0)
        {
            perror("Erreur lors de la lecture de l'entête");
            exit(1);
        }
        if (n == 0)
        {
            break;
        }

        /* TODO : la fin de l'archive est atteinte si le bloc est rempli de zéros,
         * et en particulier si name vaut "" */

        // if (strcmp(phd->name,"")==0){
        //  break;
        // }
        // if (phd->name[0]=='\0'){
        //  break;
        // }
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
        int size;
        sscanf(phd->size, "%o", &size);

        int nblocks = (size + BLOCKSIZE - 1) / BLOCKSIZE;

        /* TODO[1] s'il s'agit du fichier cherché, le désarchiver puis
         * quitter; sinon, sauter à l'entête suivant */
        // if (strcmp(phd->name,filename)==0){
        if (strcmp(phd->name, argv[2]) == 0)
        {
            int res = extract(fd, phd, size);
            if (res == 1)
            {
                close(fd);
                exit(1);
            }
            break;
        }
        else
        {
            int res = lseek(fd, nblocks * BLOCKSIZE, SEEK_CUR);
            if (res < 0)
            {
                perror("Erreur lors du déplacement du fichier");
                exit(1);
            }
        }

        /* TODO[2] s'il s'agit d'une extraction complète (all == 1),
         * désarchiver le fichier et continuer (sauf en cas d'erreur)*/
    }

    /* TODO : fermeture du fichier tar */
    int res = close(fd);
    if (res < 0)
    {
        perror("Erreur lors de la fermeture du fichier tar");
        exit(1);
    }

    exit(ret);
}

// int r=read(fd,buf,filesize-BLOCKSIZE);
// if (r<0){
//   perror("Erreur lors de la lecture du fichier grosminet");
//   return 1;
// }

// int result=write(fdf,buf,r);
// // int result=write(fdf,buf,filesize-BLOCKSIZE);
// if (result<0){
//  perror("Erreur lors de l'écriture du fichier grosminet");
//   return 1;
// }