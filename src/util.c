#include "lapInfo.h"
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>

void sleep_ms(int ms) {
    struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = ms * 1000000};
    nanosleep(&sleep_time, NULL);
}

void *create_shared_memory(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_SHARED;
    return mmap(NULL, size, prot, flags, 0, 0);
}

sem_t *init_shared_sem(int init_value) {
    sem_t *sem = (sem_t *)create_shared_memory(sizeof(sem_t));
    sem_init(sem, 1, init_value);
    return sem;
}

void signal_n_times(sem_t *sem, int n) {
    for (int i = 0; i < n; i++)
        sem_post(sem);
}

void wait_n_times(sem_t *sem, int n) {
    for (int i = 0; i < n; i++)
        sem_wait(sem);
}

int cars_still_racing(struct LapInfo *lap_infos, int car_number) {
    int count = 0;
    for (int i = 0; i < car_number; i++) {
        if (lap_infos[i].race_done == 0)
            count++;
    }
    return count;
}

int done(struct LapInfo *lap_infos, int car_number) { return cars_still_racing(lap_infos, car_number) == 0; }

unsigned int lap_time(struct LapInfo *lap_info) {
    unsigned int sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += lap_info->current_time[i];
    }
    return sum;
}

void reset(struct LapInfo *lap_infos, int length) {
    for (int i = 0; i < length; i++) {
        lap_infos[i].current_time[0] = 0;
        lap_infos[i].current_time[1] = 0;
        lap_infos[i].current_time[2] = 0;

        lap_infos[i].current_lap = 0;
        lap_infos[i].current_sector = 0;
        lap_infos[i].in_stand = 0;
        lap_infos[i].abandon = 0;
        lap_infos[i].race_done = 0;
        lap_infos[i].best_time = 0;
    }
}
