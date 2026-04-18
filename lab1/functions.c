#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "check.h"

void print_err(Err status){

    if (status == ERR_EOF)
        printf("%s\n", "End of file occured");
    if (status == ERR_MEM)
        printf("%s\n", "Memory allocation error");
}

Err input(Matrix *rm){
    const char *pr = "";
    int m = 0;
    int *p;
    Line *status;
    int res;
    do {
        printf("%s\n", pr);
        printf("Enter number of lines: --> ");
        pr = "You are wrong; repeat, please!";
        res = check_int(&m);
        if(res == 0)    
            return ERR_EOF;
        } while ((m < 1) || (res < 0));
        rm->lines = m;  
        status = (Line*) calloc(m, sizeof(Line));
        if (status == NULL)
            return ERR_MEM;
        rm->matr = status;
        for (int i = 0; i < rm->lines; i++){
            pr = "";
            do{
                printf("%s\n", pr);
                printf("Enter number of items in line %d: --> ", i+1);
                pr = "You are wrong; repeat, pleas!";
                res = check_int(&m);
                if (res == 0){
                    rm->lines = i;     
                    erase(rm);
                    return ERR_EOF;
                }
            } while ((m < 1) || (res < 0));
            rm->matr[i].n = m;              
            p = (int*) malloc(m*sizeof(int));
            if (p == NULL){
                erase(rm);
                return ERR_MEM;
            }
            rm->matr[i].a = p;   
            printf("Enter items for matrix line #%d:\n", i+1);
            for (int j = 0; j < m; ++j, ++p){
                pr = "";
                do {
                    printf("%s\n", pr);
                    printf("Enter item №%d\n", j+1);
                    pr = "You are wrong; repeat, pleas!";
                    res = check_int(p);
                    if (res == 0){ 
                        rm->lines = i + 1;
                        erase(rm); 
                        return ERR_EOF;     
                    }
                } while (res < 0);
            }
        }
        return ERR_OK;

}


void output(const char *msg, Matrix m){
    printf("%s\n", msg);
    for (int i = 0; i < m.lines; i++){
        for (int j = 0; j < m.matr[i].n; j++){
            printf("%d ", m.matr[i].a[j]);
        }
        printf("\n");
    }
}

void erase(Matrix *rm){
    for (int i = 0; i < rm->lines; i++){
        free(rm->matr[i].a);
    }
    free(rm->matr);
}
 


Err modified_matr(Matrix m, Matrix *new_m){
    new_m->lines = m.lines;
    new_m->matr = (Line*) calloc(m.lines, sizeof(Line));
    if (new_m->matr == NULL) return ERR_MEM;
    for (int i = 0; i < m.lines; i++){
        new_m->matr[i].n = m.matr[i].n;
        new_m->matr[i].a = (int*) malloc(sizeof(int)*(m.matr[i].n));
        if (new_m->matr[i].a == NULL) {
            erase(new_m);
            return ERR_MEM;
        }
        int len = m.matr[i].n;
        for (int j = len-1; j >= 0; j--){
            new_m->matr[i].a[j] = m.matr[i].a[len-j-1];
        } 

    }
    return ERR_OK;
}                 
