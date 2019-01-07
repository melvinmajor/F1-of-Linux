#ifndef CAR_H
#define CAR_H

#include "carstruct.h"
#include "timeunit.h"
#include "sharedstruct.h"


void car(SharedStruct *shared_struct, int index);

void step(SharedStruct *shared_struct, int car_index, int step_index, TimeUnit min, int lap_number);

#endif //CAR_H
