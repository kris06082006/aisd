#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "stack.h"

Err check_int(int* n){
    char c;
    int result = scanf("%d%c", n, &c);
    if (result == EOF) return ERR_EOF;
    if (result == 0 || c != '\n'){
        scanf("%*[^\n]");
        return ERR_GARBAGE;
    }
    return ERR_OK;
}

const char *error_message(Err status){
    static const char* array_error[6] = {
        "End of file.\n",
        "Error of memory allocation.\n",
        "You enter garbage.\n",
        "Stack in empty.\n",
        "Stack is overflow.\n",
        "",
    };
    return array_error[status]; 
}

Err is_operand(const char *s){
    if ((strlen(s) == 1) && ((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z'))) return ERR_OK;
    int number = atoi(s);
    if ((s[0] != '0') && (number == 0)) return ERR_GARBAGE;
    else return ERR_OK;
}

Err is_operation(char *s){
    if (strlen(s) == 1 && (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/')) return ERR_OK;
    else return ERR_GARBAGE;
}

Priority_operation which_operation(char operation){
    if (operation == '*') return MULTIPLY;
    if (operation == '/') return DIVIDE;
    if (operation == '+') return PLUS;
    if (operation == '-') return MINUS;
}


