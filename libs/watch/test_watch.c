
#include <stdlib.h>
#include <stdio.h>
#include "watch.h"

#define LOOP 1000

int main(){
    int i,r;
    struct timeval result_time;
    char tm_err[1024];
    init_watch(TMR0, tm_err);
    i = 0;
    while(i++ < LOOP){
        r=i;
        r++;}

    stop_watch(TMR0, &result_time, tm_err );
    printf( "Use time : %lu.%.6lu\n", result_time.tv_sec, result_time.tv_usec );

    init_watch(TMR1, tm_err);
    i = 0;
    for(; i++ < LOOP;){
        r=i;
        r++;
    }
    stop_watch(TMR1, &result_time, tm_err );
    printf( "Use time : %lu.%.6lu\n", result_time.tv_sec, result_time.tv_usec );

    return 0;
}
