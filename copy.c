#include "libops.h"

int main(int argc, char **argv)
{
    int gsemid;
    int psemid;

    int gshmid;
    int pshmid;

    char buff[BUFF_SZ];
    char *shmbuf;

    int loo_flag = 1;
    key_t gkey = getGetKey();
    key_t pkey = getPutKey();

    // TODO get psemid;
    gsemid = semget(get_key, gkey, IPC_CREAT | 0666);
    if(gsemid == -1){
        perror("create semaphores failed.");
        exit(CREATE_SEM_ERROR);
    }

    // TODO get gshm
    gshmid = shmget(gkey, BUFF_SZ, IPC_CREAT|0666);
    if(gshmid == -1){
        perror("create shm failed.");
        exit(CREATE_SHM_ERROR);
    }

    shmbuf = (char *)shmat(gshmid, 0, 0);
    
    while(loo_flag){
        // TODO ops V for get process
        if(V(gsemid) == -1){
            perror("ops v error");
            exit(OPS_SEM_ERROR);
        }

        memcpy(buff, shmbuf, BUFF_SZ);
        // TODO ops P for get process
        if(P(gsemid) == -1){
            perror("ops p error");
            exit(OPS_SEM_ERROR);
        }

        // TODO copy mem to put process's buffer.
        putchar(*buff);
    } 
}
