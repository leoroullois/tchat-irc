#include "utils.h"

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
