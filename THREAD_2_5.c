#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void* thr_fn(void *arg) {
    sleep(time(0) % 10);
    pthread_exit(NULL);
}

int main() {
    pthread_t th_id;
    void* answer;
    int err = pthread_create(&th_id, NULL, thr_fn, NULL);
    if (err) {
        printf("pthread_create ended with exit code: %u\n", (unsigned int) err);
        return 1;
    }
    time_t time_start = time(0);
    err = pthread_join(th_id, &answer);
    if (err) {
        printf("pthread_join ended with exit code: %u\n", (unsigned int) err);
        return 1;
    }
    if (answer) { 
        printf("thread ended with exit code: %p\n", answer);
        return 1;
    }
    printf("Thread %#x execution time = %u\n", (unsigned int) th_id, (unsigned int) (time(0) - time_start));
    return 0;
}