#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* декларация структуры сообщения */
struct {
    long mtype;
    char data[256];
} Message;

int main(int argc, char **argv) {
    key_t key;
    int msgid;
    char str[256];
    /* получаем уникальный ключ, однозначно определяющий
    доступ к ресурсу данного типа */
    key = ftok("/tmp/mash", 't');
    /* создаем новую очередь сообщений,
    0666 определяет права доступа */
    msgid = msgget(key, 0666 | IPC_CREAT);
    /* запускаем вечный цикл */
    for (;;) {
        fgets(str, 256, stdin); /* читаем из стандартного ввода строку */
        /* и копируем ее в буфер сообщения */
        strcpy(Message.data, str);
        /* анализируем первый символ прочитанной строки */
        switch (str[0]) {
            case 'a':
            case 'A':
                /* устанавливаем тип 1 для ПРОЦЕССА A*/
                Message.mtype = 1;
                /* посылаем сообщение в очередь */
                msgsnd(msgid, (struct msgbuf*) (&Message), strlen(str) + 1, 0);
                break;
            case 'b':
            case 'B':
                /* устанавливаем тип 2 для ПРОЦЕССА A*/
                Message.mtype = 2;
                msgsnd(msgid, (struct msgbuf*) (&Message), strlen(str) + 1, 0);
            break;
            case 'q':
            case 'Q':
                Message.mtype = 1;
                msgsnd(msgid, (struct msgbuf*) (&Message), strlen(str) + 1, 0);
                Message.mtype = 2;
                msgsnd(msgid, (struct msgbuf*) (&Message), strlen(str) + 1, 0);
                /* ждем получения сообщений процессами A и В*/
                sleep(10);
                msgctl(msgid, IPC_RMID, NULL);
                exit(0);
            default:
                /* игнорируем остальные случаи */
                break;
        }
    }
}