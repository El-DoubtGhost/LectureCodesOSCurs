#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    if ((fork()) == 0) {
        execl("/bin/echo", "echo", "This is", "string1", NULL);
        exit(1);
    }
    if ((fork()) == 0) {
        execl("/bin/echo", "echo", "This is", "string2", NULL);
        exit(1);
    }
    printf("process-father is waiting for children\n");
    while (wait(NULL) != -1);
    // wait вернет -1, если ждать больше некого
    printf("all children terminated\n");
    exit(0);
}