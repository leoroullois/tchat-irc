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

#include "src/client.h"
#include "src/server.h"

#define PORT 5000
#define BUF_SIZE 1024

static int socketToClose;

void signalHandler() {
  printf("okay let's go\n");
  shutdown(socketToClose, 2);
  exit(0);
}

int main(int argc, char *argv[]) {
  // Searching for the protocol :
  // struct protoent *protoent;
  // protoent = getprotobyname("tcp");

  // Searching for the service :
  // struct servent *servent;
  // servent = getservbyname("tcp", protoent->p_name);

  // Searching for the host name :
  struct hostent *hostent;
  char *host = "localhost";
  hostent = gethostbyname(host);

  // Initialisation of the sockaddr_in data structure :
  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = ((struct in_addr *)(hostent->h_addr))->s_addr;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  socketToClose = sock;

  signal(SIGINT, SIG_IGN);
  signal(SIGINT, signalHandler);
  if (strcmp(argv[1], "-s") == 0) {
    // Naming :
    int code;
    code = bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    if (code == -1) {
      perror("bind");
      exit(1);
    }

    socklen_t len = sizeof(struct sockaddr_in);
    getsockname(sock, (struct sockaddr *)&addr, &len);

    startServer(sock, addr);
  } else if (strcmp(argv[1], "-w") == 0) {
    startWriter(sock, addr, argv);
  } else if (strcmp(argv[1], "-r") == 0) {
    startReader(sock, addr, argv);
  }
  return 0;
}
