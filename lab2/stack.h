#ifndef STACK_H
#define STACK_H

typedef enum Priority_operation {
    NO = 0,
    PLUS = 1,
    MINUS = 1,
    MULTIPLY = 2,
    DIVIDE = 2,
} Priority_operation;

typedef enum {
    ERR_EOF = 0,
    ERR_MEM = 1,
    ERR_GARBAGE = 2,
    ERR_EMPTY = 3,
    ERR_OVERFLOW = 4,
    ERR_OK = 5,
} Err;

typedef struct Stack Stack;

typedef struct Item{
    char *s;
    Priority_operation priority;
} Item;

Err input_size(int*);
Err push(Stack*, Item*);
Item *pop(Stack*, Err*);
Err init_stack(Stack **stack, int);
void clean_stack(Stack*);
Err print_stack(Stack*);
int count_elem(Stack*);

#endif
