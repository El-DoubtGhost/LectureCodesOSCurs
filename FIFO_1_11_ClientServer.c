/* процесс-сервер*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int fd;
int pid;

void SysHndlr(int s) {
    close(fd);
    unlink("fifo");
    exit(0);
}


int main(int argc, char **argv) {
    /*создали специальный файл FIFO с открытыми для всех
    правами доступа на чтение и запись*/
    signal(SIGINT, SysHndlr);
    mkfifo("fifo", 0666);
    /* открыли канал на чтение*/
    fd = open("fifo", O_RDONLY);
    while (read (fd, &pid, sizeof(int)) >0) {
        printf("Server %d got message from %d !\n", getpid(), pid);
    };
    close(fd);
    unlink("fifo");
    return 0;
}