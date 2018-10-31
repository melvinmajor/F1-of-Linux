#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "random.h"

#define CAR_COUNT 20

int f1[CAR_COUNT] = {44, 77, 5, 7, 3, 33, 11, 31, 18, 35, 27, 55, 10, 28, 8, 20, 2, 14, 9, 16};

void *create_shared_memory(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_SHARED;
    return mmap(NULL, size, prot, flags, 0, 0);
}

int main(void) {
    int i, n;

    sem_t *sem;
    sem = (sem_t *)create_shared_memory(sizeof(sem));
    sem_init(sem, 1, 0);

    pid_t pid;
    int car_index;

    for (i = 0, n = CAR_COUNT; i < n; i++) {
        car_index = i;
        pid = fork();
        if (pid == 0) {
            break;
        }
    }

    if (pid < 0) {
        fprintf(stderr, "An error occured while forking: %d\n", pid);
    } else if (pid == 0) {
        sem_wait(sem);
        sem_post(sem);
        // start
        srand(getpid());
        printf("Starting race\n");
        int total_time = 0;
        int max = 3600000;
        int lap = 0;
        while (total_time < max) {
            for (i = 0; i < 3; i++) {
                int base_time = 40000;
                int deviation = 1000;
                int random_sector = bounded_rand(base_time - deviation, base_time + deviation);
                total_time += random_sector;
                printf("[%d] lap: %d, sector: %d, time: %d\n", car_index, lap, i, random_sector);
            }
            lap++;
        }

        printf("Race done\n");
    } else {
        sem_post(sem);

        //TODO display here

        // wait for children to exit
        for (i = 0; i < CAR_COUNT; i++) {
            wait(NULL);
        }
    }
    exit(0);
}
