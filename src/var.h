#pragma once

#define CAR_COUNT 20
extern char const *const CAR_NAMES[];

enum step {
    P1, P2, P3, Q1, Q2, Q3, RACE
};

typedef struct Options {
    enum step choosen_step;
    char *step_name;
    int total_time;
    int number_of_laps;
    float lap_length;
    int participant_count;
} Options;
