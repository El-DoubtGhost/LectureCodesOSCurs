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
    struct sockaddr_in own_addr, 
                       party_addr;

    int sockfd, newsockfd;
    int party_len, len;
    char buf[BUFLEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&own_addr, 0, sizeof(own_addr));

    own_addr.sin_family = AF_INET;
    own_addr.sin_addr.s_addr = INADDR_ANY;
    own_addr.sin_port = htons(PORTNUM);

    bind(sockfd, (struct sockaddr*) &own_addr, sizeof(own_addr));

    listen(sockfd, QUESIZE);

    while (1) {
        memset(&party_addr, 0, sizeof(party_addr));
        party_len = sizeof(party_addr);

        newsockfd = accept(sockfd, (struct sockaddr*) &party_addr, &party_len);

        if (fork() == 0) {
            close(sockfd);
            len = recv(newsockfd, &buf, BUFLEN, 0);
            printf("received string %s from client %s:%d \n", buf, inet_ntoa(party_addr.sin_addr.s_addr), ntohs(party_addr.sin_port));

            sprintf(buf, "%d", strlen(buf));
            send(newsockfd, &buf, strlen(buf) + 1, 0);
            close(newsockfd);

            return 0;
        }
        
        close(newsockfd);
    }
}