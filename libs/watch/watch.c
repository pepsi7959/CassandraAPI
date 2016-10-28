#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

#include "watch.h"

extern struct timeval tm_counter[];

static int
timeval_subtract (struct timeval *result, struct timeval *end,struct timeval *start)
{
    if (end->tv_usec < start->tv_usec) {
        int nsec = (start->tv_usec - end->tv_usec) / 1000000 + 1;
        start->tv_usec -= 1000000 * nsec;
        start->tv_sec += nsec;
    }
    if (end->tv_usec - start->tv_usec > 1000000) {
        int nsec = (end->tv_usec - start->tv_usec) / 1000000;
        start->tv_usec += 1000000 * nsec;
        start->tv_sec -= nsec;
    }

    result->tv_sec = end->tv_sec - start->tv_sec;
    result->tv_usec = end->tv_usec - start->tv_usec;

    return end->tv_sec < start->tv_sec;
}

int init_watch(int counter, char *err)
{
    if( counter >= 0 && counter < MAX_TIMER_COUNTER ){
        if( gettimeofday( &tm_counter[counter], NULL ) != 0 ){
            sprintf(err, "Cannot gettimeofday [%s]", strerror( errno ) );
            return -1;
        }
    }else{
        sprintf(err, "Counter is more then MAX_COUNTER[%d,%d]", counter, MAX_TIMER_COUNTER);
        return -1;
    }
    return 0;
}


int reset_watch(int counter, struct timeval *result_time, char *err )
{
    struct timeval curr_time;
    if( counter >= 0 && counter < MAX_TIMER_COUNTER ){
        if ( gettimeofday( &curr_time, NULL ) != 0 ){
            sprintf(err, "Cannot get time [%s]", strerror( errno ) );
        }

        timeval_subtract( result_time, &curr_time, &tm_counter[counter]);
        tm_counter[counter].tv_sec = curr_time.tv_sec;
        tm_counter[counter].tv_usec = curr_time.tv_usec;

    }else{
        sprintf(err, "Counter is more then MAX_COUNTER[%d,%d]", counter, MAX_TIMER_COUNTER);
        return -1;
    }

    return 0;
}


int lap_watch(int counter, struct timeval *result_time, char *err )
{
    struct timeval curr_time;

    if( counter >= 0 && counter < MAX_TIMER_COUNTER ){
        if ( gettimeofday( &curr_time, NULL ) != 0 ){
            sprintf( err, "Cannot get time [%s]", strerror( errno ) );
        }

        timeval_subtract( result_time, &curr_time, &tm_counter[counter]);

    }else{
        sprintf(err, "Counter is more then MAX_COUNTER[%d,%d]", counter, MAX_TIMER_COUNTER);
        return -1;
    }

    return 0;
}


int stop_watch(int counter, struct timeval *result_time , char *err)
{
    struct timeval curr_time;

    if( counter >= 0 && counter < MAX_TIMER_COUNTER ){
        if ( gettimeofday( &curr_time, NULL ) != 0 ){
            sprintf( err, "Cannot get time [%s]", strerror( errno ) );
        }

        timeval_subtract( result_time, &curr_time, &tm_counter[counter]);
        timerclear(&tm_counter[counter]);

    }else{
        sprintf(err, "Counter is more then MAX_COUNTER[%d,%d]", counter, MAX_TIMER_COUNTER);
        return -1;
    }

    return 0;
}
