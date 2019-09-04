#include "display.h"
#include "../lib/fort.h"
#include "time.h"
#include "var.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

WINDOW *status_win;
Options options;
int done = 0;
SharedStruct copy[20];

int comparator(const void *a, const void *b) {
    const SharedStruct *pa = (SharedStruct *)a;
    const SharedStruct *pb = (SharedStruct *)b;
    if (options.choosen_step != RACE) {
        if (pa->best_lap_time < pb->best_lap_time)
            return -1;
        else if (pa->best_lap_time > pb->best_lap_time)
            return 1;
        else
            return 0;
    } else {
        if (pa->out == pb->out) {
            if (pa->lap < pb->lap)
                return 1;
            else if (pa->lap > pb->lap)
                return -1;
            else {
                if (pa->sector > pb->sector)
                    return 1;
                else if (pa->sector < pb->sector)
                    return -1;
                else
                    return 0;
            }
        } else {
            return pa->out - pb->out;
        }
    }
}

void print_table() {
    ft_table_t *table = ft_create_table();

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "POSITION", "NAME", "S1", "S2", "S3", "LAP", "OUT",
                "PIT", "DONE", "BEST LAP TIME");

    for (int i = 0; i < options.participant_count; i++) {
        SharedStruct current = copy[i];
        char s1_str[25];
        to_string(current.s1, s1_str);
        char s2_str[25];

        if (current.sector == 1)
            strcpy(s2_str, "");
        else
            to_string(current.s2, s2_str);

        char s3_str[25];
        if (current.sector < 3)
            strcpy(s3_str, "");
        else
            to_string(current.s3, s3_str);

        char lap_str[25];
        to_string(current.best_lap_time, lap_str);

        ft_printf_ln(table, "%d|%2s|%8s|%8s|%8s|%d|%d|%d|%d|%10s", i + 1,
                     CAR_NAMES[current.id], s1_str, s2_str, s3_str,
                     current.lap, current.out, current.pit, current.done,
                     lap_str);
    }

    const char *table_string = ft_to_string(table);
    mvprintw(0, 0, "%s\n", table_string);
    ft_destroy_table(table);
    refresh();
}

// return car index
int best_s1() {
    int best = 0;
    int index = 0;
    for (int i = 0; i < options.participant_count; i++) {
        if (best == 0 || copy[i].best_s1 < best) {
            best = copy[i].best_s1;
            index = i;
        }
    }
    return index;
}

// return car index
int best_s2() {
    int best = 0;
    int index = 0;
    for (int i = 0; i < options.participant_count; i++) {
        if (best == 0 || copy[i].best_s2 < best) {
            best = copy[i].best_s2;
            index = i;
        }
    }
    return index;
}

// return car index
int best_s3() {
    int best = 0;
    int index = 0;
    for (int i = 0; i < options.participant_count; i++) {
        if (best == 0 || copy[i].best_s3 < best) {
            best = copy[i].best_s3;
            index = i;
        }
    }
    return index;
}

void write_status() {
    mvwprintw(status_win, 0, 0, "%s circuit:%.3fkm\n", options.step_name,
              options.lap_length);
    if (done) {
        mvwprintw(status_win, 1, 0, "%s\n", "DONE");
    } else if (options.choosen_step == RACE) {
        mvwprintw(status_win, 1, 0, "%d %s\n",
                  options.number_of_laps - copy[0].lap, "laps remaining");
    }

    int s1 = best_s1();
    char s1_value[25];
    to_string(copy[s1].best_s1, s1_value);

    int s2 = best_s2();
    char s2_value[25];
    to_string(copy[s2].best_s2, s2_value);

    int s3 = best_s3();
    char s3_value[25];
    to_string(copy[s3].best_s3, s3_value);

    mvwprintw(status_win, 3, 0, "Best sectors\n");
    ft_table_t *table = ft_create_table();

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "SECTOR", "NAME", "VALUE");
    ft_printf_ln(table, "%s|%s|%s", "S1", CAR_NAMES[s1], s1_value);
    ft_printf_ln(table, "%s|%s|%s", "S2", CAR_NAMES[s2], s2_value);
    ft_printf_ln(table, "%s|%s|%s", "S3", CAR_NAMES[s3], s3_value);

    const char *table_string = ft_to_string(table);
    mvwprintw(status_win, 3, 0, "%s\n", table_string);
    ft_destroy_table(table);
    wrefresh(status_win);
    refresh();
}

int finished() {
    for (int i = 0; i < options.participant_count; ++i) {
        if (!copy[i].done) {
            return 0;
        }
    }
    return 1;
}

void save_ranking() {
    FILE *f = fopen(options.step_name, "w");
    if (f == NULL) {
        perror("open");
        exit(1);
    }
    for (int i = 0; i < options.participant_count; i++) {
        fprintf(f, "%d\n", copy[i].id);
    }
    if (fclose(f) != 0) {
        perror("fclose");
        exit(1);
    }
}

void display(sem_t *sem, SharedStruct *data, Options o) {
    options = o;
    init_window();
    int y = options.participant_count + 4;
    status_win = newwin(12, COLS, y, 0);

    while (1) {
        sem_wait(sem);
        memcpy(copy, data, sizeof(SharedStruct) * options.participant_count);
        sem_post(sem);
        qsort(copy, options.participant_count, sizeof(SharedStruct),
              comparator);
        if (finished()) {
            break;
        }
        print_table();
        write_status();
        sleep(1);
    }
    done = 1;

    // still display for 5 seconds
    for (int i = 0; i < 5; i++) {
        sem_wait(sem);
        memcpy(copy, data, options.participant_count * sizeof(SharedStruct));
        sem_post(sem);
        qsort(copy, options.participant_count, sizeof(SharedStruct),
              comparator);
        print_table(copy);
        write_status();
        sleep(1);
    }

    sleep(6);
    save_ranking();

    terminate_window();
}
