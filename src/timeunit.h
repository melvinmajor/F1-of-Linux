#ifndef TIMEUNIT_H
#define TIMEUNIT_H

typedef struct TimeUnit {
    int m;
    int s;
    int ms;
} TimeUnit;

TimeUnit to_time_unit(int ms);

TimeUnit minutes(int minutes);

int to_ms(TimeUnit timeUnit);

void to_string(int ms, char *str);


#endif //TIMEUNIT_H
