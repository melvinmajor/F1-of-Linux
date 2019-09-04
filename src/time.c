#include "time.h"
#include <stdio.h>
#include <stdlib.h>

Time time_from_ms(int ms) {
    Time time;
    div_t output;

    output = div(ms, 60000);
    time.m = output.quot;
    ms = output.rem;

    output = div(ms, 1000);
    time.s = output.quot;
    ms = output.rem;

    time.ms = ms;

    return time;
}

Time minutes(int minutes) { return (Time){.m = minutes, .s = 0, .ms = 0}; }

int to_ms(Time time) {
    int ms = 0;
    ms += time.ms;
    ms += time.s * 1000;
    ms += time.m * 60 * 1000;
    return ms;
}

int minute_to_ms(int minutes) { return minutes * 60000; }

void to_string(int ms, char *str) {
    struct Time time = time_from_ms(ms);
    sprintf(str, "%d:%d'%d", time.m, time.s, time.ms);
}
