#include "random.h"

void init_random(unsigned int seed) { srand(seed); }

// produce a random number in the inclusive range [min, max]
int next_random(int min, int max) { return rand() % (max + 1 - min) + min; }

int chance(int arg) { return (rand() % arg) < 1; }
