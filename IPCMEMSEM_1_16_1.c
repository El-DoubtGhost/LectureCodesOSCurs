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

    /* создаем уникальный ключ */
    key = ftok("/tmp/ter/exmpl", 'S');

    /* Должен быть обеспечен порядок запуска процессов*/
    /* сначала первый, а затем второй */

    /*создаем один семафор с определенными правами доступа*/
    semid = semget(key, 1, 0666 | IPC_CREAT);

    /* создаем разделяемую память на 256 байт */
    shmid = shmget(key, NMAX, 0666 | IPC_CREAT);

    /* подключаемся к разделяемой памяти, в shaddr –
    указатель на буфер с разделяемой памятью */
    shmaddr = shmat(shmid, NULL, 0);

    /* инициализируем семафор значением 0 */
    semctl(semid,0,SETVAL, (int) 0);
    sops.sem_num = 0;
    sops.sem_flg = 0;

    do { /* запуск цикла */
        printf("Введите строку:");
        if (fgets(str, NMAX, stdin) == NULL)
        {
            /* окончание ввода */
            /* пишем признак завершения – строку “Q” */
            strcpy(str, "Q");
        }

        /* в текущий момент семафор открыт для этого
        процесса */
        strcpy(shmaddr, str); /* копируем строку в разд.
        память */

        /* предоставляем второму процессу возможность войти */
        sops.sem_op = 3; /* увеличение семафора на 3 */
        semop(semid, &sops, 1);

        /* ждем, пока семафор будет открыт для 1го процесса
        - для следующей итерации цикла */
        sops.sem_op = 0; /* ожидание обнуления семафора */
        semop(semid, &sops, 1);
    } while (str[0] != 'Q');

    /* в данный момент второй процесс уже дочитал из
    разделяемой памяти и отключился от нее – можно ее
    удалять*/
    shmdt(shmaddr) ; /* отключаемся от разделяемой памяти */
    shmctl(shmid, IPC_RMID, NULL); /* уничтожаем разделяемую память */
    semctl(semid, 0, IPC_RMID, (int) 0); /*уничтожаем семафор*/
    return 0;
}