#ifndef _BASE_EPOLL_H_
#define _BASE_EPOLL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdarg.h>

#include "common.h"
#include "sdflog.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct epoll_event_element		epoll_event_element_t;
typedef struct base_epoll				base_epoll_t;

#define epoll_event_element_size		sizeof(epoll_event_element_t)
#define epoll_event_size				sizeof(base_epoll_t)

enum _E_BASE_EPOLL_EVENT_OPTION
{
	BASE_EPOLL_SET_READ_CALLBACK,
	BASE_EPOLL_SET_WRITE_CALLBACK,
	BASE_EPOLL_SET_CLOSE_CALLBACK,
	BASE_EPOLL_SET_USER_DATA,
};

//************************************
// Method:    epoll_base_new (new base_epoll instance)
// Returns:   base_epoll_t*
// Qualifier:
// Parameter: int max_session (max fd number) 
// Parameter: int max_event (max event handle per loop)
// Parameter: int timeout (epoll_wait timeout)
//************************************
base_epoll_t*							epoll_base_new(int max_session, int max_event,  int timeout);

//************************************
// Method:    epoll_base_delete (delete base_epoll instance)
// Returns:   void
// Qualifier:
// Parameter: base_epoll_t * (base_epoll instance)
//************************************
void									epoll_base_delete(base_epoll_t*);

//************************************
// Method:    epoll_event_add (add or update flag for trigger events)
// Returns:   int
// Qualifier:
// Parameter: base_epoll_t * (base_epoll instance)
// Parameter: int (File descriptor)
// Parameter: uint32_t (flag ex. EPOLLIN , EPOLLOUT , EPOLLET)
// Parameter: epoll_event_element_t ** (out param : epoll_event instance)
//************************************
int										epoll_event_add(base_epoll_t*, int, uint32_t, epoll_event_element_t **);

//************************************
// Method:    epoll_event_remove ( remove event handle by specific file descriptor )
// Returns:   int
// Qualifier:
// Parameter: base_epoll_t * (base_epoll instance)
// Parameter: int (File descriptor)
//************************************
int										epoll_event_remove(base_epoll_t*, int);

int										epoll_event_process(base_epoll_t *);

void									epoll_events_set_option(epoll_event_element_t * elem,int option,...);

//!--Extern vars
extern int _EPOLL_MAX_EVENT;
extern int _EPOLL_MAX_SESSION;
extern struct epoll_event* _EPOLL_EVENTS;

#ifdef __cplusplus
}
#endif

#endif
