#ifndef _vm_h
#define _vm_h

#include "common.h"
#include "chunk.h"
#include "store.h"

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMP_ERR,
    INTERPRET_RUNTIME_ERR
} InterpretResult;

typedef enum {
    OPERATION_GET,
    OPERATION_SET
} Operation;

typedef struct {
    uint8_t* ip;
    Chunk* chunk;
    Store* store;
} VM;

void init_vm(VM* vm);
void free_vm(VM* vm);
InterpretResult interpret(VM* vm, const char* source, char* output_buffer);

// void start();

#endif