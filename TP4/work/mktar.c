#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
#include <sys/param.h>
#include <errno.h>
#include "tarutils.h"

char zeros[BLOCKSIZE] = {0};
// char buf[BLOCKSIZE];
// struct posix_header hd;

/* ajoute un fichier de nom filename à l'archive ouverte (fd)
 * retourne 0 en cas de succès, 1 en cas d'échec */
int tar_one_file(int fd, char *filename)
{
    struct stat st;
    struct posix_header hd;         // la structure meme
    struct posix_header *phd = &hd; // le pointeur vers la structure
    int file_fd = -1;
    ssize_t nwritten;
    char *buf = NULL;

    /* TODO : création de l'entête; commencer par le remplir de 0, puis
     * renseigner au moins les champs indispensables : typeflag, name,
     * mode, size, et _EN DERNIER_ checksum.
     * Pour typeflag, mode et size, utiliser stat() */
    if (stat(filename, &st) < 0)
    {
        perror("erreur stat");
        goto error;
    }

    if (strlen(filename) <= sizeof(phd->name)) //* Si c'est plus petit ou égal alors on peut le mettre
    {
        strcpy(phd->name, filename);
    }

    sprintf(phd->mode, "%7o", st.st_mode & 0777);

    if (S_ISREG(st.st_mode))
    {
        phd->typeflag = REGTYPE;
        if (st.st_size < 0 || st.st_size > 0777777777777L) //* verifie que la taille du fichier est valide : pas négatif et ne dépasse pas 0777777777777L
        {
            goto error;
        }
        sprintf(phd->size, "%2lo", st.st_size);
    }
    else
    {
        dprintf(2, "erreur: type de fichier non supporté"); //* on accepte que les fichier ordinaire
    }

    /* calcul de checksum : indispensable pour gnu tar :-( */
    set_checksum(phd);

    /* TODO : copie de l'entête dans l'archive */
    nwritten = write(fd, phd, BLOCKSIZE);
    if (nwritten != BLOCKSIZE)
    {
        goto error;
    }
    /* TODO : ouverture du fichier à copier */

    buf = malloc(BLOCKSIZE);
    if (buf == NULL)
        goto error;

    file_fd = open(filename, O_RDONLY);
    if (file_fd < 0)
        goto error;

    /* TODO : copie du fichier; ATTENTION à compléter le dernier bloc si
     * nécessaire */
    int partial_read = 0;

    while (1)
    {
        memset(buf, 0, BLOCKSIZE);
        ssize_t nread = read(file_fd, buf, BLOCKSIZE);
        if (nread < 0)
            goto error;
        else if (nread == 0)
            break;
        // Si on lit moins que 512 octet alors c'est la derniere itération
        if (partial_read)
            goto error; //* On aura deja ecrit le dernier bloc quand on arrive ici
        if (nread < BLOCKSIZE)
            partial_read = 1;
        nwritten = write(fd, buf, BLOCKSIZE);
        if (nwritten != BLOCKSIZE)
            goto error;
    }
    /* TODO : fermeture du fichier copié */
    close(file_fd);
    free(buf);
    return 0;
error:
    if (errno != 0)
        perror("tar_one_file");
    errno = 0;
    if (file_fd >= 0)
        close(fd);
    if (buf)
        free(buf);
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage : %s file.tar f1 [... fn]\n", argv[0]);
        exit(1);
    }

    /* TODO : création du fichier d'archive */
    int tar_fd = -1;
    tar_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (tar_fd < 0)
    {
        goto error;
    }

    /* TODO : boucle principale pour ajouter chaque argv[i] l'un après
     * l'autre */
    for (int i = 2; i < argc; i++)
    {
        if (tar_one_file(tar_fd, argv[i]) != 0)
        {
            goto error;
        }
    }

    /* TODO : finalisation : ajout de deux blocs vides */
    //* Les deux blocs vides ne sont pas associés au dernier 

    for (int i = 0; i < 2; i++)
    {
        if (write(tar_fd, zeros, BLOCKSIZE) != BLOCKSIZE)
        {
            goto error;
        }
    }
    /* TODO : fermeture du fichier tar */
    close(tar_fd);
    return 0;
error:
    if (tar_fd >= 0)
        close(tar_fd);
    return 1;
}
