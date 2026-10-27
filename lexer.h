#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define MAX_LEXEME 256

typedef enum {
    TOKEN_EOF = 0,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_CHARACTER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_PREPROCESSOR,
    TOKEN_COMMENT,
    TOKEN_UNKNOWN,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME];
    int line;
} Token;

Token get_next_token(FILE *source);
const char *token_type_name(TokenType type);

#endif
