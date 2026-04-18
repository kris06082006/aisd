#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "functions.h"

#define DELIM " \t"

struct Stack {
    int top;
    int capacity;
    Item *arr;
};

Err input_size(int *size){
    printf("Enter size of stack: ");
    Err status = check_int(size);
    printf("%s", error_message(status));
    while(status != ERR_OK || *size <= 0){
        if (status == ERR_EOF) return status;
        printf("Try again: ");
        status = check_int(size);
        printf("%s", error_message(status));
    }
    return ERR_OK;
}

Err push(Stack *stack, Item *item){
    if (stack->top == stack->capacity) return ERR_OVERFLOW;                              
    stack->arr[stack->top].s = item->s;    
    stack->arr[stack->top].priority = item->priority; 
    stack->top++;
    free(item);
    return ERR_OK;    
}

Item *pop(Stack *stack, Err *status){
    if (stack->top == 0){
        *status = ERR_EMPTY;
        return NULL;
    }
    Item *item = malloc(sizeof(Item));
    if (item == NULL){
        *status = ERR_MEM;
        return NULL;
    }
    stack->top--;
    item->s = stack->arr[stack->top].s;
    item->priority = stack->arr[stack->top].priority; 
    *status = ERR_OK;
    return item;
}

Err init_stack(Stack **stack,int size){
    *stack = malloc(sizeof(Stack)); 
    if (*stack == NULL) return ERR_MEM;
    (*stack)->arr = malloc(size * sizeof(Item));
    if ((*stack)->arr == NULL) return ERR_MEM;   
    (*stack)->capacity = size;
    (*stack)->top = 0;
    return ERR_OK;
}

void clean_stack(Stack *stack){
    for (int i = 0; i < stack->top; i++){
        free(stack->arr[i].s);
    }
    free(stack->arr);
    free(stack);
}

Err print_stack(Stack *stack){
    if (stack->top == 0 || stack == NULL) return ERR_EMPTY;
    for (int i = 0; i < stack->top; i++){
        printf("%s", stack->arr[i].s);
    }
    printf("\n");
    return ERR_OK;
}

int count_elem(Stack *stack){
    return stack->top; 
}
