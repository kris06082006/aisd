#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum{
    ERR_EOF = 0,
    ERR_MEM = 1,
    ERR_EMPTY = 2,
    ERR_READ = 3,
    ERR_GARBAGE = 4,
    ERR_FIND = 5,
    ERR_OPEN = 6,
    ERR_WRITE = 7,
    ERR_NOT_INIT = 8,
    ERR_INIT = 9,
    ERR_SYS = 10,
    ERR_OK = 11
} Err;

Err input_int(int *);
Err scan_int(int *);
char *error_message(Err);

#endif
