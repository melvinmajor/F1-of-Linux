#include "car.h"
#include "random.h"
#include "step.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// #define DEBUG

#ifdef DEBUG
#define DIVIDER 100000
#endif // DEBUG

#ifndef DEBUG
#define DIVIDER 1000
#endif // DEBUG

// the average time of a sector
int average_time;

int variance;

// function to sleep during x ms
void sleep_ms(int ms) {
    struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = ms * 1000000};
    nanosleep(&sleep_time, NULL);
}

// function called after the fork, once for each car
void car(SharedStruct *shared_struct, int index) {
    init_rand((unsigned int)getpid());

    average_time = 40000;
    variance = average_time * 10 / 100;

    step(shared_struct, index, P1, minutes(90), 0);
    step(shared_struct, index, P2, minutes(90), 0);
    step(shared_struct, index, P3, minutes(60), 0);

    step(shared_struct, index, Q1, minutes(18), 0);
    step(shared_struct, index, Q2, minutes(15), 0);
    step(shared_struct, index, Q3, minutes(12), 0);

    // TODO
    int lap_number = 15;
    step(shared_struct, index, RACE, minutes(90), lap_number);
    exit(0);
}

// function used to generate random times and sleep depending on the value
// the values are also assigned inside the race_step struct
void generate_lap(RaceStep *race_step, int lap) {
    race_step->stand = 0;
    for (int i = 0; i < 3; ++i) {
        int rand = bounded_rand(average_time - variance, average_time + variance);
        sleep_ms(rand / DIVIDER);

        if (i == 2 && proba(1, 100)) {
            int time_at_stand = bounded_rand(19000, 21000);
            sleep_ms(time_at_stand / DIVIDER);
            rand += time_at_stand;
            race_step->stand = 1;
        }

        if (lap != 0 && proba(1, 400)) {
            race_step->withdrawal = 1;
            break;
        }

        race_step->time[lap][i] = rand;
    }
}

// function called once for each step of the formula 1 weekend
void step(SharedStruct *shared_struct, int car_index, int step_index, TimeUnit min, int lap_number) {
    while (shared_struct->step != step_index)
        ;
    sem_wait(shared_struct->sem);

    Car *car = &shared_struct->car_structs[car_index];
    RaceStep *race_step = &car->race_steps[step_index];

    if (!race_step->allowed) {
        race_step->done = 1;
        return;
    }

    int total_time = to_ms(min);

    int current_time = 0;
    int lap = 0;

    while (1) {
        generate_lap(race_step, lap);

        if (race_step->withdrawal)
            break;

        int sum = 0;
        sum += race_step->time[lap][0];
        sum += race_step->time[lap][1];
        sum += race_step->time[lap][2];

        if ((step_index != RACE && current_time + sum > total_time) || (step_index == RACE && lap == lap_number)) {
            break;
        } else {
            current_time += sum;
            race_step->lap = lap++;
        }
    }

    race_step->done = 1;
}
