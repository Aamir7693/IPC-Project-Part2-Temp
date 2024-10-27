#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/kernel.h>
#include <errno.h>

// Define syscall numbers (replace these with your actual syscall numbers)
#define SYS_buffer_init 555      
#define SYS_buffer_cleanup 555   
#define SYS_producer_function 555 
#define SYS_consumer_function 555

// Function to call the producer_function system call in a pthread
void *producer_thread_fn(void *arg)
{
    long ret;

    printf("Producer thread started\n");

    // Call the producer_function system call
    ret = syscall(SYS_producer_function);
    if (ret != 0) {
        printf("Error: producer_function system call failed with return value: %ld, errno: %d\n", ret, errno);
    } else {
        printf("Producer thread completed successfully\n");
    }

    return NULL;
}

void *consumer_thread_fn(void *arg)
{
    long ret;

    printf("Consumer thread started\n");

    // Call the producer_function system call
    ret = syscall(SYS_consumer_function);
    if (ret != 0) {
        printf("Error: consumer_function system call failed with return value: %ld, errno: %d\n", ret, errno);
    } else {
        printf("consumer thread completed successfully\n");
    }

    return NULL;
}


int main()
{
    pthread_t producer_thread, consumer_thread;
    long ret;

    // Call the buffer_init system call to initialize the buffers
    printf("Initializing buffers with buffer_init system call\n");
    ret = syscall(SYS_buffer_init);
    if (ret != 0) {
        printf("Error: buffer_init system call failed with return value: %ld, errno: %d\n", ret, errno);
        return -1;
    }
    printf("Buffers initialized successfully\n");

    // Create the producer thread to call producer_function
    if (pthread_create(&producer_thread, NULL, producer_thread_fn, NULL) != 0) {
        printf("Error: Failed to create producer thread\n");
        return -1;
    }else{
	printf("Successfully created producer thread\n");
    }

    if (pthread_create(&consumer_thread, NULL, consumer_thread_fn, NULL) != 0) {
        printf("Error: Failed to create consumer thread\n");
        return -1;
    }else{
	printf("Successfully created consumer thread\n");
    }

    // Wait for the producer and consumer threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    printf("Producer and consumer threads finished\n");


    // Call the buffer_cleanup system call to clean up the buffers
    printf("Cleaning up buffers with buffer_cleanup system call\n");
    ret = syscall(SYS_buffer_cleanup);
    if (ret != 0) {
        printf("Error: buffer_cleanup system call failed with return value: %ld, errno: %d\n", ret, errno);
        return -1;
    }
    printf("Buffers cleaned up successfully\n");

    return 0;
}
