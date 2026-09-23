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
    struct ThreadArgs args[COUNT_THREADS];

    // sys call - open
    // file, modes, rights
    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("main: pid = %d, opened file: \'output.log\' (fd = %d)\n", getThreadID(), g_fd);

    // create structs for threads
    args[0].id  = 1;
    strcpy(args[0].tag, "First\0");
    args[1].id  = 2;
    strcpy(args[1].tag, "Second\0");
    args[2].id  = 3;
    strcpy(args[2].tag, "Third\0");
    args[3].id  = 4;
    strcpy(args[3].tag, "Fourth\0");
    
    // create threads
    for (int i = 0; i < COUNT_THREADS; i++) {
        int rc = pthread_create(&threads[i], NULL, func_thread, &args[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            return EXIT_FAILURE;
        }
    }

    // wait stoping all thread
    for (int i = 0; i < COUNT_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // sys call for close file
    if (close(g_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }
    // remove mutex
    pthread_mutex_destroy(&g_lock);
    printf("main: all threads finished, file closed\n");
    return EXIT_SUCCESS;
}