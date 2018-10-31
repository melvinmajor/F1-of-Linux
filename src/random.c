#include <stdlib.h>
#include <time.h>

// n percentage
void init_rand() { srand(time(NULL)); }

int proba(int n) { return (rand() % 100) < n; }

int bounded_rand(int min, int max) { return rand() % (max + 1 - min) + min; }
