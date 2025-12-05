/* СЕРВЕР */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {

    struct {
        long mestype;
        char mes[100];
    } messageto;

    struct {
        long mestype;
        long mes;
    } messagefrom;

    key_t key;
    int mesid;

    key = ftok("/tmp/example", 'r');
    mesid = msgget(key, 0666 | IPC_CREAT);
    for (;;) {
        msgrcv(mesid, (struct msgbuf*) (&messagefrom), sizeof(long), 1, 0);
        messageto.mestype = messagefrom.mes;
        strcpy(messageto.mes, "MessageForClient\n");
        msgsnd(mesid, (struct msgbuf*) (&messageto), 100 * sizeof(char), 0);
        msgctl(mesid, IPC_RMID, NULL);
        break;
    }
    return 0;
}
