#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

// count of threads
#define COUNT_THREADS 4
// count of iterations
#define COUNT_ITERATIONS 3

// struct send to thread
struct ThreadArgs {
    // my ID of thread
    int  id;
    // something string (with name of thread, f.e.)
    char tag[10];
};


// common resources - is a file for logging
static int g_fd = -1;

// mutex 
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

// git ID of current thread
static pid_t getThreadID(void);

// write a string from thread with mutex
static void write_line(const char *msg);

// pointer for thread's function
static void *func_thread(void *arg);

void about(void);