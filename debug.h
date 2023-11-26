#ifndef _debug_h
#define _debug_h

#include "chunk.h"
#include "scanner.h"

int disassemble_instruction(Chunk* chunk, int offset);
void disassemble_chunk(Chunk* chunk, const char* name);

void print_token(Token token);
#endif