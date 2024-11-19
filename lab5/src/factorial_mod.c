#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Глобальные переменные
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
long result = 1;
int k, mod, pnum;

// Функция для вычисления частичного факториала
void* partial_factorial(void* arg) {
    int start = *((int*)arg);
    int end = start + k / pnum;
    for (int i = start; i <= end && i <= k; i++) {
        pthread_mutex_lock(&mut);
        result = (result * i) % mod;
        pthread_mutex_unlock(&mut);
    }
    return NULL;
}

// Функция для разбора аргументов командной строки
void parse_arguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            k = atoi(argv[++i]);
        } else if (strncmp(argv[i], "--pnum=", 7) == 0) {
            pnum = atoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--mod=", 6) == 0) {
            mod = atoi(argv[i] + 6);
        } else {
            fprintf(stderr, "Usage: %s -k <num> --pnum=<num> --mod=<num>\n", argv[0]);
            exit(1);
        }
    }
    if (k <= 0 || pnum <= 0 || mod <= 0) {
        fprintf(stderr, "Invalid arguments. All values must be positive integers.\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    // Разбор аргументов командной строки
    parse_arguments(argc, argv);

    pthread_t threads[pnum];
    int starts[pnum];

    // Создание потоков
    for (int i = 0; i < pnum; i++) {
        starts[i] = i * (k / pnum) + 1;
        pthread_create(&threads[i], NULL, partial_factorial, &starts[i]);
    }

    // Ожидание завершения потоков
    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Factorial mod %d = %ld\n", mod, result);
    return 0;
}