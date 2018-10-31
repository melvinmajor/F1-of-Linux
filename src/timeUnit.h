struct TimeUnit {
    int m;
    int s;
    int ms;
};
struct TimeUnit new_time_unit(int min, int sec, int ms);
struct TimeUnit to_time_unit(int ms);
int to_ms(struct TimeUnit timeUnit);
