#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum Err {
    ERR_OK = 0,
    ERR_EOF = 1,
    ERR_MEM = 2,
} Err;

typedef struct Line{
    int n; 
    int *a;
} Line; 

typedef struct Matrix {
    int lines;         
    Line *matr; 
} Matrix;

void print_err(Err status);
Err input(Matrix *rm);
void output(const char *msg, Matrix a);
void erase(Matrix *matr);
Err modified_matr(Matrix matr, Matrix *new_matr);


#endif
