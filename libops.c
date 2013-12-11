#include "libops.h"

key_t getKey(int token)
{
    key_t mkey;
    mkey = ftok(".", token);
    if(mkey < 0){
        perror("generate key failed. exit");
        exit(GEN_KEY_ERROR);
    }
    return mkey;
}
