
/*
Copyright (C) 2014, MIMO Tech Co., Ltd.

sdflog.c

Revision history

   1.0.0   Initialize.

*/

#include <stdio.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "sdflog.h"


static int log_level = SDFLOG_LEVEL_ALL;
static char log_dir[1024];
static char log_name[256];
#ifdef SDFLOG_SPLIT
static int log_split_interval = 5;
#endif

static jmp_buf log_recovery;
static char log_buffer[SDFLOG_MSG_SIZE];

static void
timeout(int sig)
{
    longjmp(log_recovery, 1);
}

#ifdef SDFLOG_SPLIT
void SDF_log_init (char *name, char *dir, int interval)
#else
void SDF_log_init (char *name, char *dir)
#endif
{
    int n;
#ifdef SDFLOG_SPLIT
   log_split_interval = interval;
#endif
    n = (int)strlen(name);
    if (n >= sizeof(log_name)-1)
        n = sizeof(log_name)-1 - 1;
    (void)memcpy(log_name, name, n);
    log_name[n] = '\0';

    n = (int)strlen(dir);
    if (n > 0 && dir[n - 1] == '/')
        --n;
    if (n >= sizeof(log_dir)-1)
        n = sizeof(log_dir)-1 - 1;
    (void)memcpy(log_dir, dir, n);
    log_dir[n] = '\0';
}


static int
memXcmp(char *m1, char *M2, int len)
{
#define _TOU(_c)   ( ((_c)>='a'&&(_c)<='z')? ((_c)-'a'+'A') : (_c) )
    while (len > 0)
    {
        if (*m1 != *M2 && _TOU(*m1) != *M2)
            return -1;
        ++m1;
        ++M2;
        --len;
    }
    return 0;
}


int
SDF_set_level_string(char *level)
{
#define _ISLEV(_S,_v)                            \
    (                                                \
    (memXcmp (level, (_S), sizeof(_S)-1) == 0)?   \
    (                                             \
    lev |= (_v),                               \
    level += sizeof(_S)-1,                     \
    1                                          \
    )                                             \
    :                                             \
    0                                             \
    )
#define _SKIP_WHITESPACE   { while (*level == ' ' || *level == '\t' || *level == '\n' || *level == '\r') ++level; }
#define _IS_DELIMITER   ( (*level == ' ' || *level == ',' || *level == '+' || *level == '|')? (++level, 1) : (((*level) == '\0')? 1 : 0) )
    int lev;

    lev = SDFLOG_LEVEL_NONE;
    _SKIP_WHITESPACE
        if (*level == '\0')
            return -1;
    while (*level)
    {
        if (_ISLEV("NONE", SDFLOG_LEVEL_NONE))
        {
            if (!_IS_DELIMITER)
                return -2;
            lev = SDFLOG_LEVEL_NONE;
        }
        else if (_ISLEV("FATAL", SDFLOG_LEVEL_FATAL))
        {
            if (!_IS_DELIMITER)
                return -2;
        }
        else if (_ISLEV("ERROR", SDFLOG_LEVEL_ERROR))
        {
            if (!_IS_DELIMITER)
                return -2;
        }
        else if (_ISLEV("WARN", SDFLOG_LEVEL_WARN))
        {
            if (!_IS_DELIMITER)
                return -2;
        }
        else if (_ISLEV("WARNNING", SDFLOG_LEVEL_WARN))
        {
            if (!_IS_DELIMITER)
                return -2;
        }
        else if (_ISLEV("INFO", SDFLOG_LEVEL_INFO))
        {
            if (!_IS_DELIMITER)
                return -2;
        }
        else if (_ISLEV("DEBUG", SDFLOG_LEVEL_DEBUG))
        {
            if (!_IS_DELIMITER)
                return -2;
        }
        else if (_ISLEV("ALL", SDFLOG_LEVEL_ALL))
        {
            if (!_IS_DELIMITER)
                return -2;
        }
        else
            return -3;
    }
    log_level = lev;
    return 0;
}


/* Enable GCC's LTO to automatic inlining for maximum performance. */
int
SDF_log_level()
{
    return log_level;
}

char*
SDF_log_get_buffer()
{
    return (char *)log_buffer;
}

char *
level_to_string(int level)
{
    switch (level)
    {
        case SDFLOG_LEVEL_FATAL:
            return "FATAL";
        case SDFLOG_LEVEL_ERROR:
            return "ERROR";
        case SDFLOG_LEVEL_WARN:
            return "WARNING";
        case SDFLOG_LEVEL_INFO:
            return "INFO";
        case SDFLOG_LEVEL_DEBUG:
            return "DEBUG";
    }
    return "UNKNOWN";
}


void
SDF_log_write_string(int level, char *file, char *func, int line, char *msg)
{
    char buff[(2 * 1024) + SDFLOG_MSG_SIZE];
    int fd, n;
    struct tm tm;
    //time_t ct;
#ifdef SDFLOG_SPLIT
   int min, hour, tmp;
#endif
    void(*old_sig) (int);

    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    tm = *localtime(&curTime.tv_sec);
    int micro = curTime.tv_usec;
#ifdef SDFLOG_SPLIT
   if (log_split_interval > 0){
		tmp = (tm.tm_hour * 60) + tm.tm_min;
		tmp = tmp - (tmp % log_split_interval);
		hour = tmp / 60;
		min = tmp % 60;
		
	   (void) sprintf (buff, "%s/%s_%04d%02d%02d_%02d%02d.log", log_dir, log_name, 1900+tm.tm_year, tm.tm_mon+1, tm.tm_mday, hour, min);
   }else{
	   (void) sprintf (buff, "%s/%s", log_dir, log_name);
   }
#else
    (void)sprintf(buff, "%s/%s_%04d%02d%02d.log", log_dir, log_name, 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday);
#endif
    fd = open(buff, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    n = (int)strlen(msg);
    if (n > SDFLOG_MSG_SIZE)
        n = SDFLOG_MSG_SIZE;
    (void)sprintf(buff, "%s|%02d:%02d:%02d.%06d|%d|%s|%s|%d|%.*s\n", level_to_string(level), tm.tm_hour, tm.tm_min, tm.tm_sec, micro, getpid(), file, func, line, n, msg);
    n = (int)strlen(buff);

    old_sig = signal(SIGALRM, timeout);
    if (setjmp(log_recovery) == 0)
    {
        (void)alarm(10);
        if (fd >= 0)
            (void)write(fd, buff, n);
        (void)write(STDERR_FILENO, buff, n);
        (void)alarm(0);
    }
    (void)signal(SIGALRM, old_sig);
    if (fd >= 0)
        (void)close(fd);
}

void
get_file_message_name(char* buff)
{
    if (buff == NULL)
        return;

    struct tm tm;
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    tm = *localtime(&curTime.tv_sec);

    (void)sprintf(buff, "%s/%s_%04d%02d%02d_message.log", log_dir, log_name, 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday);
}

//CALL update_time BEFORE get_file_name for real time filename other last file name.
void
get_file_name(char* buff)
{
    if (buff == NULL)
        return;

    struct tm tm;
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    tm = *localtime(&curTime.tv_sec);

#ifdef SDFLOG_SPLIT
    int min, hour, tmp;
#endif

#ifdef SDFLOG_SPLIT
    if (log_split_interval > 0){
        tmp = (tm.tm_hour * 60) + tm.tm_min;
        tmp = tmp - (tmp % log_split_interval);
        hour = tmp / 60;
        min = tmp % 60;

        (void)sprintf(buff, "%s/%s_%04d%02d%02d_%02d%02d.log", log_dir, log_name, 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday, hour, min);
    }
    else{
        (void)sprintf(buff, "%s/%s", log_dir, log_name);
    }
#else
    (void)sprintf(buff, "%s/%s_%04d%02d%02d.log", log_dir, log_name, 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday);
#endif
}

char* get_log_dir(void)
{
    return log_dir;
}
