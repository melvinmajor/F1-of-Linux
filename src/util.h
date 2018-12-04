#include <time.h>
#include <sys/types.h>
#include <semaphore.h>
#include "lapInfo.h"

void sleep_ms(int ms);

void *create_shared_memory(size_t size);

sem_t *init_shared_sem(int init_value);

void wait_n_times(sem_t *sem, int n);

void signal_n_times(sem_t *sem, int n);

int cars_still_racing(struct LapInfo *lap_infos, int car_number);

int done(struct LapInfo *lap_infos, int car_number);

unsigned int lap_time(struct LapInfo *lap_info);

void reset(struct LapInfo *lap_infos, int length);