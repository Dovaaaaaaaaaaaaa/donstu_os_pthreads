#include "pthreadfuncs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>




int main(void) {
    // headline
    about();
    // array with threads
    pthread_t threads[COUNT_THREADS];
    struct ThreadArgs args[COUNT_THREADS];

    // sys call - open
    // file, modes, rights
    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("main: pid = %d, opened file: \'output.log\' (fd = %d)\n", getpid(), g_fd);

    // create structs for threads
    args[0].id  = 1;
    strcpy(args[0].tag, "First");
    args[1].id  = 2;
    strcpy(args[1].tag, "Second");
    args[2].id  = 3;
    strcpy(args[2].tag, "Third");
    args[3].id  = 4;
    strcpy(args[3].tag, "Fourth");
    

    /* --- Создаём потоки --- */
    for (int i = 0; i < COUNT_THREADS; i++) {


        int rc = pthread_create(&threads[i], NULL, worker, &args[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            return EXIT_FAILURE;
        }
    }

    /* --- Ждём завершения --- */
    for (int i = 0; i < COUNT_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* --- Системный вызов close() --- */
    if (close(g_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }

    pthread_mutex_destroy(&g_lock);
    printf("main: все потоки завершены, файл закрыт\n");
    return EXIT_SUCCESS;
}