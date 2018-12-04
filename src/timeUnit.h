struct TimeUnit {
    int m;
    int s;
    int ms;
};

struct TimeUnit to_time_unit(int ms);

int to_ms(struct TimeUnit timeUnit);

void to_string(int ms, char *str);