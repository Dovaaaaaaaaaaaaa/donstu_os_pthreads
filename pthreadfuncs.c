#include "pthreadfuncs.h"
#include <time.h>


// common resources - is a file for logging
int g_fd = -1;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

// git ID of current thread
pid_t getThreadID(void) {
    return (pid_t) syscall(SYS_gettid);
}

// write a string from thread with mutex
int write_line(const char *msg) {
    pthread_mutex_lock(&g_lock);

    ssize_t n = write(g_fd, msg, strlen(msg));

    pthread_mutex_unlock(&g_lock);

    if (n < 0) {
        fprintf(stderr, "write() failed: %s, [file descr = %d]\n",
                strerror(errno), g_fd);
        return -1;
    }
    return 0;
}

// function for thread
void *func_thread(void *arg) {
    //pthread_detach(pthread_self());

    struct ThreadArgs *t = (struct ThreadArgs *)arg;
    char buf[128];

    for (int i = 0; i < COUNT_ITERATIONS; ++i) {
        snprintf(buf, sizeof(buf),
                 "[tag = %s] msg = '%s' pid = %d ppid = %d tid = %d pthread_self = %lu iter = %d\n",
                 t->tag, t->message, getpid(), getppid(), getThreadID(),
                 (unsigned long)pthread_self(), i);
        write_line(buf);

        struct timespec ts;
        ts.tv_sec  = 10;
        ts.tv_nsec = 0;   // 0.1 сек
        nanosleep(&ts, NULL);
    }
      for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&g_lock);
        counter++;
        pthread_mutex_unlock(&g_lock);
    }
    free(t);     
    return NULL;
}

void about() {
    printf("Pthread example v2\n");
}
