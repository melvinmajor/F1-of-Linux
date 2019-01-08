#include "display.h"
#include "options.h"
#include "step.h"
#include "timeunit.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// contains index of qualified cars
int qualified[NUMBER_OF_CARS];
int number_of_cars_allowed;

int current_step;
char step_name[5];

// function called from the main method as the parent process
void display(SharedStruct *shared_struct) {
    init_step(shared_struct, P1);
    display_step(shared_struct, P1);

    init_step(shared_struct, P2);
    display_step(shared_struct, P2);

    init_step(shared_struct, P3);
    display_step(shared_struct, P3);

    init_step(shared_struct, Q1);
    display_step(shared_struct, Q1);

    init_step(shared_struct, Q2);
    display_step(shared_struct, Q2);

    init_step(shared_struct, Q3);
    display_step(shared_struct, Q3);

    init_step(shared_struct, RACE);
    display_step(shared_struct, RACE);
}

// set the current step name, allow the cars for the next step depending on their positions
// and the signal them with a semaphore
void init_step(SharedStruct *shared_struct, int step) {
    current_step = step;

    // they're all qualified for P1
    if (step == P1) {
        for (int i = 0; i < NUMBER_OF_CARS; ++i) {
            qualified[i] = i;
        }
    }

    if (step == P1)
        strcpy(step_name, "P1");
    else if (step == P2)
        strcpy(step_name, "P2");
    else if (step == P3)
        strcpy(step_name, "P3");
    else if (step == Q1)
        strcpy(step_name, "Q1");
    else if (step == Q2)
        strcpy(step_name, "Q2");
    else if (step == Q3)
        strcpy(step_name, "Q3");
    else if (step == RACE)
        strcpy(step_name, "race");

    if (step == Q2)
        number_of_cars_allowed = 15;
    else if (step == Q3 || step == RACE)
        number_of_cars_allowed = 10;
    else
        number_of_cars_allowed = 20;

    char *names[number_of_cars_allowed];

    for (int i = 0; i < number_of_cars_allowed; ++i) {
        int car_index = qualified[i];
        names[i] = shared_struct->car_structs[car_index].name;
        shared_struct->car_structs[car_index].race_steps[step].allowed = 1;
    }

    printf("\n\nThe followings cars are qualified for the coming step:\n");
    print_car_names(names, number_of_cars_allowed);
    printf("\n");

    sleep(1);

    shared_struct->step = step;

    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        sem_post(shared_struct->sem);
    }
}

// function used to display a step
void display_step(SharedStruct *shared_struct, int step_index) {
    printf("Starting %s---------------------------\n\n", step_name);

    while (!done(shared_struct->car_structs)) {

        sleep(1);

        struct e sorted[NUMBER_OF_CARS];
        if (current_step == RACE) {
            sort_car_by_lap(sorted, shared_struct->car_structs, current_step);
        } else {
            sort_car_by_time(sorted, shared_struct->car_structs, current_step);
        }

        if (current_step == RACE) {
            printf("\nname out current lap\n");
            printf("---- --- -----------\n");
        } else {
            printf("\nname out current lap best lap time\n");
            printf("---- --- ----------- ------------\n");
        }

        for (int i = 0; i < NUMBER_OF_CARS; ++i) {
            Car *car = &shared_struct->car_structs[sorted[i].car_index];
            RaceStep *race_step = &car->race_steps[step_index];
            if (!race_step->allowed)
                continue;

            char time[25];
            to_string(sorted[i].value, time);

            char out[4];
            if (race_step->out)
                strcpy(out, "yes");
            else
                strcpy(out, "no");

            if (current_step == RACE) {
                printf("%2s   %-3s %-2d\n", car->name, out, race_step->lap);
            } else {
                printf("%2s   %-3s %-2d          %s\n", car->name, out, race_step->lap, time);
            }
        }
    }

    printf("%s done------------------------------\n\n", step_name);
    printf("Summary\n");

    struct e sorted[NUMBER_OF_CARS];
    if (current_step == RACE) {
        sort_car_by_lap(sorted, shared_struct->car_structs, current_step);
    } else {
        sort_car_by_time(sorted, shared_struct->car_structs, current_step);
    }

    if (current_step == RACE) {
        printf("\nname out current lap\n");
        printf("---- --- -----------\n");
    } else {
        printf("\nname out current lap best lap time\n");
        printf("---- --- ----------- ------------\n");
    }

    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        Car *car = &shared_struct->car_structs[sorted[i].car_index];
        RaceStep *race_step = &car->race_steps[step_index];
        if (!race_step->allowed)
            continue;

        char time[25];
        to_string(sorted[i].value, time);

        char out[4];
        if (race_step->out)
            strcpy(out, "yes");
        else
            strcpy(out, "no");

        if (current_step == RACE) {
            printf("%2s   %-3s %-2d\n", car->name, out, race_step->lap);
        } else {
            printf("%2s   %-3s %-2d          %s\n", car->name, out, race_step->lap, time);
        }
    }

    if (current_step == RACE) {

        for (int i = 0; i < NUMBER_OF_CARS; ++i) {
            Car *car = &shared_struct->car_structs[sorted[i].car_index];
            RaceStep *race_step = &car->race_steps[step_index];

            if (!race_step->allowed || race_step->out)
                continue;
            printf("\n\nThe winner of the race is %s !!!\n", car->name);
            break;
        }

    } else {
        sleep(2);
    }
}

// function returning 1 if the current step is done
int done(Car *cars) {
    int count = 0;
    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        Car *car = &cars[i];
        if (!car->race_steps[current_step].allowed)
            continue;
        if (!car->race_steps[current_step].done)
            ++count;
    }
    return count == 0;
}
