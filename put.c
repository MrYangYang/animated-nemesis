#include "libops.h"

int main(void)
{
    int fsemid, esemid;
    int shmid;
    int msqid;

    char *shmbuf;
    mymsg msgobj;

    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, 1, SEM_UNDO};

    // create key.
    key_t fkey = getKey(PUT_FULL_KEY_GEN);
    key_t ekey = getKey(PUT_EMPTY_KEY_GEN);
    key_t shmkey = getKey(PUT_SHM_KEY_GEN);
    key_t msgkey = getKey(COPY_TO_PUT_MSG_KEY);

    // get sem.
    fsemid = semget(fkey, IPC_CREAT|0666);
    if(fsemid == -1){
        exit(CREATE_SEM_ERROR);
    }
    esemid = semget(ekey, IPC_CREAT|0666);
    if(esemid == -1){
        semctl(fsemid, IPC_RMID, 0);
        exit(CREATE_SEM_ERROR);
    }

    // get shm
    shmid = shmget(shmkey, BUFF_Z, IPC_CREAT|0666|IPC_NOWAIT);
    if(shmid == -1){
        semctl(fsemid, IPC_RMID, 0);
        semctl(esemid, IPC_RMID, 0);
        exit(CREATE_SHM_ERROR);
    }

    // map shm
    shmbuf = (char *)shmat(shmid, 0, IPC_NOWAIT);
    if(shmbuf == -1){
        semctl(esemid, IPC_RMID, 0);
        semctl(fsemid, IPC_RMID, 0);
        shmctl(shmid, IPC_RMID, (struct shmid_ds *)shmbuf);
        exit(CREATE_SHM_ERROR);
    }

    // get msg queue
    msqid = msgget(msgkey, IPC_CREAT|IPC_NOWAIT|0666);
    if(msqid == -1){
        semctl(esemid, IPC_RMID, 0);
        semctl(fsemid, IPC_RMID, 0);
        shmctl(shmid, IPC_RMID, (struct shmid_ds *)shmbuf);
        exit(CREATE_MSQ_ERROR);
    }

    // loop, put buffer data to file.
    int ret_v;
    while(1){
        // work is done.
        ret_v = msgrcv(msqid, &msgobj, sizeof(mymsg), 0, IPC_NOWAIT);
        if(ret_v != -1 && msgobj.mtype = MSG_TYPE_EXIT){
            semctl(fsemid, IPC_RMID, 0);
            semctl(esemid, IPC_RMID, 0);
            shmctl(shmid, IPC_RMID, (struct shmid_ds *)shmbuf);
            msgctl(msqid, IPC_RMID, 0);
            exit(0);
        }

        // lock full
        ret_v = semop(fsemid, &lock, 1);
        if(ret_v == -1){
            semctl(fsemid, IPC_RMID, 0);
            semctl(esemid, IPC_RMID, 0);
            shmctl(shmid, IPC_RMID, (struct shmid_ds *)shmbuf);
            msgctl(msqid, IPC_RMID, 0);
            exit(OPS_SEM_ERROR);
        }

        // TODO output data.

        // unlock empty
        ret_v = semop(esemid, &unlock, 1);
        if(ret_v == -1){
            semctl(esemid, IPC_RMID, 0);
            semctl(fsemid, IPC_RMID, 0);
            shmctl(shmid, IPC_RMID, (struct shmid_ds *)shmbuf);
            msgctl(msqid, IPC_RMID, 0);
            exit(OPS_SEM_ERROR);
        }
    }
}
