#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_COUNTER 3
/* Общая переменная используемая нитями */
int shared_counter = 0;

/* функция - прототип нити */
void* thr_fn(void *arg) {
    /* локальная переменная */
    //sleep(abs(5 - (int)arg));
    /* рандомная генерация */
    int local = 1 + rand() % 10;
    sleep(local);
    printf("***Thread #%d with ID %#x: shared value is %d, local value is %d\n", 
            (int)arg, (unsigned int) pthread_self(), shared_counter++, local++);
    return NULL;
}

int main() {
    pthread_t th_id;
    int i, err;
    for (i = 0; i < NUM_COUNTER; i++) {
        err = pthread_create(&th_id, NULL, thr_fn, (void *) i);
        printf("MAIN: created thread with ID %#x\n", (unsigned int) th_id);
    }
    sleep(30);
    return 0;
}