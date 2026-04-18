#include <stdio.h>
#include "functions.h"
#include "check.h"

int main(){
    Matrix matr = {0, NULL};
    Matrix new_matr = {0, NULL};

    Err status = input(&matr);
    print_err(status);
    if (status != ERR_OK) return 1;

    status = modified_matr(matr, &new_matr);
    print_err(status);
    if (status != ERR_OK) return 1;

    output("Source matrix", matr);
    output("Resulting matrix", new_matr);
    erase(&matr);
    erase(&new_matr);
    return 0;

} 




