#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATIONS 5

int buffer = 0;
int has_data = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    (void)arg;
    for (int i = 1; i <= ITERATIONS; i++) {
        pthread_mutex_lock(&mtx);
        while (has_data) {
            pthread_cond_wait(&cond, &mtx);
        }
        buffer = i;
        has_data = 1;
        printf("producer: put %d\n", buffer);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
        usleep(100000);
    }
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    for (int i = 1; i <= ITERATIONS; i++) {
        pthread_mutex_lock(&mtx);
        while (!has_data) {
            pthread_cond_wait(&cond, &mtx);
        }
        int value = buffer;
        has_data = 0;
        printf("consumer: got %d\n", value);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
        usleep(150000);
    }
    return NULL;
}

int main(void) {
    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);
    return 0;
}
