#ifndef _scanner_h
#define _scanner_h

#include "common.h"

typedef enum {
    TOKEN_EOF,
    TOKEN_GET,
    TOKEN_SET,
    TOKEN_QUOTE,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_STR,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
} Token;

typedef struct {
    const char* start;
    const char* current;
} Scanner;

void init_scanner(Scanner* scanner, const char* source);
void free_scanner(Scanner* scanner);
Token scan_token(Scanner* scanner);

#endif