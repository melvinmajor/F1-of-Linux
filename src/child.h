#pragma once

#include <semaphore.h>
#include "struct.h"
#include "var.h"

void child(sem_t *sem, SharedStruct *data, Options options, int position);
