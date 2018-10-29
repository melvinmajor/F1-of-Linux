#include <stdio.h>
#include <stdlib.h>
#include "timeUnit.h"
#include "car.h"

#define TABLE_SIZE 20

int f1[TABLE_SIZE] = {44, 77, 5, 7, 3, 33, 11, 31, 18, 35, 27, 55, 10, 28, 8, 20, 2, 14, 9, 16};

int my_rand(int min, int max) { return rand() % (max + 1 - min) + min; }

int comp(const void *elem1, const void *elem2) {
/*    int f = *((int *)elem1)->ms;
    int s = *((int *)elem2)->ms;
    if (f > s) return 1;
    if (f < s) return -1; */

    struct Car a = (struct Car *)elem1;
    struct Car b = (struct Car *)elem2;
    return (a->ms - b->ms);
}

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

    struct Car cars[TABLE_SIZE];

    int i = 0;
    for (; i < TABLE_SIZE; i++) {
        int ra = my_rand(minimum, maximum);
//        times[i] = ra;
        struct Car car;
        car.name = f1[i];
        car.ms = ra;
        cars[i] = car;       
    }

    qsort(cars, sizeof(cars) / sizeof(*cars), sizeof(*cars), comp);

    for (i = 0; i < TABLE_SIZE; i++) {
        printf("%d\n", times[i]);
    }

    exit(0);
}
