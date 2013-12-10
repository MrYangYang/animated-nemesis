#include "libops.h"

int P(int semid)
{
    struct sembuf sops = {0, 1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}

int V(int semid)
{
    struct sembuf sops = {0, -1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}

key_t getPutKey()
{
    key_t mkey;
    mkey = ftok(".", PUT_KEY_GEN);
    if(mkey < 0){
        perror("generate put key failed. exit");
        exit(GEN_KEY_ERROR);
    }

    return mkey;
}

key_t getGetKey()
{
    key_t mkey;
    mkey = ftok(".", GET_KEY_GEN);
    if(mkey < 0){
        perror("generate put key failed. exit");
        exit(GEN_KEY_ERROR);
    }
    return mkey;
}
