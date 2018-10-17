#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    int min = 40
    int max = 70;
    
    srand(time(NULL));
    int random_loop = rand() % (max+1-min)+min;
    printf("%d\n", random_loop);

    return 0;
 }