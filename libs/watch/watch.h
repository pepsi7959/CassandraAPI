#ifndef WATCH_H
#define WATCH_H

#include <sys/time.h>

#ifndef MAX_TIMER_COUNTER
 #define MAX_TIMER_COUNTER 8
#endif

enum TMR_CNTR {
    TMR0 = 0,
    TMR1,
    TMR2,
    TMR3,
    TMR4,
    TMR5,
    TMR6,
    TMR7
    };

struct timeval tm_counter[MAX_TIMER_COUNTER];

int init_watch(int counter, char *err);
static int timeval_subtract (struct timeval *result
                      , struct timeval *end
                      , struct timeval *start);
int reset_watch(int counter, struct timeval *result_time, char *err);
int lap_watch(int counter, struct timeval *result_time, char *err);
int stop_watch(int counter, struct timeval *result_time, char *err);

#endif //WATCH_H
