#ifndef DISPLAY_H
#define DISPLAY_H

#include <semaphore.h>
#include "carstruct.h"
#include "sharedstruct.h"

void display(SharedStruct *shared_struct);

int cars_running(Car *cars);

int done(Car *cars);

#endif //DISPLAY_H
