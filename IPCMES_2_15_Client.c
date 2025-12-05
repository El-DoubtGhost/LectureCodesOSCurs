#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    struct {
        long mestype;
        long mes;
    } messageto;

    struct {
        long mestype;
        char mes[100];
    } messagefrom;

    key_t key;
    int mesid;
    long pid = getpid();

    key = ftok("/tmp/example", 'r');
    mesid = msgget(key, 0666);

    messageto.mestype = 1;
    messageto.mes = pid;
    msgsnd(mesid, (struct msgbuf*) (&messageto), sizeof(long), 0);
    msgrcv(mesid, (struct msgbuf*) (&messagefrom), 100 * sizeof(char), pid, 0);
    printf("%s", messagefrom.mes);
    return 0;
}