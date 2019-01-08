#ifndef DISPLAY_H
#define DISPLAY_H

#include <semaphore.h>
#include "carstruct.h"
#include "sharedstruct.h"

void display(SharedStruct *shared_struct);

void init_step(SharedStruct *shared_struct, int step);

void display_step(SharedStruct *shared_struct, int step_index);

int done(Car *cars);

#endif //DISPLAY_H
