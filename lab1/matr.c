#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

#include "matr.h"

void print_err(Err status){

    if (status == ERR_EOF)
        printf("%s\n", "End of file occured");
    if (status == ERR_MEM)
        printf("%s\n", "Memory allocation error");
    if (status == ERR_OPEN)
        printf("%s\n", "File opening error");
    if (status == ERR_SEEK)
        printf("%s\n", "Error of seek in file");
    if (status == ERR_READ)
        printf ("%s\n", "Error of read from file");
    if (status == ERR_WRITE)
        printf ("%s\n", "Error of write in file");
}

Err erase(Matrix *ptr_m){
    
    fclose(ptr_m->f);
    free(ptr_m->matr);
    return ERR_OK;
}

Err read_matr(Matrix *ptr_m, char *rfile){
    int read_count = 0;
    FILE *file = fopen(rfile, "rb");
    if (file == NULL)
        return ERR_OPEN;   

    ptr_m->f = file;
    read_count = fread(&(ptr_m->lines), sizeof(int), 1, file); 
    if (read_count != 1){
        fclose(file);
        if (feof(file)){
            return ERR_EOF;
        }
        else {
            return ERR_READ;
        }
    }  
    ptr_m->matr = (Line*) calloc(ptr_m->lines, sizeof(Line));
    if (ptr_m->matr == NULL){
        fclose(file);
        return ERR_MEM; 
    }
    for (int i = 0; i < ptr_m->lines; i++){
        read_count = fread(&(ptr_m->matr[i].n), sizeof(int), 1, file); 
        if (read_count != 1){ 
            fclose(file);
            free(ptr_m->matr);
            if (feof(file)){
                return ERR_EOF;
            }
            else {
                return ERR_READ;
            }
        }
        read_count = fread(&(ptr_m->matr[i].a), sizeof(int), 1, file); 
        if (read_count != 1){
            free(ptr_m->matr);
            if (feof(file)){
                return ERR_EOF;
            }
            else {
                return ERR_READ;
            }
        }
    } 
    return ERR_OK; 

}

Err write_matr(Matrix *ptr_m, char *wfile){
    int el = 0;
    int read_count = 0;
    int write_count = 0;
    int err_seek = 0;
    
    FILE *file = fopen(wfile, "wb");
    if (file == NULL)
        return ERR_OPEN; 

    write_count = fwrite(&(ptr_m->lines), sizeof(int), 1, file);
    if (write_count != 1){
        fclose(file);
        if (feof(file)){
            return ERR_EOF;
        }
        else {
            return ERR_WRITE;
        }
    }
    for (int i = 0; i < ptr_m->lines; i++){
        write_count = fwrite(&(ptr_m->matr[i].n), sizeof(int), 1, file);
        if (write_count != 1){
            fclose(file);
            if (feof(file)){
                return ERR_EOF;
            }
            else {
                return ERR_WRITE;
            }
        }
        write_count = fwrite(&(ptr_m->matr[i].a), sizeof(int), 1, file);
        if (write_count != 1){
            fclose(file);
            if (feof(file)){
                return ERR_EOF;
            }
            else {
                return ERR_WRITE;
            }
        }
    }
    for (int j = 0; j < ptr_m->lines; j++){
        for (int k = ptr_m->matr[j].n - 1; k >= 0; k--){
            err_seek = fseek(ptr_m->f, ptr_m->matr[j].a + k * sizeof(int), SEEK_SET);
            if (err_seek != 0){
                fclose(file);
                if (feof(ptr_m->f)){
                    return ERR_EOF;
                }
                else {
                    return ERR_SEEK;
                }
            }
            read_count = fread(&el, sizeof(int), 1, ptr_m->f); 
            if (read_count != 1){
                fclose(file);
                if (feof(ptr_m->f)){
                    return ERR_EOF;
                }
                else {
                    return ERR_READ;
                }
            }
            write_count = fwrite(&el, sizeof(int), 1, file); 
            if (write_count != 1){
                fclose(file);
                if (feof(file)){
                    return ERR_EOF;
                }
                else {
                    return ERR_WRITE;
                }
            }
        }
        printf("\n");
    }

    fclose(file);

    return ERR_OK;

}




