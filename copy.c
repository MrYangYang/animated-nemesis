#include "libops.h"

int main(int argc, char **argv)
{
    // empty & full
    int gesemid, gfsemid;
    int pesemid, pfsemid;

    // shared mem
    int gshmid;
    int pshmid;

    char *gshmbuf;
    char *pshmbuf;

    int loo_flag = 1;

    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, +1, SEM_UNDO};

    key_t gekey = getKey(GET_EMPTY_KEY_GEN);
    key_t gfkey = getKey(GET_FULL_KEY_GEN);
    key_t gkey = getKey(GET_SHM_KEY_GEN);

    key_t pekey = getKey(PUT_EMPTY_KEY_GEN);
    key_t pfkey = getKey(PUT_FULL_KEY_GEN);
    key_t pshmkey = getKey(PUT_SHM_KEY_GEN);
    key_t pmkey = getKey(COPY_TO_PUT_MSG_KEY);
    key_t gmkey = getKey(GET_TO_CP_MSG_KEY);

    int gmsqid = msgget(gmkey, IPC_NOWAIT | IPC_CREAT | 0666);
    int pmsqid = msgget(pmkey, IPC_NOWAIT | IPC_CREAT | 0666);

    // get put.c semids
    pesemid = semget(pekey, 1, IPC_CREAT | 0666);
    if(pesemid == -1){
        exit(CREATE_SEM_ERROR);
    }

    pfsemid = semget(pfkey, 1, IPC_CREAT | 0666);
    if(pfsemid == -1){
        semctl(pesemid, 0, IPC_RMID);
        exit(CREATE_SEM_ERROR);
    }

    gesemid = semget(gekey, 1, IPC_CREAT | 0666);
    if(gesemid == -1){
        semctl(pesemid, 0, IPC_RMID);
        semctl(pfsemid, 0, IPC_RMID);
        exit(CREATE_SEM_ERROR);
    }
    gfsemid = semget(gfkey, 1, IPC_CREAT | 0666);
    if(gfsemid == -1){
        semctl(pfsemid, 0, IPC_RMID);
        semctl(pesemid, 0, IPC_RMID);
        semctl(gesemid, 0, IPC_RMID);
        exit(CREATE_SEM_ERROR);
    }



    // TODO get gshm
    gshmid = shmget(gkey, BUFF_SZ, IPC_CREAT|0666);
    if(gshmid == -1){
        semctl(gesemid, 0, IPC_RMID);
        semctl(gfsemid, 0, IPC_RMID);
        semctl(pesemid, 0, IPC_RMID);
        semctl(pfsemid, 0, IPC_RMID);
        exit(CREATE_SHM_ERROR);
    }
    gshmbuf = (char *)shmat(gshmid, 0, 0);

    pshmid = shmget(pshmkey, BUFF_SZ, IPC_CREAT|0666);
    if(pshmid == -1){
        semctl(pfsemid, 0, IPC_RMID);
        semctl(pesemid, 0, IPC_RMID);
        semctl(gesemid, 0, IPC_RMID);
        semctl(gfsemid, 0, IPC_RMID);
        shmctl(gshmid, IPC_RMID, (struct shmid_ds *)gshmbuf);
    }
    pshmbuf = (char *)shmat(pshmid, 0, 0);

    int ret_v;
    mymsg msg;
    // semop(pfsemid, &unlock, 1);
    while(loo_flag){
        // TODO ops V for get process
        
        semop(pfsemid, &unlock, 1);
        semop(gfsemid, &lock, 1);
        semop(pesemid, &lock, 1);
        memcpy(pshmbuf, gshmbuf, BUFF_SZ);
        ret_v = msgrcv(gmsqid, &msg, sizeof(mymsg), 0, IPC_NOWAIT);
        if(ret_v != -1 && msg.mtype == 100){
            shmctl(gshmid, IPC_RMID, (struct shmid_ds *)gshmbuf);
            semctl(gfsemid, 0, IPC_RMID);
            semctl(gesemid, 0, IPC_RMID);
            msgctl(gmsqid, IPC_RMID, (struct msqid_ds *)(&msg));

            msgsnd(pmsqid, &msg, sizeof(mymsg), IPC_NOWAIT);
            semop(pfsemid, &unlock, 1);
            exit(0);
        }

        // semop(pesemid, &lock, 1);

        // memcpy(pshmbuf, gshmbuf, BUFF_SZ);

        semop(gesemid, &unlock, 1);
    } 
    return 0;
}
