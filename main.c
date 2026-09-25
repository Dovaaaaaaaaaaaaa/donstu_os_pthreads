#include "pthreadfuncs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>


int main(void) {
    // headline
    about();

    // array with threads
    pthread_t threads[COUNT_THREADS];

    // sys call - open
    // file, modes, rights
    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    // write headline to log
    char buf[128];
    snprintf(buf, sizeof(buf),
             "main: pid = %d, opened file: 'output.log' (fd = %d)\n",
             getpid(), g_fd);
    if (write_line(buf) != 0) {
        fprintf(stderr, "main: write_line failed\n");
    }

    // create structs for threads via malloc
    // (задание 26: detached-потоку нельзя передавать стек main)
    for (int i = 0; i < COUNT_THREADS; i++) {
        struct ThreadArgs *arg = malloc(sizeof(struct ThreadArgs));
        if (arg == NULL) {
            perror("malloc");
            close(g_fd);
            return EXIT_FAILURE;
        }

        arg->id = i;
        snprintf(arg->tag, sizeof(arg->tag), "T%d", i);
        snprintf(arg->message, sizeof(arg->message),
                 "hello from thread %d", i);

        int rc = pthread_create(&threads[i], NULL, func_thread, arg);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            free(arg);
            close(g_fd);
            return EXIT_FAILURE;
        }
    }

    // wait stopping all threads
    // (для detached-потоков join вернёт ошибку — это ожидаемо)
    for (int i = 0; i < COUNT_THREADS; i++) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "join thread %d failed: %s\n", i, strerror(rc));
        }
    }
        printf("counter = %d (expected %d)\n",
           counter, COUNT_THREADS * 100000);
    // write final message to log WHILE file is still open
    if (write_line("main: all threads finished, file closed\n") != 0) {
        fprintf(stderr, "main: write_line failed\n");
    }

    // sys call for close file
    if (close(g_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }

    // remove mutex
    pthread_mutex_destroy(&g_lock);

    return EXIT_SUCCESS;
}
