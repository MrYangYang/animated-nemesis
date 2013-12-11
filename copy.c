#include "libops.h"

int main(int argc, char **argv)
{
    // empty & full
    int gesemid, gfsemid;

    // shared mem
    int gshmid;

    char buff[BUFF_SZ];
    char *shmbuf;

    int loo_flag = 1;

    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, +1, SEM_UNDO};

    key_t gekey = getKey(GET_EMPTY_KEY_GEN);
    key_t gfkey = getKey(GET_FULL_KEY_GEN);
    key_t gkey = getKey(GET_SHM_KEY_GEN);

    key_t gmkey = getKey(GET_TO_CP_MSG_KEY);
    int gmsqid = msgget(gmkey, IPC_NOWAIT | IPC_CREAT | 0666);

    // TODO get psemid;
    gesemid = semget(gekey, 1, IPC_CREAT | 0666);
    if(gesemid == -1){
        perror("create semaphores failed.");
        exit(CREATE_SEM_ERROR);
    }
    gfsemid = semget(gfkey, 1, IPC_CREAT | 0666);
    if(gfsemid == -1){
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

    int ret_v;
    mymsg msg;
    while(loo_flag){
        // TODO ops V for get process
        ret_v = msgrcv(gmsqid, &msg, sizeof(mymsg), 0, IPC_NOWAIT);
        if(ret_v != -1 && msg.mtype == 100){
            shmctl(gshmid, IPC_RMID, (struct shmid_ds *)shmbuf);
            semctl(gfsemid, 0, IPC_RMID);
            semctl(gesemid, 0, IPC_RMID);
            msgctl(gmsqid, IPC_RMID, (struct msqid_ds *)(&msg));
            exit(0);
        }semop(gfsemid, &lock, 1);



        memcpy(buff, shmbuf, BUFF_SZ);
        // TODO ops P for get process
        putchar(*buff);
        semop(gesemid, &unlock, 1);

        // TODO copy mem to put process's buffer.
    } 
    return 0;
}
