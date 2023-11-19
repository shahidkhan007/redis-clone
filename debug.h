#ifndef _debug_h
#define _debug_h

#include "chunk.h"

int disassemble_instruction(Chunk* chunk, int offset);
void disassemble_chunk(Chunk* chunk, const char* name);

#endif