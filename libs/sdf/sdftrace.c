
/*

   sftrace.c

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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "sdftrace.h"

#define SDFTRACE_NUMBER_SIZE						256
#define SDFTRACE_SESSION_SIZE                       256
#define SDFTRACE_MAX_SESSION_LIST                   32

static char trace_dir[1024];
static char trace_name[256];

typedef struct _my_trace_
{
	int num_size;
	int session_index;
	char number_list[SDFTRACE_MAX_NUMBER_LIST][SDFTRACE_NUMBER_SIZE];
	char session_list[SDFTRACE_MAX_SESSION_LIST][SDFTRACE_SESSION_SIZE];

} MY_TRACE;

static MY_TRACE mtrace;
static jmp_buf trace_recovery;

static void
timeout (int sig)
{
   longjmp (trace_recovery, 1);
}


void
SDF_trace_init (char *name, char *dir)
{
	int n;

	(void) memset (&mtrace, 0, sizeof(MY_TRACE));
	n = (int) strlen (name);
	if (n >= sizeof(trace_name)-1)
		n = sizeof(trace_name)-1-1;
	(void) memcpy (trace_name, name, n);
	trace_name[n] = '\0';

	n = (int) strlen (dir);
	if (n > 0 && dir[n-1] == '/') --n;
	if (n >= sizeof(trace_dir)-1)
		n = sizeof(trace_dir)-1-1;
	(void) memcpy (trace_dir, dir, n);
	trace_dir[n] = '\0';
}

int
SDF_trace_set_option (int option, int type, char *number, char *err)
{
	int r, i;
	switch (option)
	{
		case SDFTRACE_OPTION_SET:
			for (i=0; i<SDFTRACE_MAX_NUMBER_LIST; i++)
			{
				if (mtrace.number_list[i][0]==0)
				{
					strncpy(mtrace.number_list[i], number, SDFTRACE_NUMBER_SIZE);
					++(mtrace.num_size);
					return SDFTRACE_RET_SUCCESS;
				}
			}
			if (i==SDFTRACE_MAX_NUMBER_LIST)
			{
				r = SDFTRACE_RET_ERR_OPTION;
				if (err!=NULL) strcpy(err, "Number list exceeds");
			}
			break;
		case SDFTRACE_OPTION_REMOVE: r = SDFTRACE_RET_ERR_OPTION;
			if (err!=NULL) strcpy(err, "Not implement yet");
			break;
		case SDFTRACE_OPTION_RESET:
			(void) memset (&mtrace, 0, sizeof(MY_TRACE));
			r = SDFTRACE_RET_SUCCESS;
			break;
			
		case SDFTRACE_OPTION_NONE:
		default: r = SDFTRACE_RET_ERR_OPTION;
			if (err!=NULL) strcpy(err, "Unknown option");
	}

	return r;
}

static void
sf_trace_write_string (char *string)
{
	char buff[1024];
	int fd, n;
	struct tm tm;
	time_t ct;
	void (*old_sig) (int);

	ct = time (NULL);
	tm = *localtime (&ct);

	(void) snprintf (buff, 1024, "%s/%s_%04d%02d%02d.trace", trace_dir, trace_name, 1900+tm.tm_year, tm.tm_mon+1, tm.tm_mday);
	fd = open (buff, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);


	old_sig = signal (SIGALRM, timeout);
	if (setjmp (trace_recovery) == 0)
	{
		n = (int) strlen (string);
		(void) alarm (10);
		if (fd >= 0) (void) write (fd, string, n);
		(void) alarm (0);
	}
	(void) signal (SIGALRM, old_sig);
	if (fd >= 0) (void) close (fd);
}

void
SDF_trace_message (int direction, const char *msg)
{
	int logging=0, n=0, i;
	char *buff=NULL;
	char matched_number[SDFTRACE_NUMBER_SIZE];
	char matched_session[SDFTRACE_SESSION_SIZE];
	struct tm tm;
	struct timeval tv;
	time_t ct;

	if (msg==NULL) return;
	if (mtrace.num_size==0) return;
	
	(void) memset (matched_number, 0, sizeof(matched_number));
 	(void) memset (matched_session, 0, sizeof(matched_session));
  
	for (i=0; i<SDFTRACE_MAX_NUMBER_LIST; i++)
	{
		if (mtrace.number_list[i][0]!=0)
		{
			if (strstr(msg, mtrace.number_list[i])!=NULL)
			{
				logging = 1;
				strcpy(matched_number, mtrace.number_list[i]);
				break;
			}
		}
	}
	if (logging==1)
	{
		char *ptr=NULL, *_ptr=NULL;
		// Find session id
		ptr = strstr(msg, " session=\"");
		if (ptr!=NULL)
		{
			ptr += strlen(" session=\"");
			_ptr = strstr(ptr, "\"");
			for (i=0; i<SDFTRACE_MAX_SESSION_LIST; i++)
				if (strncmp(mtrace.session_list[i], ptr, _ptr-ptr)==0)
					break;
			if (i==SDFTRACE_MAX_SESSION_LIST)
			{
				memcpy(mtrace.session_list[mtrace.session_index], ptr, _ptr-ptr);
				strcpy(matched_session, mtrace.session_list[mtrace.session_index]);
				++(mtrace.session_index);
				if (mtrace.session_index >= SDFTRACE_MAX_SESSION_LIST) mtrace.session_index = 0;
			}
		}
	}
	else
	{
		for (i=0; i<SDFTRACE_MAX_SESSION_LIST; i++)
		{
			if (mtrace.session_list[i][0]!=0)
			{
				if (strstr(msg, mtrace.session_list[i])!=NULL)
				{
					logging = 1;
					strcpy(matched_session, mtrace.session_list[i]);
					break;
				}
			}
		}
	}

	ct = time (NULL);
	tm = *localtime (&ct);
	gettimeofday(&tv, NULL);
	n = (int) strlen(msg) + 1024;
	buff = (char *) malloc (n);
	if (direction==SDFTRACE_INPUT)
	{
		(void) sprintf (buff,
						"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
						" INPUT MESSAGE\n"
						"        : Time %02d.%02d.%04d %02d:%02d:%02d.%06d\n"
						"        : Number %s\n"
						"        : Session %s\n"
						"        : Size %d\n"
						"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
						"[%s]\n\n",
						tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec, (int)tv.tv_usec,
						matched_number, matched_session, n-1024, msg);
	}
	else
	{
		(void) sprintf (buff,
						">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
						" OUTPUT MESSAGE\n"
						"        : Time %02d.%02d.%04d %02d:%02d:%02d.%06d\n"
						"        : Number %s\n"
						"        : Session %s\n"
						"        : Size %d\n"
						">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
						"[%s]\n\n",
						tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec, (int)tv.tv_usec,
						matched_number, matched_session, n-1024, msg);
	}
						
	
	if (logging==1) sf_trace_write_string (buff);
	free(buff);
}

