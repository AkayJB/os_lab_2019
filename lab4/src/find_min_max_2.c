#include <limits.h>
#include "find_min_max_2.h"

struct MinMax GetMinMax(int *array, int begin, int end) {
    struct MinMax min_max = {INT_MAX, INT_MIN};
    for (int i = begin; i < end; i++) {
        if (array[i] < min_max.min) min_max.min = array[i];
        if (array[i] > min_max.max) min_max.max = array[i];
    }
    return min_max;
}