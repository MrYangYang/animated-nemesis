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
#define GET_KEY_GEN 1
#define PUT_KEY_GEN 2

#define GEN_KEY_ERROR -1
#define CREATE_SHM_ERROR -2
#define OPEN_FILE_ERROR -3
#define CREATE_SEM_ERROR -4
#define OPS_SEM_ERROR -5

#define BUFF_SZ 1

typedef struct _mymsg
{
    long mtype;
} mymsg;

// P ops.
int P(int semid);

// V ops
int V(int semid);

// get put process key_t
key_t getPutKey();

// get GET process key_t
key_t getGetKey();
#endif
