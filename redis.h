#ifndef _redis_h
#define _redis_h


#include "vm.h"
#include "store.h"


VM vm;
Store store;

char output_buffer[4096];

void create_redis();
char* query(char* query);


#endif