/* Двухпроцессный "будильник" */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void alr(int s) {
    printf("\nБыстрее!!!\n");
}

int main(int argc, char **argv) {
    char string[80];
    pid_t pid;
    signal(SIGALRM, alr);
    if (pid = fork()) {
        /* батя */
        for (;;) {
            sleep(5);
            kill(pid, SIGALRM);
        }
    }
    else {
        /* сыночка */
        printf("Введите имя: \n");
        for (;;) {
            printf("\tимя:");
            if (fgets(string, 80, stdin) != NULL) break;
        }
        printf("Ок!\n");
        kill(getppid(), SIGTERM);
        
    }
    return 0;
}