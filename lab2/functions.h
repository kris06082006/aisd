#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "stack.h"

Err check_int(int*);
const char *error_message(Err status);
Err is_operand(const char*);
Err is_operation(char*);
Priority_operation which_operation(char);

#endif
