#include <stdlib.h>
#include "utils_2.h"

void GenerateArray(int *array, int size, int seed) {
    srand(seed);
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100; // Генерация чисел от 0 до 99
    }
}