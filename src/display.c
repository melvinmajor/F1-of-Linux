#include <stdio.h>

void print_practice(// TODO lapinfos, count) {
    printf("%4s|%s\n", "NAME", "TT");
    printf("----|--------------------\n");
    for (int i = 0; i < car_number; i++) {
        char str[35];
        sprintf(str, "%4s|%-u \n", names[i], best_times[i]);
        printf("%s", str);
    }
}

void print_qualif() {}

void print_race() {}
