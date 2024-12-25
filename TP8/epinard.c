#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    pid_t child_pid;
    int cmp = 9;

    dprintf(1, "Mange tes épinard!\n");
    child_pid = fork();
    for (int i = 0; i < cmp; i++)
    {
        if (child_pid > 0)
        { // parent
            wait(NULL);
            dprintf(1,"C'est bien, Popeye! Tu seras fort comme papa.\n");
            exit(0);
        }else if(child_pid==0){
            for(int i=0;i<cmp;i++){
                dprintf(1,"Non!\n");
            }  
            dprintf(1,"Oui pôpa...\n");
            exit(0);
        }
    }

}