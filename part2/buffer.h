#ifndef BUFFER_H
#define BUFFER_H

#include <linux/semaphore.h>   // For kernel semaphores
#include <linux/mutex.h>       // For kernel mutexes
#include <linux/time64.h>      // For time structures in the kernel
#include <linux/types.h>       // For basic types like bool

#define BUFFER_SIZE 10
#define TEMP_BUFFER_SIZE 16
#define MAX_DATA_LEN 256
#define NUM_APPS 3

// Kernel equivalent of time_t is struct timespec64
struct buffer_item {
    int app_id;
    int priority;
    int sleep_interval;
    char data[MAX_DATA_LEN];
    struct timespec64 timestamp;  // Kernel space time representation
};

// Use kernel mutex and semaphores for circular buffer
struct circular_buffer {
    struct buffer_item items[BUFFER_SIZE];
    int read, write, count;
    struct semaphore mutex;   // Semaphore for mutual exclusion
    struct semaphore full;    // Semaphore for tracking filled slots
    struct semaphore empty;   // Semaphore for tracking empty slots
};

// Use kernel mutex for temp_buffer
struct temp_buffer {
    struct buffer_item items[TEMP_BUFFER_SIZE];
    int count;
    struct mutex mutex;       // Kernel mutex for temp buffer
};

// Declare global variables
extern struct circular_buffer *main_buffer;
extern struct temp_buffer *temp_buffers;
extern bool should_stop;

// Declare functions that will be defined in the source file (likely as syscalls)
void buffer_init(struct temp_buffer *temp_buffers);
void buffer_cleanup(struct temp_buffer *temp_buffers);
void producer_function(void *arg);
void consumer_function(void *arg);

#endif
