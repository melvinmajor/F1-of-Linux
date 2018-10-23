#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 20

int f1[TABLE_SIZE] = {44, 77, 5, 7, 3, 33, 11, 31, 18, 35, 27, 55, 10, 28, 8, 20, 2, 14, 9, 16};

struct TimeUnit {
    int minutes;
    int seconds;
    int milliseconds;
};

//FIXME
struct TimeUnit toTimeUnit(int ms) {
    struct TimeUnit timeUnit;
    timeUnit.seconds = ms / 1000;
    timeUnit.milliseconds = ms % 1000;
    timeUnit.minutes = timeUnit.seconds / 60;
    timeUnit.seconds = timeUnit.seconds % 60;
    return timeUnit;
}

int toMs(struct TimeUnit *timeUnit) {
    int ms;
    ms += timeUnit->milliseconds;
    ms += timeUnit->seconds * 60;
    ms += timeUnit->minutes * 60 * 1000;
    return ms;
}

int my_rand(int min, int max) { return rand() % (max + 1 - min) + min; }

int main(int argc, char *argv[]) {
    struct TimeUnit min;
    min.minutes = 0;
    min.seconds = 30;
    min.milliseconds = 0;

    struct TimeUnit max;
    max.minutes = 0;
    max.seconds = 45;
    max.milliseconds = 0;

    int times[TABLE_SIZE];
    int minimum = toMs(&min);
    int maximum = toMs(&max);

    int i = 0;
    for (; i < TABLE_SIZE; i++) {
        int ra = my_rand(minimum, maximum);
        struct TimeUnit timeUnit = toTimeUnit(ra);
        printf("name: %d, min: %d, sec: %d, ms: %d\n", f1[i], timeUnit.minutes, timeUnit.seconds,
               timeUnit.milliseconds);
    }

    exit(0);
}
