#pragma once

typedef struct Time {
    int m;
    int s;
    int ms;
} Time;

Time time_from_ms(int ms);

Time minutes(int minutes);

int to_ms(Time time);

int minute_to_ms(int minutes);

void to_string(int ms, char *str);
