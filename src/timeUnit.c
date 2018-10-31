#include "timeUnit.h"
#include <stdlib.h>

struct TimeUnit new_time_unit(int min, int sec, int ms) {
    struct TimeUnit t;
    t.m = min;
    t.s = sec;
    t.ms = ms;
    return t;
}

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
