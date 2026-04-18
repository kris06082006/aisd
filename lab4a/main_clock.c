#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tree.h"
#include "clock.h"

#define BATCH_SIZE 1000

void reverse_keys(char **keys, int n) {
    for (int i = 0; i < n/2; i++) {
        char *temp = keys[i];
        keys[i] = keys[n-1-i];
        keys[n-1-i] = temp;
    }
}

int main(void) {
    struct timespec start, end;

    // timing for insert
    FILE *keyfile = fopen("keys.txt", "r");
    if (!keyfile) {
        printf("Не удалось открыть файл с ключами\n");
        return 1;
    }

    FILE *csv = fopen("insert_times.csv", "w");
    if (!csv) {
        printf("Не удалось открыть CSV-файл\n");
        fclose(keyfile);
        return 1;
    }

    Tree *tree = NULL;
    Err status = init_tree(&tree);
    const char *err = error_message(status);
    if (status != ERR_OK) {
        printf("%s", err);
        fclose(keyfile);
        fclose(csv);
        return 1;
    }

    char *keys[1000000];
    char key[128];
    int count = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    while (fgets(key, sizeof(key), keyfile)) {
        key[strcspn(key, "\n")] = '\0';
        keys[count] = strdup(key);

        status = insert_node(tree, key, "0");
        err = error_message(status);
        if (status != ERR_OK) {
            printf("%s", err);
        }

        count++;

        if (count % BATCH_SIZE == 0) {
            clock_gettime(CLOCK_MONOTONIC, &end);
            long duration = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            fprintf(csv, "%d,%ld\n", count, duration);
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
    }

    fclose(keyfile);
    fclose(csv);

    reverse_keys(*keys, count);

    // timing for deletion
    FILE *delcsv = fopen("delete_times.csv", "w");
    if (!delcsv) {
        printf("Не удалось открыть CSV-файл для удаления\n");
        free_tree(tree);
        for (int i = 0; i < count; ++i) {
            free(keys[i]);
        }
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < count; ++i) {
        if (keys[i] == NULL) continue;

        TreeNode *node = search_node_by_key(tree, keys[i], &status);
        if (status != ERR_OK) {
            printf("Ошибка поиска: %s [%s]\n", error_message(status), keys[i]);
            continue;
        }

        if (node == NULL) {
            printf("Узел не найден для ключа: %s\n", keys[i]);
            continue;
        }

        status = delete_node(tree, node);
        if (status == ERR_OK) {
            free(keys[i]);
            keys[i] = NULL;
        } else {
            printf("Ошибка удаления: %s [%s]\n", error_message(status), keys[i]);
        }

        if ((i + 1) % BATCH_SIZE == 0) {
            clock_gettime(CLOCK_MONOTONIC, &end);
            long duration = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            fprintf(delcsv, "%d,%ld\n", i + 1, duration);
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
    }

    fclose(delcsv);
    free_tree(tree);

    for (int i = 0; i < count; ++i) {
        free(keys[i]);
    }

    return 0;
}

