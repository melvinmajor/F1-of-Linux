#include "car.h"
#include "carstruct.h"
#include "display.h"
#include "options.h"
#include "sharedstruct.h"
#include "util.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char *CAR_NAMES[NUMBER_OF_CARS] = {"44", "77", "5",  "7",  "3", "33", "11", "31", "18", "35",
                                       "27", "55", "10", "28", "8", "20", "2",  "14", "9",  "16"};

    pid_t pid = 0;
    int car_index = 0;

    size_t shared_struct_size = sizeof(SharedStruct);
    SharedStruct *shared_struct = (SharedStruct *)create_shared_memory(shared_struct_size);

    sem_t *sem = init_shared_sem(0);

    shared_struct->sem = sem;
    shared_struct->step = -1;

    for (int i = 0; i < NUMBER_OF_CARS; i++) {
        shared_struct->car_structs[i].name = CAR_NAMES[i];

        for (int j = 0; j < 7; ++j) {
            RaceStep race_step = {.lap = 0, .stand = 0, .withdrawal = 0, .done = 0, .allowed = 0};
            shared_struct->car_structs[i].race_steps[j] = race_step;
        }
    }

    for (int i = 0; i < NUMBER_OF_CARS; i++) {
        car_index = i;
        pid = fork();
        if (pid == 0)
            break;
    }

    if (pid < 0) {
        fprintf(stderr, "An error occurred while forking: %d\n", pid);
        exit(1);
    } else if (pid == 0) {
        car(shared_struct, car_index);
        exit(0);
    } else {
        display(shared_struct);
        for (int i = 0; i < NUMBER_OF_CARS; ++i)
            wait(NULL);
        munmap(shared_struct, shared_struct_size);
        munmap(sem, sizeof(sem_t));
        exit(0);
    }
}
