#include "redis.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_redis() {
    main_redis.count = 0;
    main_redis.capacity = 0;
    main_redis.stores = NULL;
    main_redis.vms = NULL;
    main_redis.buffers = NULL;
}

int create_instance() {
    if (main_redis.count + 1 > main_redis.capacity) {
        int new_capacity = main_redis.capacity < 8 ? 8 : main_redis.capacity * 2;
        main_redis.vms = (VM**)realloc(main_redis.vms, sizeof(VM*) * new_capacity);
        main_redis.stores = (Store**)realloc(main_redis.stores, sizeof(Store*) * new_capacity);
        main_redis.buffers = (char**)realloc(main_redis.buffers, sizeof(char*) * new_capacity);
        main_redis.capacity = new_capacity;
    }

    Store* store = (Store*)malloc(sizeof(Store));
    init_store(store);

    VM* vm = (VM*)malloc(sizeof(VM));
    init_vm(vm);

    vm->store = store;

    char* buf = (char*)calloc(sizeof(char), 4096);

    main_redis.vms[main_redis.count] = vm;
    main_redis.stores[main_redis.count] = store;
    main_redis.buffers[main_redis.count] = buf;
    main_redis.count++;

    return main_redis.count - 1;
}

void free_instance(int id) {
    Store* store = main_redis.stores[id];
    VM* vm = main_redis.vms[id];
    char* buf = main_redis.buffers[id];

    free_vm(vm);
    free_store(store);
    free(buf);
    free(vm);
    free(store);
}


char* query(int id, char* q) {
    if (id > main_redis.count) {
        printf("Wrong ID\n");
        // TODO handle this case more gracefully
        return "";
    }

    VM* vm = main_redis.vms[id];
    char* output_buffer = main_redis.buffers[id];

    // Clear out the output buffer
    memset(output_buffer, '\0', sizeof(output_buffer));

    InterpretResult res = interpret(vm, q, output_buffer);
    return output_buffer;
}