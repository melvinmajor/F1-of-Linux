#include "car.h"
#include "random.h"
#include "timeUnit.h"
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 20

int f1[TABLE_SIZE] = {44, 77, 5, 7, 3, 33, 11, 31, 18, 35, 27, 55, 10, 28, 8, 20, 2, 14, 9, 16};

int comp(const void *elem1, const void *elem2) {
    struct Car *a = (struct Car *)elem1;
    struct Car *b = (struct Car *)elem2;
    return a->ms - b->ms;
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

    int minimum = toMs(min);
    int maximum = toMs(max);

    struct Car cars[TABLE_SIZE];

    int i = 0;
    init_rand();
    for (; i < TABLE_SIZE; i++) {
        int ra = bounded_rand(minimum, maximum);
        struct Car car;
        car.name = f1[i];
        car.ms = ra;
        cars[i] = car;
    }

    qsort(cars, sizeof(cars) / sizeof(*cars), sizeof(*cars), comp);

    for (i = 0; i < TABLE_SIZE; i++) {
        struct TimeUnit time = toTimeUnit(cars[i].ms);
        printf("car %-3d: %d''%d'%d\n", cars[i].name, time.m, time.s, time.ms);
    }

    exit(0);
}
