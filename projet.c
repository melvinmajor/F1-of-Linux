#include <stdio.h>
#include <stdlib.h>

int f1[20] = {44, 77, 5,  7,  3, 33, 11, 31, 18, 35,
              27, 55, 10, 28, 8, 20, 2,  14, 9,  16};

int main(int argc, char *argv[]) {
  int min = 30 * 60 * 1000;
  int max = 45 * 60 * 1000;

  for (int i = 0; i < 20; i++) {
    printf("name: %d, time:%d\n", f1[i], my_rand(min, max));
  }

  exit(0);
}

int my_rand(int min, int max) { return rand() % (max + 1 - min) + min; }
