#ifndef _redis_h
#define _redis_h


#include "vm.h"
#include "store.h"


typedef struct {
    int count;
    int capacity;
    Store** stores;
    VM** vms;
    char** buffers;
} Redis;




void init_redis();
int create_instance();
void free_instance(int id);
char* query(int id, char* query);

Redis main_redis;

#endif