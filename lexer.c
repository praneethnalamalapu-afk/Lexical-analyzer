#include "lexer.h"
#include <ctype.h>
#include <string.h>

static int current_line = 1;

static int read_char(FILE *source)
{
    int c = fgetc(source);
    if (c == '\n') {
        current_line++;
    }
    return c;
}

static void unread_char(int c, FILE *source)
{
    if (c == EOF) return;
    if (c == '\n') current_line--;
    ungetc(c, source);
}

static void append_char(Token *token, int c)
{
    size_t len = strlen(token->lexeme);
    if (len + 1 < sizeof(token->lexeme)) {
        token->lexeme[len] = (char)c;
        token->lexeme[len + 1] = '\0';
    }
}

static Token make_token(TokenType type, int line)
{
    Token token;
    token.type = type;
    token.lexeme[0] = '\0';
    token.line = line;
    return token;
}

static int is_keyword(const char *word)
{
    static const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while", "_Bool", "_Complex",
        "_Imaginary", "inline", "restrict"
    };
    size_t i;
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}

static int is_delimiter(int c)
{
    return c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ';' || c == ',' ||
           c == '.' || c == ':';
}

static int is_operator_char(int c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '!' || c == '<' || c == '>' || c == '&' ||
           c == '|' || c == '^' || c == '~' || c == '?' ;
}

static Token scan_identifier(FILE *source, int first, int line)
{
    Token token = make_token(TOKEN_IDENTIFIER, line);
    int c;
    append_char(&token, first);
    while ((c = read_char(source)) != EOF && (isalnum((unsigned char)c) || c == '_')) {
        append_char(&token, c);
    }
    unread_char(c, source);
    if (is_keyword(token.lexeme)) token.type = TOKEN_KEYWORD;
    return token;
}

static Token scan_number(FILE *source, int first, int line)
{
    Token token = make_token(TOKEN_INTEGER, line);
    int c, has_dot = 0;
    append_char(&token, first);

    while ((c = read_char(source)) != EOF) {
        if (isdigit((unsigned char)c)) {
            append_char(&token, c);
        } else if (c == '.' && !has_dot) {
            has_dot = 1;
            append_char(&token, c);
        } else {
            unread_char(c, source);
            break;
        }
    }
    token.type = has_dot ? TOKEN_FLOAT : TOKEN_INTEGER;
    return token;
}

static Token scan_quoted(FILE *source, int quote, int line)
{
    Token token = make_token(quote == '"' ? TOKEN_STRING : TOKEN_CHARACTER, line);
    int c, closed = 0;
    append_char(&token, quote);

    while ((c = read_char(source)) != EOF) {
        append_char(&token, c);
        if (c == '\\') {
            int escaped = read_char(source);
            if (escaped == EOF) break;
            append_char(&token, escaped);
        } else if (c == quote) {
            closed = 1;
            break;
        } else if (c == '\n') {
            break;
        }
    }
    if (!closed) token.type = TOKEN_ERROR;
    return token;
}

static Token scan_preprocessor(FILE *source, int line)
{
    Token token = make_token(TOKEN_PREPROCESSOR, line);
    int c;
    append_char(&token, '#');
    while ((c = read_char(source)) != EOF && c != '\n') append_char(&token, c);
    unread_char(c, source);
    return token;
}

static Token scan_slash(FILE *source, int line)
{
    Token token = make_token(TOKEN_OPERATOR, line);
    int c = read_char(source);
    append_char(&token, '/');

    if (c == '/') {
        token.type = TOKEN_COMMENT;
        append_char(&token, c);
        while ((c = read_char(source)) != EOF && c != '\n') append_char(&token, c);
        unread_char(c, source);
        return token;
    }
    if (c == '*') {
        int previous = 0, closed = 0;
        token.type = TOKEN_COMMENT;
        append_char(&token, c);
        while ((c = read_char(source)) != EOF) {
            append_char(&token, c);
            if (previous == '*' && c == '/') {
                closed = 1;
                break;
            }
            previous = c;
        }
        if (!closed) token.type = TOKEN_ERROR;
        return token;
    }
    if (c == '=') {
        append_char(&token, c);
    } else {
        unread_char(c, source);
    }
    return token;
}

static Token scan_operator(FILE *source, int first, int line)
{
    Token token = make_token(TOKEN_OPERATOR, line);
    int c;
    append_char(&token, first);
    c = read_char(source);

    if ((first == '+' && (c == '+' || c == '=')) ||
        (first == '-' && (c == '-' || c == '=' || c == '>')) ||
        ((first == '*' || first == '%' || first == '=' || first == '!' ||
          first == '<' || first == '>' || first == '&' || first == '|' ||
          first == '^') && c == '=') ||
        ((first == '<' || first == '>') && c == first) ||
        ((first == '&' || first == '|') && c == first)) {
        append_char(&token, c);
        if ((first == '<' || first == '>') && c == first) {
            int third = read_char(source);
            if (third == '=') append_char(&token, third);
            else unread_char(third, source);
        }
    } else {
        unread_char(c, source);
    }
    return token;
}

Token get_next_token(FILE *source)
{
    int c;
    Token token;

    while ((c = read_char(source)) != EOF && isspace((unsigned char)c)) {
        /* Skip whitespace while maintaining line count. */
    }
    if (c == EOF) return make_token(TOKEN_EOF, current_line);

    token = make_token(TOKEN_UNKNOWN, current_line);

    if (isalpha((unsigned char)c) || c == '_') return scan_identifier(source, c, token.line);
    if (isdigit((unsigned char)c)) return scan_number(source, c, token.line);
    if (c == '"' || c == '\'') return scan_quoted(source, c, token.line);
    if (c == '#') return scan_preprocessor(source, token.line);
    if (c == '/') return scan_slash(source, token.line);
    if (is_operator_char(c)) return scan_operator(source, c, token.line);

    if (is_delimiter(c)) {
        token.type = TOKEN_DELIMITER;
        append_char(&token, c);
        return token;
    }

    token.type = TOKEN_UNKNOWN;
    append_char(&token, c);
    return token;
}

const char *token_type_name(TokenType type)
{
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER_CONSTANT";
        case TOKEN_FLOAT: return "FLOAT_CONSTANT";
        case TOKEN_STRING: return "STRING_LITERAL";
        case TOKEN_CHARACTER: return "CHARACTER_LITERAL";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_DELIMITER: return "DELIMITER";
        case TOKEN_PREPROCESSOR: return "PREPROCESSOR";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_ERROR: return "LEXICAL_ERROR";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
