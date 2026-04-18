#ifndef MATR_H
#define MATR_H

#include <stdio.h>

typedef enum Err{
    ERR_OK = 0,
    ERR_EOF = 1,
    ERR_MEM = 2,
    ERR_OPEN = 3,
    ERR_READ = 4,
    ERR_WRITE = 5,
    ERR_SEEK = 6,
} Err;

typedef struct Line{
    int n; 
    int a;
} Line; 

typedef struct Matrix {
    int lines;         
    Line *matr; 
    FILE *f;
} Matrix;

void print_err(Err status);
Err read_matr(Matrix *ptr_m, char *file);
Err erase(Matrix *matr);
Err write_matr(Matrix *ptr_m, char *file);


#endif
