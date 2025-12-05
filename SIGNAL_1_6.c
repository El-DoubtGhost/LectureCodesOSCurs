#include <signal.h>
#include <sys/types.h>
#include <stdio.h>

int count = 0;

void SigHndl(int s) {
    printf("\n I got SIGINT %d times\n", ++count);
    if (count == 5) {
        signal(SIGINT, SIG_DFL);
    }
}

int main() {
    signal(SIGINT, SigHndl);
    while(1);
    return 0;
}
