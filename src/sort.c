#include "lapInfo.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int lap_sort(const void *a, const void *b) {
    struct LapInfo *aa = (struct LapInfo *)a;
    struct LapInfo *bb = (struct LapInfo *)b;

    unsigned int sum_a = lap_time(aa);
    unsigned int sum_b = lap_time(bb);

    return sum_a - sum_b;
}

struct LapInfo *sort_by_time(struct LapInfo *lap_infos, int size) {
    size_t sizes = sizeof(struct LapInfo) * size;
    struct LapInfo *lap_copy = malloc(sizes);
    memcpy(lap_copy, lap_infos, sizes);
    qsort(lap_copy, size, sizes, lap_sort);
    return lap_copy;
}
