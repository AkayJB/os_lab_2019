#ifndef FIND_MIN_MAX_2_H
#define FIND_MIN_MAX_2_H

struct MinMax {
    int min;
    int max;
};

struct MinMax GetMinMax(int *array, int begin, int end);

#endif