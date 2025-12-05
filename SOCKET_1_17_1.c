#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORTNUM 50500
#define BUFLEN 80
#define QUESIZE 5 /* условный размер очереди запросов на соединение */