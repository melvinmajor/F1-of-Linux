#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float main(void) {
    int max = 45;
    int min = 30;
    #define NUM_COUNT (max -min + 1)

    srand(time(NULL));
    float qualif = min + rand()%NUM_COUNT/100.0;
    printf("%.3f\n", qualif);

    return 0;
}