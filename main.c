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

#define PORT 5000
#define BUF_SIZE 1024

typedef struct Writer {
  int sock;
  char *name;
} Writer;

void printTime(void) {
  time_t now = time(NULL);
  struct tm *tm_struct = localtime(&now);

  int hour = tm_struct->tm_hour;
  int min = tm_struct->tm_min;
  int sec = tm_struct->tm_sec;
  char *padStart = "";

  if (sec < 10) {
    padStart = "0";
  }

  printf("%d:%d:%s%d", hour, min, padStart, sec);
}

void handleWriter(Writer writer, int reader, struct sockaddr_in addr, int len) {

  pid_t pid;

  pid = fork();

  switch (pid) {
  case -1:
    perror("fork");
    exit(1);
    break;
  case 0:
    while (1) {
      char msg[BUF_SIZE] = "";
      recv(writer.sock, msg, BUF_SIZE, 0);

      char output[BUF_SIZE] = "";
      strcat(output, writer.name);
      strcat(output, " a dit : ");
      strcat(output, msg);

      printTime();
      printf(" %s\n", output);
      send(reader, output, BUF_SIZE, 0);
    }
    exit(0);
    break;
  default:
    break;
  }
}

void startServer(const int sock, struct sockaddr_in addrServ) {

  listen(sock, 5);

  int port = ntohs(addrServ.sin_port);
  char *ip = inet_ntoa(addrServ.sin_addr);
  printf("IP: %s\n", ip);
  printf("PORT: %d\n", port);

  while (1) {
    int clientSock;
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    clientSock = accept(sock, (struct sockaddr *)&addr, &len);

    char name[BUF_SIZE] = "";
    recv(clientSock, name, BUF_SIZE, 0);

    int reader;
    if (strcmp(name, "r") == 0) {
      reader = clientSock;
    } else {
      Writer writer;
      writer.sock = clientSock;
      writer.name = name;

      printf("%s joined the tchat.\n", name);
      handleWriter(writer, reader, addr, len);
    }
  }
}

void startWriter(int sock, struct sockaddr_in addr, char *argv[]) {

  printf("Démarre un nouveau client de tchat en écriture...\n");
  char *ip = argv[2];
  char *port = argv[3];
  char *name = argv[4];

  // Connexion au serveur
  socklen_t len = sizeof(struct sockaddr_in);
  if (connect(sock, (struct sockaddr *)&addr, len) < 0) {
    printf("Failed connect");
    exit(1);
  }

  // Une fois la connexion effectuée, envoie du nom de l'utilisateur
  send(sock, name, strlen(name), 0);

  // Boucle pour l'envoie de messages
  while (1) {
    char msg[BUF_SIZE] = "";
    printf("> ");
    fgets(msg, BUF_SIZE, stdin);
    send(sock, msg, strlen(msg), 0);
  }
}

void startReader(int sock, struct sockaddr_in addr, char *argv[]) {
  printf("Démarre un nouveau client de tchat en lecture...\n");
  char *ip = argv[2];
  char *port = argv[3];

  socklen_t len = sizeof(struct sockaddr_in);

  if (connect(sock, (struct sockaddr *)&addr, len) < 0) {
    printf("Failed connect");
    exit(1);
  }

  send(sock, "r", 1, 0);

  while (1) {
    char msg[BUF_SIZE] = "";
    recv(sock, msg, BUF_SIZE, 0);
    printTime();
    printf(" %s\n", msg);
  }
}

static int socketToClose;

void signalHandler() {
  printf("okay let's go\n");
  shutdown(socketToClose, 2);
  exit(0);
}

int main(int argc, char *argv[]) {
  // Searching for the protocol :
  struct protoent *protoent;
  protoent = getprotobyname("tcp");

  // Searching for the service :
  struct servent *servent;
  servent = getservbyname("tcp", protoent->p_name);

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
