#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include "utils_2.h"
#include "sum.h"

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *)args;
    unsigned long long int *result = malloc(sizeof(unsigned long long int));
    *result = Sum(sum_args);
    return (void *)result;
}

int main(int argc, char **argv) {
    uint32_t threads_num = 0;
    uint32_t seed = 0;
    uint32_t array_size = 0;

    while (true) {
        static struct option options[] = {
            {"threads_num", required_argument, 0, 0},
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);
        if (c == -1) break;

        switch (option_index) {
            case 0:
                threads_num = atoi(optarg);
                break;
            case 1:
                seed = atoi(optarg);
                break;
            case 2:
                array_size = atoi(optarg);
                break;
            default:
                printf("Usage: %s --threads_num num --seed num --array_size num\n", argv[0]);
                return 1;
        }
    }

    if (threads_num == 0 || seed == 0 || array_size == 0) {
        printf("Usage: %s --threads_num num --seed num --array_size num\n", argv[0]);
        return 1;
    }

    pthread_t threads[threads_num];
    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);

    struct SumArgs args[threads_num];
    int chunk_size = array_size / threads_num;

    clock_t start_time = clock();

    for (uint32_t i = 0; i < threads_num; i++) {
        args[i].array = array;
        args[i].begin = i * chunk_size;
        args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * chunk_size;

        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i]) != 0) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }

    unsigned long long int total_sum = 0;
    for (uint32_t i = 0; i < threads_num; i++) {
        unsigned long long int *result;
        pthread_join(threads[i], (void **)&result);
        total_sum += *result;
        free(result);
    }

    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    free(array);
    printf("Total: %llu\n", total_sum);
    printf("Time taken to calculate sum: %.6f seconds\n", time_taken);
    return 0;
}