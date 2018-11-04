#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "random.h"
#include "timeUnit.h"
#include "util.h"

#define CAR_COUNT 20

char *f1[CAR_COUNT] = {"44", "77", "5",  "7",  "3", "33", "11", "31", "18", "35",
                       "27", "55", "10", "28", "8", "20", "2",  "14", "9",  "16"};

struct LapInfo {
    // how many lines ??
    int time[1500][3];
    int current_lap;
    int current_sector;
    int in_stand;
    int abandon;
    int race_done;
};

void to_string(int ms, char *str) {
    struct TimeUnit time = to_time_unit(ms);
    sprintf(str, "%d'%d", time.s, time.ms);
}

void child(sem_t *sem, struct LapInfo *lap_info) {
    sem_wait(sem);
    sem_post(sem);

    srand(getpid());

    int i, lap;
    int total_time = 0;

    int max_time = to_ms((struct TimeUnit){.m = 90, .s = 0, .ms = 0});
    int base_time = to_ms((struct TimeUnit){.m = 0, .s = 40, .ms = 0});
    int deviation = to_ms((struct TimeUnit){.m = 0, .s = 5, .ms = 0});

    for (lap = 0; total_time < max_time; lap++) {
        lap_info->current_lap = lap;
        for (i = 0; i < 3; i++) {
            lap_info->current_sector = i;
            int random_sector = bounded_rand(base_time - deviation, base_time + deviation);
            lap_info->time[lap][i] = random_sector;
            total_time += random_sector;
        }

        sleep_ms(100);
    }
    lap_info->race_done = 1;
}

int done(const struct LapInfo *lap_infos) {
    int i;
    for (i = 0; i < CAR_COUNT; i++)
        if (lap_infos[i].race_done) return 1;
    return 0;
}

int started(const struct LapInfo *lap_infos) {
    int i;
    for (i = 0; i < CAR_COUNT; i++)
        if (lap_infos[i].current_lap < 2) return 0;
    return 1;
}

void display(sem_t *sem, struct LapInfo *lap_infos) {
    sem_post(sem);

    int s = 0;
    while (!done(lap_infos)) {
        sleep_ms(500);

        if (!s && !started(lap_infos))
            continue;
        else
            s = 1;

        printf("\n%4s|%-6s|%-6s|%-6s\n", "NAME", "S1", "S2", "S3");
        printf("----|------|------|------\n");
        for (int i = 0; i < CAR_COUNT; i++) {
            int previous_lap = lap_infos[i].current_lap - 1;

            char a[7];
            to_string(lap_infos[i].time[previous_lap][0], a);

            char b[7];
            to_string(lap_infos[i].time[previous_lap][1], b);

            char c[7];
            to_string(lap_infos[i].time[previous_lap][2], c);

            printf("%4s|%-6s|%-6s|%-6s  lap:%d\n", f1[i], a, b, c, previous_lap);
        }
    }
}

int main(void) {
    sem_t *sem = init_shared_sem(0);

    size_t lap_infos_size = sizeof(struct LapInfo) * CAR_COUNT;
    struct LapInfo *lap_infos_shared = (struct LapInfo *)create_shared_memory(lap_infos_size);

    int i;
    for (int i = 0; i < CAR_COUNT; i++) {
        struct LapInfo inf;
        inf.race_done = 0;
        lap_infos_shared[i] = inf;
    }

    pid_t pid;
    int car_index;

    for (i = 0; i < CAR_COUNT; i++) {
        car_index = i;
        pid = fork();
        if (pid == 0) break;
    }

    if (pid < 0) {
        fprintf(stderr, "An error occured while forking: %d\n", pid);
        exit(1);
    } else if (pid == 0) {
        child(sem, &lap_infos_shared[car_index]);
        exit(0);
    } else {
        display(sem, lap_infos_shared);
    }

    for (i = 0; i < CAR_COUNT; i++) wait(NULL);

    exit(0);
}
