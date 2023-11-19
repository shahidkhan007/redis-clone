#include "chunk.h"

#include <stdlib.h>

void init_chunk(Chunk* chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;

    init_value_array(&chunk->constants);
}

void free_chunk(Chunk* chunk) {
    // Free the memory for the dynamic array and reset the chunk state
    free(chunk->code);
    free_value_array(&chunk->constants);
    init_chunk(chunk);

}

void write_chunk(Chunk* chunk, uint8_t byte) {
    // Dynamic array stuf.. If we run out of space in the array, we allocate more space.
    // The size will grow like: 8 -> 16 -> 32 ...
    if (chunk->count + 1 > chunk->capacity) {
        int new_capacity = chunk->capacity < 8 ? 8 : chunk->capacity * 2;
        chunk->code = (uint8_t*)realloc(chunk->code, sizeof(uint8_t) * new_capacity);
        chunk->capacity = new_capacity;
    }

    // Then simply insert and increment the counter.
    chunk->code[chunk->count] = byte;
    chunk->count++;
}