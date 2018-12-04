#include "timeUnit.h"
#include <stdio.h>
#include <stdlib.h>

struct TimeUnit to_time_unit(int ms) {
    struct TimeUnit timeUnit;
    div_t output;

    output = div(ms, 60000);
    timeUnit.m = output.quot;
    ms = output.rem;

    output = div(ms, 1000);
    timeUnit.s = output.quot;
    ms = output.rem;

    timeUnit.ms = ms;

    return timeUnit;
}

int to_ms(struct TimeUnit timeUnit) {
    int ms = 0;
    ms += timeUnit.ms;
    ms += timeUnit.s * 1000;
    ms += timeUnit.m * 60 * 1000;
    return ms;
}

void to_string(int ms, char *str) {
    struct TimeUnit time = to_time_unit(ms);
    sprintf(str, "%d'%d", time.s, time.ms);
}