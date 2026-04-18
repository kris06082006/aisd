#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

#include "matr.h"


int main(){

    int status = 0;

    char *rfile = readline("Enter file for reading: ");
    if (rfile == NULL)
        return 1;
    char *wfile = readline("Enter file for writing: ");
    if (wfile == NULL){
        free(rfile);
        return 1;
    }

    Matrix matr = {0, NULL};

    status = read_matr(&matr, rfile);
    print_err(status);
    if (status != ERR_OK){
        free(rfile);
        free(wfile);
        return 1;
    }    
    status = write_matr(&matr, wfile);
    print_err(status);
    if (status != ERR_OK){
        erase(&matr);
        free(rfile);
        free(wfile);
        return 1;
    }    
    erase(&matr);
    free(rfile);
    free(wfile);
    return 0;
}

 
