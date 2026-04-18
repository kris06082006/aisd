#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "dialog.h"

int main(){
    Err status = dialog();
    if (status == ERR_OK) return 0;
    else return 1;
}
