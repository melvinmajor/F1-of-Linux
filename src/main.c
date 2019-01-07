#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "display.h"
#include "car.h"
#include "carstruct.h"
#include <semaphore.h>
#include "options.h"
#include "sharedstruct.h"
#include <stdlib.h>

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
    sem_t *sem = (sem_t *) create_shared_memory(sizeof(sem_t));
    sem_init(sem, 1, init_value);
    return sem;
}

int main(int argc, char *argv[]) {
    int lap_length = 6000;

    if(argc == 2){
        int val = atoi(argv[1]);
        if (val == 0) {
            fprintf(stderr, "Lap length must be a non zero integer\n");
            exit(1);
        }
        else if (val <= 7500 && val >= 4500) {
            lap_length = val;
        }
        else {
            fprintf(stderr, "Lap length must be beetween 4500 and 7500 meters\n");
            exit(1);
        }
    } else {
        printf("No arguments provided, defaulting to %d meters per lap\n", lap_length);
    }

    char *ALL_CAR_NAMES[20] = {"44", "77", "5", "7", "3", "33", "11", "31", "18", "35",
                               "27", "55", "10", "28", "8", "20", "2", "14", "9", "16"};

    // only used for debugging
    if (NUMBER_OF_CARS != 20) {
        fprintf(stderr, "Not yet implemented %d\n", NUMBER_OF_CARS);
        exit(1);
    }

    char *CAR_NAMES[NUMBER_OF_CARS];
    for (int i = 0; i < NUMBER_OF_CARS; i++) {
        CAR_NAMES[i] = ALL_CAR_NAMES[i];
    }

    pid_t pid = 0;
    int car_index = 0;


    size_t shared_struct_size = sizeof(SharedStruct);
    SharedStruct *shared_struct = (SharedStruct *) create_shared_memory(shared_struct_size);

    sem_t *sem = init_shared_sem(0);

    shared_struct->sem = sem;
    shared_struct->step = -1;

    for (int i = 0; i < NUMBER_OF_CARS; i++) {
        shared_struct->car_structs[i].name = CAR_NAMES[i];

        for (int j = 0; j < 7; ++j) {
            RaceStep race_step = {
                    .lap = 0,
                    .stand = 0,
                    .withdrawal = 0,
                    .done = 0,
                    .allowed = 0
            };
            shared_struct->car_structs[i].race_steps[j] = race_step;
        }
    }

    for (int i = 0; i < NUMBER_OF_CARS; i++) {
        car_index = i;
        pid = fork();
        if (pid == 0) break;
    }

    if (pid < 0) {
        fprintf(stderr, "An error occurred while forking: %d\n", pid);
        exit(1);
    } else if (pid == 0) {
        car(shared_struct, car_index, lap_length);
        exit(0);
    } else {
        display(shared_struct);
        for (int i = 0; i < NUMBER_OF_CARS; ++i) wait(NULL);
        munmap(shared_struct, shared_struct_size);
        munmap(sem, sizeof(sem_t));
        exit(0);
    }

}
