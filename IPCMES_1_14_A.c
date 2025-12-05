#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
struct {
    long mtype; /* тип сообщения */
    char Data[256]; /* сообщение */
} Message;


int main(int argc, char **argv) {
    key_t key;
    int msgid;
    /* получаем ключ по тем же параметрам */
    key = ftok("/tmp/mash",'t');
    /*подключаемся к очереди сообщений */
    msgid = msgget(key, 0666);
    /* запускаем вечный цикл */
    for(;;) {
        /* читаем сообщение с типом 1 для ПРОЦЕССА A */
        msgrcv(msgid, (struct msgbuf*) (&Message), 256, 1, 0);
        printf("%s", Message.Data);
        if(Message.Data[0] == 'q' || Message.Data[0] == 'Q') break;
    }
    return 0;
}