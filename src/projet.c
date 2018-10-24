#include "timeUnit.h"
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 20

int f1[TABLE_SIZE] = {44, 77, 5, 7, 3, 33, 11, 31, 18, 35, 27, 55, 10, 28, 8, 20, 2, 14, 9, 16};

int my_rand(int min, int max) { return rand() % (max + 1 - min) + min; }

int main(int argc, char *argv[]) {
    struct TimeUnit min;
    min.m = 0;
    min.s = 30;
    min.ms = 0;

    struct TimeUnit max;
    max.m = 0;
    max.s = 45;
    max.ms = 0;

    int times[TABLE_SIZE];
    int minimum = toMs(min);
    int maximum = toMs(max);

    int i = 0;
    for (; i < TABLE_SIZE; i++) {
        int ra = my_rand(minimum, maximum);
        struct TimeUnit timeUnit = toTimeUnit(ra);
        printf("name: %d, min: %d, sec: %d, ms: %d\n", f1[i], timeUnit.m, timeUnit.s, timeUnit.ms);
    }

    exit(0);
}
