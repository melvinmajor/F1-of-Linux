#include "util.h"
#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// print car names assuming a name length is at max 2
void print_car_names(char **names, int length) {
    int size = 4 * length;
    char str[size];
    strcpy(str, "");
    for (int i = 0; i < length; ++i) {
        strcat(str, names[i]);
        if (i != length - 1)
            strcat(str, ", ");
    }
    printf("%s\n", str);
}

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

// utility function returning a pointer from shared memory
void *create_shared_memory(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_SHARED;
    void *ptr = mmap(NULL, size, prot, flags, 0, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Error mmap\n");
        exit(1);
    }
    return ptr;
}

// utility function returning a pointer to a semaphore in shared memory
sem_t *init_shared_sem(unsigned int init_value) {
    sem_t *sem = (sem_t *)create_shared_memory(sizeof(sem_t));
    sem_init(sem, 1, init_value);
    return sem;
}
