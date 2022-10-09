#include "server.h"

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
