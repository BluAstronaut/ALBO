   #include <argon2.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    #define HASHLEN 32
    #define SALTLEN 16

    char STRPassword[64];
    int HashPassword(char* PWD);