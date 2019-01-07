#ifndef CARSTRUCT_H
#define CARSTRUCT_H

#include "racestep.h"

// struct containing a car name and a race step struct for each step
typedef struct Car {
    char *name;

    RaceStep race_steps[7];
} Car;

#endif //CARSTRUCT_H
