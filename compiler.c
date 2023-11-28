#include "compiler.h"
#include "value.h"
#include "debug.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void error_at(Compiler* compiler, Token token, const char* message) {
    fprintf(stderr, "Error");

    if (token.type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token.type == TOKEN_ERROR) {
        
    } else {
        fprintf(stderr, " at %s", token.start);
    }

    fprintf(stderr, ": %s\n", message);
    compiler->had_error = true;
}

static void error(Compiler* compiler, const char* message) {
    error_at(compiler, compiler->previous, message);
}

static void error_at_current(Compiler* compiler, const char* message) {
    error_at(compiler, compiler->current, message);
}

static Token advance(Compiler* compiler, Scanner* scanner) {
    compiler->previous = compiler->current;

    for (;;) {
        compiler->current = scan_token(scanner);
        if (compiler->current.type != TOKEN_ERROR) {
            break;
        }

        compiler->had_error = true;

    }

    return compiler->current;
}

static void consume(Compiler* compiler, Scanner* scanner, TokenType type, const char* message) {
    Token t = advance(compiler, scanner);
    if (t.type == type) {
        return;
    }

    error_at_current(compiler, message);
}

static void emit_byte(Chunk* chunk, uint8_t byte) {
    write_chunk(chunk, byte);
}

static uint8_t write_constant(Token token, Chunk* chunk) {
    Value v;

    switch (token.type) {
        case TOKEN_INT: {
            char lexeme[token.length];
            strncpy(lexeme, token.start, token.length);
            v.type = VT_INT;
            v.as.integer = strtoull(lexeme, NULL, 10);
            break;
        }
        case TOKEN_FLOAT: {
            char lexeme[token.length];
            strncpy(lexeme, token.start, token.length);
            v.type = VT_FLOAT;
            v.as.flt = atof(lexeme);
            break;
        }
        case TOKEN_STR: {
            char* lexeme = (char*)calloc(sizeof(char), 512);
            strncpy(lexeme, token.start + 1, token.length - 2);

            v.type = VT_STRING;
            v.as.str = lexeme;
            break;
        }
        case TOKEN_TRUE:
        case TOKEN_FALSE: {
            char lexeme[token.length];
            strncpy(lexeme, token.start, token.length);
            v.type = VT_BOOL;
            v.as.boolean = lexeme[0] == 't' ? true : false;
            break;
        }
    }

    uint8_t idx = write_value_array(&chunk->constants, v);

    return idx;
}

static void get(Compiler* compiler, Scanner* scanner) {
    Token token = advance(compiler, scanner);
    
    switch (token.type) {
        case TOKEN_STR:
        case TOKEN_INT:
            goto emit;
        case TOKEN_EOF:
            error_at_current(compiler, "Expected a key after the GET.");
            return;
        default:
            error_at_current(compiler, "A key can only be of type 'String' or 'Int'.");
            return;
    }

    emit:
        emit_byte(compiler->chunk, OP_GET);
        emit_byte(compiler->chunk, write_constant(token, compiler->chunk));

}

static void set(Compiler* compiler, Scanner* scanner) {
    Token key_token = advance(compiler, scanner);

    switch (key_token.type) {
        case TOKEN_INT:
        case TOKEN_STR:
            break;
        case TOKEN_EOF:
            error_at_current(compiler, "Expected a key after the SET.");
            return;
        default:
            error_at_current(compiler, "A key can only be of type 'String' or 'Int'.");
            return;
    }

    Token value_token = advance(compiler, scanner);

    switch (value_token.type) {
        case TOKEN_STR:
        case TOKEN_INT:
        case TOKEN_FLOAT:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            goto emit;
        case TOKEN_EOF:
            error_at_current(compiler, "Expected a value after the key.");
            return;
        default:
            error_at_current(compiler, "Expected a string or number for value.");
            return;
    }

    emit: {
        // advance(compiler, scanner);

        uint8_t key_idx = write_constant(key_token, compiler->chunk);
        uint8_t value_idx = write_constant(value_token, compiler->chunk);

        emit_byte(compiler->chunk, OP_SET);
        emit_byte(compiler->chunk, key_idx);
        emit_byte(compiler->chunk, value_idx);
    }
}

static void statement(Compiler* compiler, Scanner* scanner) {
    Token token = advance(compiler, scanner);

    switch (token.type) {
        case TOKEN_GET:
            return get(compiler, scanner);
        case TOKEN_SET:
            return set(compiler, scanner);
        default:
            error_at_current(compiler, "Expected a valid statement.");
    }
}

bool compile(const char* source, Chunk* chunk) {
    Scanner scanner;
    init_scanner(&scanner, source);

    Compiler compiler;
    compiler.chunk = chunk;
    compiler.had_error = false;
    
    statement(&compiler, &scanner);

    if (compiler.had_error) {
        return false;
    }

    consume(&compiler, &scanner, TOKEN_EOF, "Expected end of statement.");
    emit_byte(chunk, OP_RETURN);
    return true;
}