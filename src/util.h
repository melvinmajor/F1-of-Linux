#ifndef UTIL_H
#define UTIL_H

#include "carstruct.h"
#include <sys/types.h>
#include <semaphore.h>

// TODO rename
struct e {
    int car_index;
    int value;
};

void print_car_names(char **names, int length);

int number_of_car_allowed(Car *cars, int step);

void sort_car_by_time(struct e *result, Car *car, int step);

void sort_car_by_lap(struct e *result, Car *car, int step);

int min_from_array(const int *array, int size);

void *create_shared_memory(size_t size);

sem_t *init_shared_sem(unsigned int init_value);

#endif //UTIL_H
