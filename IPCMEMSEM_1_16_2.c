/* необходимо корректно определить существование ресурса,
если он есть - подключиться */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

#define NMAX 256

int main(int argc, char **argv) {
    key_t key;
    int semid, shmid;
    struct sembuf sops;
    char *shmaddr;
    char str[NMAX];

    /* создаем тот же самый ключ */
    key = ftok("/tmp/ter/exmpl",'S');

    /* аналогично предыдущему процессу - инициализация
    ресурсов */
    semid = semget(key, 1, 0666);
    shmid = shmget(key, NMAX, 0666 );
    shmaddr = shmat(shmid, NULL, 0);
    sops.sem_num = 0;
    sops.sem_flg = 0;

    /* запускаем цикл */
    do {
        printf("Waiting… \n"); /* ожидание на семафоре */
        sops.sem_op = -2;
        /* будем ожидать, пока “значение семафора” +
        ”значение sem_op” не станет положительным*/
        semop(semid, &sops, 1);/* теперь значение семафора
        равно 1 */

        /*критическая секция - работа с разделяемой памятью
        - в этот момент первый процесс к разделяемой памяти
        доступа не имеет*/
        strcpy(str, shmaddr); /* копируем строку из разд.памяти */

        if (str[0] == 'Q') {
            /* завершение работы – освобождаем
            разделяемую память */
            shmdt(shmaddr);
        }

        /*после работы с текущей строкой– обнулим семафор*/
        sops.sem_op=-1;
        semop(semid, &sops, 1);
        printf("Read from shared memory: %s\n", str);
    } while (str[0] != 'Q');

    return 0;
}