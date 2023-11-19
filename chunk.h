#ifndef _chunk_h
#define _chunk_h


#include "common.h"
#include "value.h"


typedef enum {
    OP_GET,
    OP_SET,
    OP_RETURN
} OpCode;


typedef struct {
    int count;
    int capacity;
    uint8_t* code;

    ValueArray constants;
} Chunk;

void init_chunk(Chunk* chunk);
void free_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte);




#endif