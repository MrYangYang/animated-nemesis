#ifndef _LIB_OPS_H_ 
#define _LIB_OPS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


// msg type, tell process to exit
#define MYMSG_TYPE_EXIT 1

// get key gen num
#define GET_EMPTY_KEY_GEN 1
#define GET_FULL_KEY_GEN 2
#define PUT_EMPTY_KEY_GEN 3
#define PUT_FULL_KEY_GEN 4
#define GET_SHM_KEY_GEN 5
#define PUT_SHM_KEY_GEN 6
#define GET_TO_CP_MSG_KEY 7
#define COPY_TO_PUT_MSG_KEY 8


#define GEN_KEY_ERROR -1
#define CREATE_SHM_ERROR -2
#define OPEN_FILE_ERROR -3
#define CREATE_SEM_ERROR -4
#define OPS_SEM_ERROR -5
#define CREATE_MSQ_ERROR -6

#define BUFF_SZ 1

typedef struct _mymsg
{
    long mtype;
} mymsg;

// use a number (0-255) to get a unique key.
key_t getKey(int token);

#endif
