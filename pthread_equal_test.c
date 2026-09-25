#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// первый поток: сохраняет свой pthread_t и сравнивает с чужим
pthread_t first_tid;

void *thread1(void *arg) {
    (void)arg;
    first_tid = pthread_self();
    printf("thread1: pthread_self = %lu\n", (unsigned long)first_tid);
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    pthread_t me = pthread_self();
    printf("thread2: pthread_self = %lu\n", (unsigned long)me);

    if (me == first_tid) {
        printf("thread2: same as thread1\n");
    } else {
        printf("thread2: different from thread1\n");
    }

    // сравним поток с самим собой
    if (pthread_equal(me, me)) {
        printf("thread2: same as itself\n");
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_join(t1, NULL);   // дождаться, чтобы first_tid был заполнен

    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t2, NULL);

    // сравнить main-поток с потоком t2
    if (pthread_equal(pthread_self(), t2)) {
        printf("main: same as thread2\n");
    } else {
        printf("main: different from thread2\n");
    }

    return 0;
}
