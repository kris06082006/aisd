#include "clock.h"

void mix_keys(char **keys, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = keys[i];
        keys[i] = keys[j];
        keys[j] = temp;
    }
}
