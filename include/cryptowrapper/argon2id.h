<<<<<<< HEAD
#include "argon2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SALTLEN 16
#define HASHLEN 32

char* Str_Password[64];
int HashPassword(char* PWD);
=======
   #include <argon2.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    #define HASHLEN 32
    #define SALTLEN 16

    char STRPassword[64];
    int HashPassword(char* PWD);
>>>>>>> 2d1c6953329196393d80f235dc423e850a8d7ed8
