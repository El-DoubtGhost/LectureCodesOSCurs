#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORTNUM 50500
#define BUFLEN 80
#define QUESIZE 5 /* условный размер очереди запросов на соединение */

int main (int agrc, char **argv) {
    struct sockaddr_in addr;
    int sockfd;
    char buf[BUFLEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORTNUM);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
}