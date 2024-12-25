#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int j = 1;
    ;
    pid_t r;
    // On veut que le père affiche 1 et le dernier affiche 18, dans l'ordre croissant, et
    dprintf(1, "Louis %u\n", j);
    j++;
    for (int i = j; i < 19; i++)
    {
        if (r = fork() > 0)
        {
            dprintf(1, "Louis %u\n", i);
        }
        else if (r == 0)
        {
            wait(NULL);
            exit(0);
        }
        else
        {
            perror("fork");
        }
    }
    dprintf(1,"et puis plus personne plus rien…\nQu’est-ce que c'est que ces gens-là\nqui ne sont pas foutus\nde compter jusqu’à vingt ?\nJacques Prévert");
}


//! version amenah / version prof
// int main(int arc,char*argv){
//     int i =0;
//     int p;
//     for (int i=1;i<19;i++){
//         p=fork();
//         if (p>0){
//             wait(NULL); //pour avoir les exit dans l'ordre flèche bleu voir photo
//             exit(0);
//         }
//         else if( p==0){
//             dprintf(1,"Louis %u\n",i);
            
//         }
//     }


// }