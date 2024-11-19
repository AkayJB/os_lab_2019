#include <pthread.h>
#include <stdio.h>
#include <unistd.h> // Добавлено для sleep

pthread_mutex_t lock1, lock2;

void* thread1_func(void* arg) {
    pthread_mutex_lock(&lock1);
    printf("Thread 1 acquired lock1\n");
    sleep(1); // Задержка для демонстрации deadlock
    pthread_mutex_lock(&lock2);
    printf("Thread 1 acquired lock2\n");
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void* thread2_func(void* arg) {
    pthread_mutex_lock(&lock2);
    printf("Thread 2 acquired lock2\n");
    sleep(1); // Задержка для демонстрации deadlock
    pthread_mutex_lock(&lock1);
    printf("Thread 2 acquired lock1\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    return 0;
}