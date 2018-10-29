#include <stdlib.h>
#include <time.h>

// n percentage
void init_rand() { srand(time(NULL)); }

int proba(int n) { return (rand() % 100) < n; }
