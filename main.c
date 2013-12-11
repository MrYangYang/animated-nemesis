#include "libops.h"

int main(int argc, char **argv)
{
    pid_t chld_pid;
    chld_pid = fork();
    if(chld_pid == -1){
        printf("fork error\n");
        exit(FORK_ERROR);
    }

    // parent proc.
    if(chld_pid){
        chld_pid = fork();
        if(chld_pid == -1){
            printf("fork error\n");
            exit(FORK_ERROR);
        }
        
        // parent proc
        if(chld_pid){
            execv("./get", NULL);
        // else child proc
        } else {
            execv("./copy", NULL);
        }
    
    // else child proc
    } else {
        execv("./put", NULL);
    }

    return 0;
}
