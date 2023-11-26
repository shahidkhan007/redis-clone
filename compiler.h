#ifndef _compiler_h
#define _compiler_h

#include "chunk.h"
#include "scanner.h"

typedef struct {
    Token current;
    Token previous;
    bool had_error;
    Chunk* chunk;
} Compiler;

void init_compiler(Compiler* compiler, Chunk* chunk);
void free_compiler(Compiler* compiler);

bool compile(const char* source, Chunk* chunk);

#endif