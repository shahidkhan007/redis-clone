#include "redis.h"

#include <string.h>

void create_redis() {
    init_vm(&vm);
    init_store(&store);
}

char* query(char* q) {
    InterpretResult res = interpret(&vm, &store, q, output_buffer);
    return output_buffer;
}