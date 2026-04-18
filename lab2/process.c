#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "stack.h"
#include "process.h"

#define DELIM " \t"

Err postfix_to_infix(char *str){
    Err status = ERR_OK;
    int size = 0;
    status = input_size(&size);
    if (status != ERR_OK) goto clean_and_exit;

    Stack *stack = NULL;
    status = init_stack(&stack, size);
    if (status != ERR_OK) goto clean_and_exit;

    int len = strlen(str);
    char *word = strtok(str, DELIM);
    Priority_operation current_op;
    Item *item = NULL;
    Item *operand1 = NULL;
    Item *operand2 = NULL;
    char *tmpstr = NULL;
    while (word != NULL){
        if (is_operand(word) == ERR_OK){
            item = malloc(sizeof(Item));
            item->s = strdup(word);
            if (item->s == NULL){
                status = ERR_MEM;
                goto clean_and_exit;
            }
            item->priority = NO;
            status = push(stack, item);
            if (status != ERR_OK){
                status = ERR_OVERFLOW;
                goto clean_and_exit;
            }
            item = NULL;
        }
        else if (is_operation(word) == ERR_OK){
            operand1 = pop(stack, &status);
            if (status != ERR_OK) goto clean_and_exit;
            operand2 = pop(stack, &status);
            if (status != ERR_OK) goto clean_and_exit;

            current_op = which_operation(word[0]);

            tmpstr = calloc(len * 2, sizeof(char));
            if (tmpstr == NULL){
                status = ERR_MEM;
                goto clean_and_exit;
            }
            if ((operand2->priority != NO) && (operand2->priority < current_op)){ // если приоритет второго элемента меньше текущей операции
                strcat(tmpstr, "(");
                strcat(tmpstr, operand2->s);
                strcat(tmpstr, ")");
                strcat(tmpstr, word);
            }
            else {
                strcat(tmpstr, operand2->s);
                strcat(tmpstr, word);
            }
            if ((operand1->priority != NO) && (operand1->priority < current_op)){ // если приоритет первого элемента менье текущей операции
                strcat(tmpstr, "(");
                strcat(tmpstr, operand1->s);
                strcat(tmpstr, ")");
            }
            else {
                strcat(tmpstr, operand1->s);
            }
            item = malloc(sizeof(Item));
            item->s = strdup(tmpstr);
            if (item->s == NULL){
                status = ERR_MEM;
                goto clean_and_exit;
            }
            item->priority = current_op;
            status = push(stack, item);
            if (status != ERR_OK) goto clean_and_exit;
            item = NULL;

            free(operand1->s);
            free(operand2->s);
            free(operand1);
            free(operand2);
            free(tmpstr);
            operand1 = NULL;
            operand2 = NULL;
            tmpstr = NULL;
        }
        else {
            status = ERR_GARBAGE;
            goto clean_and_exit;
        }
        word = strtok(NULL, DELIM);
    }
    int count = count_elem(stack);
    if (count != 1){
        status = ERR_GARBAGE;
        goto clean_and_exit;
    }
    print_stack(stack);
    goto clean_and_exit;

    clean_and_exit:
        if (tmpstr) free(tmpstr);
        if (operand1) {
            free(operand1->s);
            free(operand1);
        }
        if (operand2){
            free(operand2->s);
            free(operand2);
        }
        if (item) {
            free(item->s);
            free(item);
        }
        if (stack) {
            clean_stack(stack);
        }
        return status;

}
