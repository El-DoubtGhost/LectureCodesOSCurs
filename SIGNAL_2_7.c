/* Программа "будильник" */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>     


void alrm(int s) {
    printf("\n Жду имя \n\tимя:");
    alarm(5);
}

int main(){
    char string[80];
    signal(SIGALRM, alrm);
    alarm(5);
    printf("Введите имя:\n");
    for (;;) {
        printf("\t имя: ");
        if (fgets(string, 80, stdin) != NULL) break;
    }
    printf("OK!");
    return 0;
}