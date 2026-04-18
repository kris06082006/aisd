#include "functions.h"
#include "dialog.h"
//#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int main(){
    Err status = dialog();
    
    return status == ERR_OK ? 0 : 1; 
}


