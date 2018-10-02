#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float main(void) {
    int max = 45;
    int min = 30;

    srand(time(NULL));
    float s1 = rand()%(max+1-min)+min; // TODO : "error: invalid operands to binary % (have 'int' and 'float')"
    printf("%.3f\n", s1);

    return 0;
}