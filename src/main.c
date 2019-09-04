#include "child.h"
#include "display.h"
#include "struct.h"
#include "time.h"
#include "var.h"
#include <errno.h>
#include <getopt.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int pid;
    int i;
    SharedStruct *data;
    int valid = 0;
    Options options;
    // default value
    options.lap_length = 7.0;

    static struct option long_options[] = {
        {"length", required_argument, NULL, 'l'},
        {"step", required_argument, NULL, 's'},
        {NULL, 0, NULL, 0}};

    char ch;
    float lap_length_maybe;
    while ((ch = getopt_long(argc, argv, "l:s:", long_options, NULL)) != -1) {
        switch (ch) {
        case 'l':
            lap_length_maybe = atof(optarg);
            if (lap_length_maybe < 3 || lap_length_maybe > 10) {
                printf("Invalid lap length\n");
                exit(1);
            }
            options.lap_length = lap_length_maybe;
            break;
        case 's':
            valid = 1;
            options.participant_count = 20;
            if (strcasecmp(optarg, "P1") == 0) {
                options.choosen_step = P1;
                options.step_name = "P1";
                options.total_time = minute_to_ms(90);
            } else if (strcasecmp(optarg, "P2") == 0) {
                options.choosen_step = P2;
                options.step_name = "P2";
                options.total_time = minute_to_ms(90);
            } else if (strcasecmp(optarg, "P3") == 0) {
                options.choosen_step = P3;
                options.step_name = "P3";
                options.total_time = minute_to_ms(60);
            } else if (strcasecmp(optarg, "Q1") == 0) {
                options.choosen_step = Q1;
                options.step_name = "Q1";
                options.total_time = minute_to_ms(18);
            } else if (strcasecmp(optarg, "Q2") == 0) {
                options.choosen_step = Q2;
                options.step_name = "Q2";
                options.total_time = minute_to_ms(15);
                options.participant_count = 15;
            } else if (strcasecmp(optarg, "Q3") == 0) {
                options.choosen_step = Q3;
                options.step_name = "Q3";
                options.total_time = minute_to_ms(12);
                options.participant_count = 10;
            } else if (strcasecmp(optarg, "race") == 0) {
                options.choosen_step = RACE;
                options.step_name = "RACE";
                options.total_time = minute_to_ms(120);
            } else {
                printf("invalid step, must be: P1|P2|P3|Q1|Q2|Q3|RACE\n");
                exit(1);
            }

            break;
        }
    }

    options.number_of_laps = 300 / options.lap_length;

    int custom = 0;
    int ids[options.participant_count];
    // verify if we need to load previous ranking
    if (options.choosen_step > 3) {
        custom = 1;
        // previous step
        char *file_to_read;
        if (options.choosen_step == Q2)
            file_to_read = "Q1";
        else if (options.choosen_step == Q3)
            file_to_read = "Q2";
        else if (options.choosen_step == RACE)
            file_to_read = "Q3";

        FILE *f = fopen(file_to_read, "r");
        if (f == NULL) {
            printf("Previous rankings missing. please run the step before this "
                   "one\n");
            exit(1);
        }
        char *line = NULL;
        size_t len = 0;
        int count = 0;
        while (getline(&line, &len, f) != 1) {
            if (count == options.participant_count ||
                (options.choosen_step == RACE && count == 10))
                break;
            int id = atoi(line);
            ids[count] = id;
            count++;
        }

        fclose(f);

        // we need to get the last 5 from Q2
        if (options.choosen_step == RACE) {
            f = fopen("Q2", "r");
            if (f == NULL) {
                printf("A file was deleted ?\n");
                exit(1);
            }
            line = NULL;
            len = 0;

            count = 0;
            while (getline(&line, &len, f) != 1) {
                if (count == 15)
                    break;
                if (count < 10) {
                    count++;
                    continue;
                }
                int id = atoi(line);
                ids[count] = id;
                count++;
            }
            fclose(f);
        }

        // we need to get the last 5 from Q1
        if (options.choosen_step == RACE) {
            f = fopen("Q1", "r");
            if (f == NULL) {
                printf("A file was deleted ?\n");
                exit(1);
            }
            line = NULL;
            len = 0;

            count = 0;
            while (getline(&line, &len, f) != 1) {
                if (count == 20)
                    break;
                if (count < 15) {
                    count++;
                    continue;
                }
                int id = atoi(line);
                ids[count] = id;
                printf("%d\n", id);
                count++;
            }
            fclose(f);
        }
    }

    if (!valid) {
        printf("You need to choose a race step with --step\n");
        exit(1);
    }

    int struct_shm_id =
        shmget(IPC_PRIVATE, sizeof(SharedStruct) * options.participant_count,
               0600 | IPC_CREAT);
    if (struct_shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    data = shmat(struct_shm_id, NULL, 0);
    if (data == (void *)(-1)) {
        perror("shmat");
        exit(1);
    }

    SharedStruct blank = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    for (int i = 0; i < options.participant_count; i++) {
        data[i] = blank;
        if (custom)
            data[i].id = ids[i];
        else
            data[i].id = i;
    }

    int sem_shm_id = shmget(IPC_PRIVATE, sizeof(sem_t), 0600 | IPC_CREAT);
    if (sem_shm_id == -1) {
        perror("shmget");
        exit(1);
    }
    sem_t *sem = shmat(sem_shm_id, NULL, 0);
    if (sem == (void *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // init semaphore with 1 as initial value
    sem_init(sem, 1, 1);

    for (i = 0; i < options.participant_count; i++) {
        pid = fork();
        if (pid == 0)
            break;
    }

    switch (pid) {
    case -1:
        fprintf(stderr, "fork failed ?!");
        exit(-1);
    case 0:
        child(sem, &data[i], options, i);
        exit(0);
    default:
        display(sem, data, options);

        // wait for children process to exit
        for (int i = 0; i < options.participant_count; i++) {
            wait(NULL);
        }

        shmdt(data);
        shmctl(struct_shm_id, IPC_RMID, NULL);

        sem_destroy(sem);
        shmdt(sem);
        shmctl(sem_shm_id, IPC_RMID, NULL);

        exit(0);
    }
}
