struct TimeUnit {
    int m;
    int s;
    int ms;
};

struct TimeUnit toTimeUnit(int ms);
int toMs(struct TimeUnit timeUnit);
