#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "functions.h"

#define DELIM " \t"
typedef struct Node {
    Item *item;
    struct Node* next;
} Node;

struct Stack {
    Node* top;
    int count;
};

Err input_size(int* size){
    return ERR_OK;
}

Err push(Stack *stack, Item *i){
    Node *node = malloc(sizeof(Node));
    if (!node) return ERR_MEM;
    node->item = i;
    node->next = stack->top;                                   
    stack->top = node;
    stack->count++;
    return ERR_OK;    
}

Item *pop(Stack *stack, Err *status){
    if (stack->top == NULL){
        (*status) = ERR_EMPTY;
        return NULL;
    }
    Item *i = stack->top->item;
    Node *tmp = stack->top;
    stack->top = stack->top->next;
    free(tmp);
    stack->count--;

    *status = ERR_OK;
    return i;
}

Err init_stack(Stack **stack,int size){
    (*stack) = malloc(sizeof(Stack));
    if ((*stack) == NULL) return ERR_MEM;
    (*stack)->top = NULL;
    return ERR_OK;
}

void clean_stack(Stack *stack){
    while (stack->top != NULL){
        Node *tmp = stack->top;
        stack->top = stack->top->next;
        free(tmp->item->s);
        free(tmp->item);
        free(tmp);

    }
    stack->top = NULL;
    free(stack);
} 

Err print_stack(Stack *stack){
    if (stack->top == NULL || stack == NULL) return ERR_EMPTY;
    Node *current = stack->top;
    while (current != NULL){
        printf("%s", current->item->s);
        current = current->next;
    }
    printf("\n");
    return ERR_OK;
}

int count_elem(Stack *stack){
    return stack->count; 
}
