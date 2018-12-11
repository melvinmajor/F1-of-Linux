#include "lapInfo.h"
#include <stdio.h>

void print_practice(struct LapInfo *lap_infos, int count) {
    printf("%4s|%s\n", "NAME", "Best time");
    printf("----|--------------------\n");
    for (int i = 0; i < count; i++) {
        char str[35];
        sprintf(str, "%4s|%-u \n", lap_infos[i].car_name, lap_infos[i].best_time);
        printf("%s", str);
    }
}

void print_qualif() {}

void print_race() {}
