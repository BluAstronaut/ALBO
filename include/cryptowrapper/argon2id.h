#include "argon2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SALTLEN 16
#define HASHLEN 32

char* Str_Password[64];
int HashPassword(char* PWD);