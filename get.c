#include "libops.h"

int main(int argc, char **argv)
{
    int shmid;
    int semid;
    char *shmbuf;
    key_t mkey = getGetKey();

    // identifer of opened file (a.txt)
    int fid;
    shmid = shmget(mkey, BUFF_SZ, IPC_CREAT|0666);
    if(shmid == -1){
        perror("create shared memory failed.");
        exit(CREATE_SHM_ERROR);
    }

    // TODO create semaphores
    semid = semget(mkey, 1, IPC_CREAT|0666);
    if(semid == -1){
        perror("create semaphores failed");
        exit(CREATE_SEM_ERROR);
    }

    // TODO ops V.
    if(V(semid) == -1){
        perror("ops V error");
        exit(OPS_SEM_ERROR);
    }

    // map shm
    shmbuf = (char *)shmat(shmid, 0, 0);
    // open a.txt
    fid = open("a.txt", O_RDONLY);

    if(fid == -1){
        perror("open file for read failed.");
        exit(OPEN_FILE_ERROR);
    }

    while(read(fid, shmbuf, BUFF_SZ) != -1)
        // TODO ops P;
        P(semid);
        V(semid);
    }

    // TODO send msg to copy process. tell it to exit.

    close(fid);
    return 0;
}
