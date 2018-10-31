#include <stdio.h>
#include <stdlib.h>
#include "car.h"
#include "random.h"
#include "timeUnit.h"

#define TABLE_SIZE 20

int f1[TABLE_SIZE] = {44, 77, 5, 7, 3, 33, 11, 31, 18, 35, 27, 55, 10, 28, 8, 20, 2, 14, 9, 16};

int comp(const void *elem1, const void *elem2) {
    struct Car *a = (struct Car *)elem1;
    struct Car *b = (struct Car *)elem2;
    return a->ms - b->ms;
}

int main(int argc, char *argv[]) {
    struct TimeUnit min = new_time_unit(0, 30, 0);
    struct TimeUnit max = new_time_unit(0, 45, 0);

    int minimum = to_ms(min);
    int maximum = to_ms(max);

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

    qsort(cars, TABLE_SIZE, sizeof(*cars), comp);

    for (i = 0; i < TABLE_SIZE; i++) {
        struct TimeUnit time = to_time_unit(cars[i].ms);
        printf("car %-2d : %d''%02d'%03d\n", cars[i].name, time.m, time.s, time.ms);
    }

    exit(0);
}
