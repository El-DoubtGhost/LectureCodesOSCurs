/* Для синхронизации только сигналы */

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_CNT 10
int target_pid, cnt;
int fd[2];
int status;

void SigHndlr(int s) {
    if (cnt < MAX_CNT) {
        read(fd[0], &cnt, sizeof(int));
        printf("pid=%d cnt=%d \n", target_pid, cnt);
        cnt++;
        sleep(1);
        write(fd[1], &cnt, sizeof(int));
        kill(target_pid, SIGUSR1);
    }
    else if (target_pid == getppid()) {
        printf("Child is going to be terminated\n");
        close(fd[1]); close(fd[0]);
        exit(0); /* нормальное завершение */
    }
    else {
        kill(target_pid, SIGUSR1);
    }
}

void Init(int s) {
    target_pid = getppid();
    printf("SON: target = %d, PID = %d\n",target_pid, getpid());
    write(fd[1], &cnt, sizeof(int));
    kill(target_pid, SIGUSR1);
}

int main(int argc, char **argv) {
    pipe(fd);
    signal(SIGUSR1, SigHndlr);
    signal(SIGUSR2, Init);
    cnt = 0;

    target_pid = fork();
    if (target_pid > 0) {
        // father
        kill(target_pid, SIGUSR2);
        printf("FATHER: target=%d, PID=%d\n", target_pid, getpid());
        
        while (wait(&status) == -1);

        printf("Parent is going to be terminated\n");
        close(fd[1]); close(fd[0]);
        return 0;
    }
    else {
        // son
        target_pid = getppid();
        for (;;);
    }
}