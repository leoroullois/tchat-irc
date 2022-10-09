#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"

#define PORT 5000
#define BUF_SIZE 1024

typedef struct Writer {
  int sock;
  char *name;
} Writer;

void handleWriter(Writer writer, int reader, struct sockaddr_in addr, int len);
void startServer(const int sock, struct sockaddr_in addrServ);
