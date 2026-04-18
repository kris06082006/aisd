#include <stdio.h>
#include <stdlib.h>

#include "check.h"

int check_int(int* n){
    char c;
    int result = scanf("%d%c", n, &c);
    if (result == EOF){
        return 0;
    }
    if (result == 0 || c != '\n'){
        scanf("%*[^\n]");
        return -1;
    }

    return 1;
}
