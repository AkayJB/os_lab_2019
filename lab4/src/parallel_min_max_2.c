#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include "find_min_max_2.h"
#include "utils_2.h"

pid_t *child_pids;
int pnum = -1;

void alarmFunc(int a) {
    for (int i = 0; i < pnum; i++) {
        kill(child_pids[i], SIGKILL);
    }
}

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    bool with_files = false;
    int timeout = -1;

    signal(SIGALRM, alarmFunc);

    while (true) {
        int current_optind = optind ? optind : 1;
        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"timeout", required_argument, 0, 0},
            {0, 0, 0, 0}};
        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0: seed = atoi(optarg); break;
                    case 1: array_size = atoi(optarg); break;
                    case 2: pnum = atoi(optarg); break;
                    case 3: timeout = atoi(optarg); break;
                }
                break;
        }
    }

    if (seed == -1 || array_size == -1 || pnum == -1) {
        printf("Usage: %s --seed num --array_size num --pnum num --timeout num\n", argv[0]);
        return 1;
    }

    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);

    child_pids = malloc(sizeof(pid_t) * pnum);
    int fd[2];
    pipe(fd);

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    for (int i = 0; i < pnum; i++) {
        pid_t child_pid = fork();
        if (child_pid >= 0) {
            child_pids[i] = child_pid;
            if (child_pid == 0) {
                struct MinMax min_max = GetMinMax(array, i * (array_size / pnum), (i + 1) * (array_size / pnum));
                write(fd[1], &min_max, sizeof(min_max));
                return 0;
            }
        }
    }

    if (timeout > 0) alarm(timeout);

    for (int i = 0; i < pnum; i++) wait(NULL);

    close(fd[1]);
    struct MinMax result = {INT_MAX, INT_MIN};
    for (int i = 0; i < pnum; i++) {
        struct MinMax min_max;
        read(fd[0], &min_max, sizeof(min_max));
        if (min_max.min < result.min) result.min = min_max.min;
        if (min_max.max > result.max) result.max = min_max.max;
    }
    close(fd[0]);

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0 +
                          (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    printf("Min: %d\n", result.min);
    printf("Max: %d\n", result.max);
    printf("Elapsed time: %fms\n", elapsed_time);

    free(array);
    free(child_pids);
    return 0;
}