/**************************************************************************
 * File        : granularity.c                                            *
 *                                                                        *
 * Description :                                                          *
 *   This C program determines the granularity of the system clock.       *
 **************************************************************************/

#ifndef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifndef HAVE_TIME_H
#include <sys/time.h>
#endif
#ifndef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifndef ULONG_MAX
#define ULONG_MAX ((unsigned long)(~0L)) /* 0xFFFFFFFF */
#endif

/**************************************************************************
 * init_time                                                              *
 *                                                                        *
 * This function initializes the ITIMER_PROF timer.                       *
 **************************************************************************/
void init_time()
{
    struct itimerval *clock_settings;
    clock_settings = (struct itimerval *)malloc(sizeof(struct itimerval));
    clock_settings->it_value.tv_sec = 3600;
    clock_settings->it_value.tv_usec = 0;
    clock_settings->it_interval.tv_sec = 3600;
    clock_settings->it_interval.tv_usec = 0;
    setitimer(ITIMER_PROF, clock_settings, NULL);
    free(clock_settings);
}

/**************************************************************************
 * get_time                                                               *
 *                                                                        *
 * This function returns the actual timestamp of the ITIMER_PROF timer.   *
 *                                                                        *
 * @returns the actual timestamp.                                         *
 **************************************************************************/
long get_time()
{
    struct itimerval curr_time;
    getitimer(ITIMER_PROF, &curr_time);
    return curr_time.it_value.tv_sec * 1000000 + curr_time.it_value.tv_usec;
}

int main(int argc, char **argv)
{

    int i = 0;
    int diff_runs = 50;

    if (argc >= 2)
    {
        diff_runs = atoi(argv[1]);
    }

    printf("Determining system clock granularity .. \n");

    init_time();

    unsigned long last_time = get_time();
    unsigned long current_time = last_time;
    unsigned long time_diff = 0;
    unsigned long min_diff = ULONG_MAX;
    unsigned long max_diff = 0;
    unsigned long avg_diff = 0;

    while (i < diff_runs)
    {
        current_time = get_time();

        time_diff = last_time - current_time;

        if (time_diff)
        {
            last_time = current_time;
            min_diff = (min_diff > time_diff) ? time_diff : min_diff;
            max_diff = (max_diff < time_diff) ? time_diff : max_diff;
            avg_diff += time_diff;
            i++;
        }
    }

    avg_diff /= diff_runs;

    printf("runs: %d, max. diff.: %lu us, min. diff.: %lu us, avg. diff.: %lu us\n", diff_runs, max_diff, min_diff, avg_diff);

    return 0;
}
