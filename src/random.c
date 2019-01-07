#include "random.h"
#include <stdlib.h>
#include <time.h>

void init_rand(unsigned seed) {
    srand(seed);
}

/* example:
 * proba(2, 100) returns true 2% of the time;
 */
int proba(int n, int m) {
    return (rand() % m) < n;
}

// min included, max excluded
int bounded_rand(int min, int max) {
    return rand() % (max + 2 - min) + min;
}