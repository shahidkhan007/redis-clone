#include "debug.h"

#include <stdio.h>

// chunk is the current Chunk we're working with,
// and the offset is the location at which we want to disassemble and instruction.
// It returns an int, that's the resulting offset after disassembly.
// Instruction have operands. Ex: ADD A B. This has two operands.
// Disassembling this will move the "cursor" three bytes forward.
int disassemble_instruction(Chunk* chunk, int offset) {
    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_RETURN:
            printf("%04d OP_RETURN\n", offset);
            return offset + 1;
        
        case OP_GET: {
            uint8_t key_idx = chunk->code[offset + 1];
            printf("%04d OP_GET %d\n", offset, key_idx);
            return offset + 2;
        }

        case OP_SET: {
            uint8_t key_idx = chunk->code[offset + 1];
            uint8_t value_idx = chunk->code[offset + 2];

            printf("%04d OP_SET %d %d\n", offset, key_idx, value_idx);
            return offset + 3;
        }
            
        default:
            printf("%04d %d\n", offset, instruction);
            return offset + 1;
    }
}

void disassemble_chunk(Chunk* chunk, const char* name) {

    printf("=== %s ===\n", name);

    // I don't specify the increment because the above function does that for us.
    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}