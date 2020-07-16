#include <stdlib.h>

void simple_shuffle(void** arr, int size) {
    for (int c = 0; c < size; c++) {
        int rand_point = rand() % size;
        void* temp = arr[c];
        arr[c] = arr[rand_point];
        arr[rand_point] = temp;
    }
}
