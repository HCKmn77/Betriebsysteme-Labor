/*
 * Small benchmark for Linux-Pipes.
 *
 * Author: Fabian Schneider, Jeremia Haackmann
 */
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#include "bench_utils.h"

#define SLEEP_TIME 1

// #define USE_MEMSET

int main(int argc, char *argv[])
{
    // Set up test packages from 64 B to 16 MB

    int min_package_size = 64; // 64 Bytes
    int max_package_size = 16777216; // 16 MB
    int sizes_num = 0;

    for (int i = min_package_size; i <= max_package_size; i *= 4)
    {
        sizes_num++;
        // printf("s: %d i: %d\n", num_packages, i);
    }
    int sizes[sizes_num];
    int k = 1; 
    for (int i = 0; i < sizes_num; i++){
        sizes[i] = k*min_package_size;
        k *= 4;
        printf("Size[%d]: %d\n", i, sizes[i]);
    }
#define MAX_SIZE sizes[sizes_num - 1]
    pid_t pid = getpid();
    int pdes[2];
    int ret;
    int i;
    char *buffer;
    FILE *fptr;

    //  Create new pipe for read and write action

    ret = pipe(pdes);

    if (ret != 0)
        ERROR("pipe()", errno);

    pid = getpid();

    ret = fork();

    if (-1 == ret)
        ERROR("fork", errno);

    buffer = malloc(MAX_SIZE);
    if (NULL == buffer)
        ERROR("malloc", ENOMEM);
    memset(buffer, 0, MAX_SIZE);

    if (0 == ret)
    {
        // Process 1: Reads from pipe

        int nread = 1;
        pid = getpid();
        close(pdes[1]);
        while (nread != 0)
        {
            nread = read(pdes[0], buffer, MAX_SIZE);
        }
        close(pdes[0]);
        return (EXIT_SUCCESS);
    }
    else if (0 < ret)
    {
        // Process 2: Writes to pipe

        int *ticks;
        close(pdes[0]);
        memset(buffer, 'a', MAX_SIZE);
        ticks = malloc(MEASUREMENTS * sizeof(int));

        if (NULL == ticks)
            ERROR("malloc", ENOMEM);
        memset(ticks, 0, MEASUREMENTS * sizeof(int));

        for (i = 0; i < sizes_num; i++)
        {
            int current_size = sizes[i];
            int j;
            int min_ticks;
            int max_ticks;
            long long ticks_all;
            struct timeval tv_start;
            struct timeval tv_stop;
            double time_delta_sec;

            assert(current_size <= MAX_SIZE);

            gettimeofday(&tv_start, NULL);
            for (j = 0; j < MEASUREMENTS; j++)
            {
                unsigned long long start;
                unsigned long long stop;
                start = getrdtsc();
                write(pdes[1], buffer, current_size);

                stop = getrdtsc();
                ticks[j] = stop - start;
            }
            gettimeofday(&tv_stop, NULL);



            min_ticks = INT_MAX;
            max_ticks = INT_MIN;
            ticks_all = 0;
            for (j = 0; j < MEASUREMENTS; j++)
            {
                if (min_ticks > ticks[j])
                    min_ticks = ticks[j];
                if (max_ticks < ticks[j])
                    max_ticks = ticks[j];
                ticks_all += ticks[j];
            }
            ticks_all -= min_ticks;
            ticks_all -= max_ticks;

            time_delta_sec = ((tv_stop.tv_sec - tv_start.tv_sec) + ((tv_stop.tv_usec - tv_start.tv_usec) / (1000.0 * 1000.0)));

            double bandwidth = ((double)current_size * MEASUREMENTS) / (1024.0 * 1024.0 * time_delta_sec); 

            // Write measurements into file
            fptr = fopen("./messungen.txt", "a");
            fprintf(fptr, "%d; %.2f\n", current_size, bandwidth);
            fclose(fptr);
            
            // Print measurements
            printf ("PID:%d time: min:%d max:%d Ticks Avg without min/max:%f Ticks (for %d measurements) for %d Bytes (%.2f MB/s)\n",
                pid, min_ticks, max_ticks,
                (double) ticks_all / (MEASUREMENTS-2.0), MEASUREMENTS, current_size,
                ((double) current_size * MEASUREMENTS)  / (1024.0*1024.0*time_delta_sec) );        }
    }
    return (EXIT_SUCCESS);
}
