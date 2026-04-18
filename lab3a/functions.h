#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <limits.h>

typedef enum{
    ERR_EOF = 0,
    ERR_MEM = 1,
    ERR_EMPTY = 2,
    ERR_SCANF = 3,
    ERR_GARBAGE = 4,
    ERR_FIND = 5,
    ERR_OPEN = 6,
    ERR_OVERFLOW = 7,
    ERR_NOT_INIT = 8,
    ERR_INIT = 9,
    ERR_OK = 10,
} Err;
Err input_int(int *);
Err scan_int(int *);
Err input_unsigned_int(unsigned int *);
Err scan_unsigned_int(unsigned int *);
char *error_message(Err);

#endif
