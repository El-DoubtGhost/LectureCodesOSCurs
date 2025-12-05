#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/user.h>



int main(int argc, char **agrv) {
    pid_t pid;
    int status;
    struct user_regs_struct REG;

    if ((pid = fork()) == 0) {
        /*находимся в процессе-потомке, разрешаем
        трассировку*/
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("bin/task_13_chld", "task_13_chld", NULL); 
        /* замещаем тело процесса*/
        /* здесь процесс-потомок будет остановлен с
        сигналом SIG_TRAP, ожидая команды продолжения
        выполнения от управляющего процесса*/
    }
    /* в процессе родителе */
    while (1)
    {
        /* ждем, когда отлаживаемый процесс приостановится */
        wait(&status);
        /*читаем содержимое регистров отлаживаемого процесса */
        ptrace(PTRACE_GETREGS, pid, &REG, &REG);
        /* выводим статус отлаживаемого процесса, номер
        сигнала, который его остановил и значения
        прочитанных регистров */
        printf("signal = %d, status = %#x, EIP=%#x, ESP=%#x\n", WSTOPSIG(status), status, REG.rip, REG.rsp);
        if (WSTOPSIG(status) != SIGTRAP) {
            if (!WIFEXITED(status)) {
                /* завершаем выполнение трассируемого процесса */
                ptrace(PTRACE_KILL, pid, NULL, NULL);
            }
            break;
        }
        ptrace(PTRACE_CONT, pid, NULL, NULL);
    }
}