#include "pthreadfuncs.h"

// git ID of current thread
static pid_t getThreadID(void) {
    return (pid_t) syscall(SYS_gettid);
}

// write a string from thread with mutex
static void write_line(const char *msg) {
    pthread_mutex_lock(&g_lock);
    ssize_t n = write(g_fd, msg, strlen(msg));
    if (n < 0) {
        // return error
        fprintf(stderr, "write() failed: %s\n", strerror(errno));
    }
    pthread_mutex_unlock(&g_lock);
}

// function for thread
static void *func_thread(void *arg){
struct thread_arg *t = (struct thread_arg *)arg;
    char buf[128];
    // write something in opened file
    for (int i = 0; i < COUNT_ITERATIONS; ++i) {
        snprintf(buf, sizeof(buf),
                 "[tag=%c] pid=%d tid=%d iter=%d\n",
                 t->tag, getpid(), get_tid(), i);

        write_line(buf);

        /* Имитация полезной работы */
        usleep(100 * 1000); /* 100 мс */
    }

    return NULL;
}

void about()  {
    printf("Pthread example\n");
}