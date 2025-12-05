#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_CNT 10
int target_pid, cnt;
int fd[2];
int status;

void SigHndlr(int s) {
    /* в обработчике сигнала происходит и чтение, и запись */
    if(cnt < MAX_CNT) {
        read(fd[0], &cnt, sizeof(int));
        printf("pid=%d cnt=%d \n", target_pid, cnt);
        cnt++;
        sleep(1);
        write(fd[1], &cnt, sizeof(int));
        /* посылаем сигнал второму: пора читать из канала */
        kill(target_pid, SIGUSR1);
    }
    else if(target_pid == getppid()){
        /* условие окончания игры проверяется потомком */
        printf("Child is going to be terminated\n");
        close(fd[1]);
        close(fd[0]);
        /* завершается потомок */
        exit(0); /* нормальное завершение */
    }
    else {
        kill(target_pid, SIGUSR1);  
    }   
}

int main(int argc, char **argv) {
    pipe(fd);
    signal(SIGUSR1, SigHndlr);
    cnt = 0;

    //printf("%ld", sizeof(&cnt));

    if (target_pid = fork()) {
        /* В родительском процессе: */
        write(fd[1], &cnt, sizeof(int)); /* старт. синхр.*/
        /* ждем завершения потомка; если при этом происходит
        поступление сигнала, то системный вызов wait прерывается с
        кодом завершения -1, поэтому используется цикл */
        while(wait(&status)== -1);

        printf("Завершение родительского процесса\n");
        close(fd[1]);
        close(fd[0]);
        return 0;
    }
    else {
        read(fd[0], &cnt, sizeof(int));
        target_pid = getppid();

        write(fd[1], &cnt, sizeof(int));
        /* сыновний процесс начинает пинг-понг */
        kill(target_pid, SIGUSR1);
        for(;;);
    }
}