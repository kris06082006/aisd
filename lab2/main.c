#include "process.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>


#define PROMPT "> "

int main(){
    Err status;
    printf("Enter mathematical expression in postfix from: ");
    char *input = readline(PROMPT);
    if (input == NULL) return 1;
        
    while (input != NULL){
        status = postfix_to_infix(input);
        printf("%s", error_message(status));
        if (status != ERR_OK && status != ERR_GARBAGE){
            return 1;
        }
        free(input);

        printf("Enter mathematical expession in postfix form : ");
        input = readline(PROMPT);
        if (input == NULL) return 1;
    }
    return 0;    
}
