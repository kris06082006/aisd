#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

Err input_int(int *x){
    Err status = scan_int(x);
    char *err = error_message(status);
    printf("%s", err);
    while(status != ERR_OK || *x < 0){
        if (status == ERR_EOF) return status;
        printf("Try again: ");
        status = scan_int(x);
        err = error_message(status);
        printf("%s", err);
    }
    return ERR_OK;
}

Err scan_int(int* n){
    char c;
    int result = scanf("%d%c", n, &c);
    if (result == EOF) return ERR_EOF;
    if (result == 0 || c != '\n'){
        scanf("%*[^\n]");
        return ERR_GARBAGE;
    }
    return ERR_OK;
}

Err scan_unsigned_int(unsigned int* n) {
    unsigned long long tmp;
    char c;
    int result = scanf("%llu%c", &tmp, &c);
    if (result == EOF) return ERR_EOF;
    if (result != 2 || c != '\n') {
        scanf("%*[^\n]");
        return ERR_GARBAGE;
    }    
    if (tmp > UINT_MAX) return ERR_OVERFLOW;
    
    *n = (unsigned int)tmp;
    return ERR_OK;
}

Err input_unsigned_int(unsigned int* x) {
    Err status = scan_unsigned_int(x);
    char *err = error_message(status);
    printf("%s", err);
    
    while(status != ERR_OK) {
        if (status == ERR_EOF) return status;
        printf("Try again: ");
        status = scan_unsigned_int(x);
        char *err = error_message(status);
        printf("%s", err);
    }
    return ERR_OK;
}

char *error_message(Err status){
    static char* array_error[13] = {
        "End of file.\n",
        "Error of memory allocation.\n",
	    "Structure is empty.\n",
	    "Error of reading from file.\n",
	    "You enter garbage.\n",
	    "Elements not found.\n",
	    "Error of opening file.\n",
	    "Error of writing to file.\n",
        "Structure is not initialized.\n",
        "Structure already is initialized.\n",
        "Failed to execute system command.\n",
        "Overflow error.\n",
        "",
    };
    return array_error[status];

}
