#ifndef UTIL_H
#define UTIL_H

#include "carstruct.h"

// TODO rename
struct e {
    int car_index;
    int value;
};

int number_of_car_allowed(Car *cars, int step);

void sort_car_by_time(struct e *result, Car *car, int step);

void sort_car_by_lap(struct e *result, Car *car, int step);

int min_from_array(const int *array, int size);

#endif //UTIL_H
