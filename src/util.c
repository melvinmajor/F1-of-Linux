#include "util.h"
#include "options.h"
#include <stdlib.h>

int comp(const void *a, const void *b) {
    int first = ((struct e *)a)->value;
    int second = ((struct e *)b)->value;

    if (first == second)
        return 0;
    else if (first == -1)
        return -1;
    else if (second == -1)
        return 1;
    else
        return first - second;
}

void sort_car_by_time(struct e *result, Car *car, int step) {
    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        Car *c = &car[i];
        RaceStep *race_step = &c->race_steps[step];
        if (!race_step->allowed) {
            result[i] = (struct e){.car_index = i, .value = -1};
            continue;
        }

        int lap_count = race_step->lap;
        int laps[lap_count];

        for (int j = 0; j < lap_count; ++j) {
            laps[j] = race_step->time[lap_count][0];
            laps[j] += race_step->time[lap_count][1];
            laps[j] += race_step->time[lap_count][2];
        }

        result[i] = (struct e){.car_index = i, .value = min_from_array(laps, lap_count)};
    }

    qsort(result, NUMBER_OF_CARS, sizeof(struct e), comp);
}

void sort_car_by_lap(struct e *result, Car *car, int step) {
    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        Car *c = &car[i];
        RaceStep *race_step = &c->race_steps[step];
        if (!race_step->allowed) {
            result[i] = (struct e){.car_index = i, .value = -1};
            continue;
        }

        int lap_count = race_step->lap;

        result[i] = (struct e){.car_index = i, .value = lap_count};
    }

    qsort(result, NUMBER_OF_CARS, sizeof(struct e), comp);
}

int min_from_array(const int *array, int size) {
    int min_value = -1;
    for (int i = 0; i < size; ++i) {
        if (array[i] < min_value || min_value == -1) {
            min_value = array[i];
        }
    }
    return min_value;
}
