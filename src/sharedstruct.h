#ifndef STRUCT_H
#define STRUCT_H

#include "carstruct.h"
#include "options.h"
#include <semaphore.h>

// struct containing all the informations shared beetween cars and the main process
typedef struct SharedStruct{
    Car car_structs[NUMBER_OF_CARS];

    sem_t *sem;

    int step;
} SharedStruct;

#endif //STRUCT_H
