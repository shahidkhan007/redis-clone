#include "scanner.h"

#include <string.h>
#include <stdio.h>


void init_scanner(Scanner* scanner, const char* source) {
    scanner->start = source;
    scanner->current = source;
}

void free_scanner(Scanner* scanner) {
    scanner->start = NULL;
    scanner->current = NULL;
}

static bool is_at_end(Scanner* scanner) {
    return *scanner->current == '\0';
}

static Token make_token(Scanner* scanner, TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner->start;
    token.length = (int)(scanner->current - scanner->start);

    return token;
}

static Token error_token(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    return token;
}

static char advance(Scanner* scanner) {
    scanner->current++;
    return scanner->current[-1];
}

static bool match(Scanner* scanner, char expected) {
    if (is_at_end(scanner)) return false;
    if (*scanner->current != expected) return false;

    scanner->current++;
    return true;
}

static char peek(Scanner* scanner) {
    return *scanner->current;
}

static char peek_next(Scanner* scanner) {
    return *(scanner->current + 1);
}

static void skip_whitespace(Scanner* scanner) {
    for (;;) {
        char c = peek(scanner);
        
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                advance(scanner);
                break;
            default:
                return;
        }
    }
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static bool check_keyword(Scanner* scanner, int start, int length, const char* rest) {
    if (scanner->current - scanner->start == start + length) {
        if (memcmp(scanner->start + start, rest, length) == 0) {
            return true;
        }
    }

    return false;
}

static Token keyword(Scanner* scanner, int start, int length, const char* rest, TokenType type) {
    while (is_alpha(peek(scanner))) {
        advance(scanner);
    }

    bool is_match = check_keyword(scanner, start, length, rest);
    if (!is_match) {
        return error_token("Unrecognized keyword.");
    }

    return make_token(scanner, type);
}

static Token number(Scanner* scanner) {
    bool is_float = false;

    while (!is_at_end(scanner)) {
        char c = peek(scanner);
        if (is_digit(c) || c == '.') {
            if (c == '.') {
                is_float = true;
            }
            advance(scanner);
        } else {
            break;
        }
    }

    return make_token(scanner, is_float ? TOKEN_FLOAT : TOKEN_INT);
}

static Token string(Scanner* scanner) {
    while (!is_at_end(scanner)) {
        if (peek(scanner) == '\\' && peek_next(scanner) == '"') {
            advance(scanner);
        } else if (peek(scanner) == '"') {
            break;
        }
        advance(scanner);
    }

    if (is_at_end(scanner)) {
        return error_token("Unterminated string literal.");
    }

    advance(scanner);
    return make_token(scanner, TOKEN_STR);

}


Token scan_token(Scanner* scanner) {
    skip_whitespace(scanner);
    scanner->start = scanner->current;

    char c = advance(scanner);
    
    if (is_at_end(scanner)) {
        return make_token(scanner, TOKEN_EOF);
    }
    
    switch (c) {
        case '"':
            return string(scanner);
        case 'G':
            return keyword(scanner, 1, 2, "ET", TOKEN_GET);
        case 'S':
            return keyword(scanner, 1, 2, "ET", TOKEN_SET);
        case 't':
            return keyword(scanner, 1, 3, "rue", TOKEN_TRUE);
        case 'f':
            return keyword(scanner, 1, 4, "alse", TOKEN_FALSE);
        default: {
            char c = peek(scanner);
            if (is_digit(c)) {
                return number(scanner);
            }
        }

    }

    return error_token("Unexpected character.");
}