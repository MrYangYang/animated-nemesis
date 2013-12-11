#include "libops.h"

int main(int argc, char **argv)
{
    int shmid;

    // empty & full
    int esemid, fsemid;
    char *shmbuf;

    // msg queue to copy
    int msqid;
    key_t msgkey = getKey(GET_TO_CP_MSG_KEY);
    msqid = msgget(msgkey, IPC_CREAT|IPC_NOWAIT|0666);

    FILE *f = NULL;
    f = fopen("a.txt", "r");

    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, 1, SEM_UNDO};
    
    key_t ekey = getKey(GET_EMPTY_KEY_GEN);
    key_t fkey = getKey(GET_FULL_KEY_GEN);
    key_t mkey = getKey(GET_SHM_KEY_GEN);

    shmid = shmget(mkey, BUFF_SZ, IPC_CREAT|0666);
    if(shmid == -1){
        perror("create shared memory failed.");
        exit(CREATE_SHM_ERROR);
    }

    // TODO create semaphores
    esemid = semget(ekey, 1, IPC_CREAT|0666);
    if(esemid == -1){
        perror("create semaphores failed");
        exit(CREATE_SEM_ERROR);
    }

    fsemid = semget(fkey, 1, IPC_CREAT|0666);
    if(fsemid == -1){
        perror("create semaphores failed");;
        exit(CREATE_SEM_ERROR);
    }

    printf("shmid %d, esemid %d, fsemid %d\n", shmid, esemid, fsemid);

    // map shm
    shmbuf = (char *)shmat(shmid, 0, 0);

    // semop(fsemid, &unlock, 1);

    while(!feof(f)){
        // TODO ops P 
        semop(fsemid, &unlock, 1);
        semop(esemid, &lock, 1);
        fread(shmbuf, BUFF_SZ, 1, f);
    }

    // TODO send msg to copy process. tell it to exit.
    mymsg msg;
    msg.mtype = 100;
    msgsnd(msqid, &msg, sizeof(mymsg), IPC_NOWAIT);
    semop(fsemid, &unlock, 1);

    fclose(f);
    return 0;
}
