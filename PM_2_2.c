#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        int status;
        if (fork() > 0) {
            wait(&status);
            printf("process-father\n");
            continue;
        }
        execlp(argv[i], argv[i], NULL);
        exit(1);
    }
}