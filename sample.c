#if 1
#include <stdio.h>


// Sample program for testing the lexical analyzer
int mainn(void)
{
    int count = 10;
    float ratio = 3.14;
    char grade = 'A';
    const char *message = "Hello, lexer!";

    if (count >= 10 && ratio != 0.0) {
        printf("%s %c\n", message, grade);
    }
    return 0;
}
#endif