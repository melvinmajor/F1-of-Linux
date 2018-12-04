#include "display.h"
#include "lapInfo.h"
#include "random.h"
#include "sort.h"
#include "timeUnit.h"
#include "util.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define CAR_COUNT 20

#define SLEEP_BTW_LAP 10

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RST "\x1B[0m"

char *CAR_NAMES[CAR_COUNT] = {"44", "77", "5",  "7",  "3", "33", "11", "31", "18", "35",
                              "27", "55", "10", "28", "8", "20", "2",  "14", "9",  "16"};

void child(sem_t *sem, sem_t *sem2, struct LapInfo *lap_info) {
    srand(getpid());

    int i, lap;
    int total_time;

    int max_time = to_ms((struct TimeUnit){.m = 90, .s = 0, .ms = 0});
    int base_time = to_ms((struct TimeUnit){.m = 0, .s = 40, .ms = 0});
    int deviation = to_ms((struct TimeUnit){.m = 0, .s = 5, .ms = 0});

    for (int seance = 0; seance < 3; seance++) {
        sem_wait(sem);
        total_time = 0;
        if (seance == 3)
            max_time = to_ms((struct TimeUnit){.m = 60, .s = 0, .ms = 0});
        for (lap = 0; total_time < max_time; lap++) {

            sem_post(sem2);
            sem_wait(sem);

            lap_info->current_lap = lap;

            if (lap_info->best_time != 0 || lap_info->best_time > lap) {

                lap_info->best_time = lap;
            }

            for (i = 0; i < 3; i++) {
                lap_info->current_sector = i;
                int random_sector = bounded_rand(base_time - deviation, base_time + deviation + 1);
                lap_info->current_time[i] = random_sector;
                total_time += random_sector;
            }

            sleep_ms(SLEEP_BTW_LAP);
        }

        lap_info->race_done = 1;
        sem_post(sem2);
    }
}

void display(sem_t *sem, sem_t *sem2, struct LapInfo *lap_infos) {
    /*------
    Practice
    --------*/
    for (int i = 1; i <= 3; i++) {
        reset(lap_infos, CAR_COUNT);
        printf("%s%d\n", "START practice", i);
        signal_n_times(sem, cars_still_racing(lap_infos, CAR_COUNT));

        for (int i = 0; !done(lap_infos, CAR_COUNT); i++) {
            wait_n_times(sem2, cars_still_racing(lap_infos, CAR_COUNT));

            if (i % 5 == 0) {
                struct LapInfo *sorted = sort_by_time(lap_infos, CAR_COUNT);

                print_practice(sorted, CAR_COUNT);
                printf("%d\n", cars_still_racing(lap_infos, CAR_COUNT));
            }

            

            signal_n_times(sem, cars_still_racing(lap_infos, CAR_COUNT));
        }

        printf("%s%d\n\n", "END practice", i);
        sleep_ms(750);
    }

    /*----
    Qualif
    ------*/
    reset(lap_infos, CAR_COUNT);
    // TODO
}

int main(void) {
    sem_t *sem = init_shared_sem(0);
    sem_t *sem2 = init_shared_sem(0);

    size_t lap_infos_size = sizeof(struct LapInfo) * CAR_COUNT;
    struct LapInfo *lap_infos_shared = (struct LapInfo *)create_shared_memory(lap_infos_size);

    int i;
    for (int i = 0; i < CAR_COUNT; i++) {
        struct LapInfo inf;
        inf.race_done = 0;
        lap_infos_shared[i] = inf;
        inf.car_name = CAR_NAMES[i];
    }

    pid_t pid;
    int car_index;

    for (i = 0; i < CAR_COUNT; i++) {
        car_index = i;
        pid = fork();
        if (pid == 0)
            break;
    }

    if (pid < 0) {
        fprintf(stderr, "An error occured while forking: %d\n", pid);
        exit(1);
    } else if (pid == 0) {
        child(sem, sem2, &lap_infos_shared[car_index]);
        exit(0);
    } else {
        display(sem, sem2, lap_infos_shared);
    }

    for (i = 0; i < CAR_COUNT; i++)
        wait(NULL);

    exit(0);
}
