#include "child.h"
#include "random.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define divider 100

int time_passed = 0;
int next_pit_lap = 0;
int current_lap = 1;
Options options;
SharedStruct *data;
int min_sec;
int max_sec;
int chance_arg;

int sector() { return next_random(min_sec, max_sec); }

int pit_duration() { return next_random(20000, 25000); }

int pit_lap_rand() { return next_random(15, 25); }

int out_rand() {
    if (data->lap == 1)
        return 0;
    int out = chance(chance_arg);
    if (out) {
        data->out = data->done = 1;
    }

    return out;
}

void set_next_pit() { next_pit_lap = pit_lap_rand() + data->lap; }

// function to sleep during x ms
void sleep_ms(int ms) {
    struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = ms * 1000000};
    nanosleep(&sleep_time, NULL);
}

int step_done() {
    if (options.choosen_step == RACE) {
        return current_lap == options.number_of_laps;
    } else {
        return time_passed >= options.total_time;
    }
}

void child(sem_t *sem, SharedStruct *d, Options o, int position) {
    options = o;
    data = d;

    int middle = 5500 * options.lap_length;
    min_sec = middle - 0.125 * middle;
    max_sec = middle + 0.125 * middle;

    chance_arg = 10000 / options.lap_length;

    init_random((unsigned int)getpid());

    if (options.choosen_step == RACE) {
        sleep_ms(position * 100);
    }

    set_next_pit();
    while (!step_done()) {
        int sleep_time;
        sem_wait(sem);
        data->s1 = sector();
        data->sector = 1;
        if (data->best_s1 == 0 || data->best_s1 > data->s1) {
            data->best_s1 = data->s1;
        }

        if (out_rand()) {
            sem_post(sem);
            exit(0);
        }

        sleep_time = data->s1 / 100;

        sem_post(sem);
        sleep_ms(sleep_time);

        sem_wait(sem);
        data->s2 = sector();
        data->sector = 2;
        if (data->best_s2 == 0 || data->best_s2 > data->s2) {
            data->best_s2 = data->s2;
        }

        if (out_rand()) {
            sem_post(sem);
            exit(0);
        }

        sleep_time = data->s2 / 100;
        sem_post(sem);
        sleep_ms(sleep_time);

        sem_wait(sem);
        data->s3 = sector();
        // if we should make a pit stop now
        if (data->lap == next_pit_lap) {
            data->s3 += pit_duration();
            data->pit++;
            set_next_pit();
        }
        data->sector = 3;

        if (out_rand()) {
            sem_post(sem);
            exit(0);
        }

        sleep_time = data->s3 / 100;
        if (data->best_s3 == 0 || data->best_s3 > data->s3) {
            data->best_s3 = data->s3;
        }
        int lap_time = data->s1 + data->s2 + data->s3;
        time_passed += lap_time;
        if (data->best_lap_time == 0 || data->best_lap_time > lap_time) {
            data->best_lap_time = lap_time;
        }

        data->lap++;
        current_lap = data->lap;
        sem_post(sem);
        sleep_ms(sleep_time);
    }

    sem_wait(sem);
    data->done = 1;
    sem_post(sem);
}
