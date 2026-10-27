#include "lexer.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *source;
    Token token;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source-file.c>\n", argv[0]);
        return 1;
    }

    source = fopen(argv[1], "r");
    if (source == NULL) {
        perror("Unable to open source file");
        return 1;
    }

    printf("%-6s | %-22s | %s\n", "LINE", "TOKEN TYPE", "LEXEME");
    printf("-------+------------------------+------------------------------\n");

    while ((token = get_next_token(source)).type != TOKEN_EOF) {
        printf("%-6d | %-22s | %s\n",
               token.line, token_type_name(token.type), token.lexeme);
    }

    fclose(source);
    return 0;
}
