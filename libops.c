#include "libops.h"

key_t getPutKey(int token)
{
    key_t mkey;
    mkey = ftok(".", token);
    if(mkey < 0){
        perror("generate put key failed. exit");
        exit(GEN_KEY_ERROR);
    }

    return mkey;
}

key_t getGetKey(int token)
{
    key_t mkey;
    mkey = ftok(".", token);
    if(mkey < 0){
        perror("generate put key failed. exit");
        exit(GEN_KEY_ERROR);
    }
    return mkey;
}
