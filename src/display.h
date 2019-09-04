#pragma once

#include <semaphore.h>
#include "struct.h"
#include "curses.h"
#include "var.h"

void display(sem_t *sem, SharedStruct *data, Options options);
