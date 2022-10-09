#include "client.h"
#include "utils.h"

void startWriter(int sock, struct sockaddr_in addr, char *argv[]) {

  printf("Démarre un nouveau client de tchat en écriture...\n");
  // char *ip = argv[2];
  // char *port = argv[3];
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
  // char *ip = argv[2];
  // char *port = argv[3];

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
