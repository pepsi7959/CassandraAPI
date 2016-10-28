#include "base_epoll.h"

//!--Extern vars
int _EPOLL_MAX_EVENT				= 0;
int _EPOLL_MAX_SESSION				= 0;
struct epoll_event*	_EPOLL_EVENTS	= NULL;	//!--Events table.

struct base_epoll
{
	epoll_event_element_t*		table;
	size_t						timeout;
	int							epoll_fd;
	void*						data;
};

struct epoll_event_element
{
	int							fd;
	int							(*write_callback) (base_epoll_t *, epoll_event_element_t *, struct epoll_event , int fd , void * user_data);
	int							(*read_callback) (base_epoll_t *, epoll_event_element_t *, struct epoll_event , int fd , void * user_data);
	int							(*close_callback) (base_epoll_t *, epoll_event_element_t *, struct epoll_event , int fd , void * user_data);
	void*						data;
	uint32_t					events;
	uint32_t					cur_event;
};


epoll_event_element_t * epoll_element_new(int fd,epoll_event_element_t* elem, uint32_t events)
{
	epoll_event_element_t* _elem = NULL;
	if ( elem->fd < 0 )
	{
		_elem = elem;
		_elem->fd = fd;
		_elem->events = events;
	}
	return _elem;
}

base_epoll_t * epoll_base_new(int max_session, int max_event,  int timeout)
{
	static char func[] = "epoll_event_new";

	base_epoll_t * poll_event = (base_epoll_t*)calloc(1, epoll_event_size);
	if (!poll_event)
	{
		SDFLOG_ERROR( ( B,"calloc failed at poll_event") );
		return NULL; // No Memory
	}

	poll_event->table = (epoll_event_element_t*)calloc( max_session , epoll_event_element_size );

	if (!poll_event->table)
	{
		free(poll_event);
		SDFLOG_ERROR( ( B,"calloc failed at map-table") );
		return NULL;
	}
	memset( poll_event->table , 0 , epoll_event_element_size * max_session );
	int idx_fd;
	for(idx_fd = 0 ; idx_fd < max_session ; ++idx_fd )
	{
		poll_event->table[idx_fd].fd = -1;
	}

	poll_event->timeout = timeout;
	poll_event->epoll_fd = epoll_create(max_event);

	struct epoll_event ev;
	ev.data.fd = poll_event->epoll_fd;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
	(void)epoll_ctl (poll_event->epoll_fd, EPOLL_CTL_ADD, poll_event->epoll_fd, &ev);

	_EPOLL_MAX_EVENT	= max_event;
	_EPOLL_MAX_SESSION	= max_session;
	_EPOLL_EVENTS		= (struct epoll_event *)calloc( _EPOLL_MAX_EVENT , sizeof(struct epoll_event) );

	SDFLOG_DEBUG( ( B,"Init epoll - fd %d , max_event %d , max_session %d ", poll_event->epoll_fd , _EPOLL_MAX_EVENT , _EPOLL_MAX_SESSION ) );

	return poll_event;
}

void epoll_base_delete(base_epoll_t* poll_event)
{
	static char func[] = "epoll_event_delete";

	SDFLOG_DEBUG( ( B,"delete base_epoll") );
	free(poll_event->table);
	close(poll_event->epoll_fd);
	free(poll_event);
	if( _EPOLL_EVENTS != NULL )
	{
		free(_EPOLL_EVENTS);
	}
}

int epoll_event_add(base_epoll_t* poll_event, int fd, uint32_t flags, epoll_event_element_t **poll_element)
{
	static char func[] = "epoll_event_add";

	if( fd >= _EPOLL_MAX_SESSION || fd < 0 ) //!--Out of range.
	{
		SDFLOG_ERROR( ( B,"FD tables exceed : sock(%d) (MAX_SESSION %d)", fd , _EPOLL_MAX_SESSION ) );
		return -1;
	}

	epoll_event_element_t* elem = (epoll_event_element_t *)&(poll_event->table[fd]);
	if (elem->fd >= 0)
	{
		//SDFLOG_DEBUG( ( B,"fd (%d) update flags", fd) );
		elem->events |= flags;
		struct epoll_event ev;
		memset(&ev, 0, sizeof(struct epoll_event));
		ev.data.fd = fd;
		ev.events = elem->events;
		*poll_element = elem;
		return epoll_ctl(poll_event->epoll_fd, EPOLL_CTL_MOD, fd, &ev);
	}
	else
	{
		elem = epoll_element_new(fd , &poll_event->table[fd] , flags );
		if ( !elem )
		{
			return -1;
		}

		//SDFLOG_DEBUG( ( B,"fd(%d) add flags", fd));
		struct epoll_event ev;
		memset(&ev, 0, sizeof(struct epoll_event));
		ev.data.fd = fd;
		ev.events = elem->events;
		*poll_element = elem;
		return epoll_ctl(poll_event->epoll_fd, EPOLL_CTL_ADD, fd, &ev);
	}
}

int epoll_event_remove(base_epoll_t* poll_event, int fd)
{
    static char func[] = "epoll_event_add";
    if (fd >= _EPOLL_MAX_SESSION || fd < 0) //!--Out of range.
    {
        SDFLOG_ERROR((B, "FD tables exceed : sock(%d) (MAX_SESSION %d)", fd, _EPOLL_MAX_SESSION));
        return -1;
    }

	// TODO Handle error
	memset( &(poll_event->table[fd]) , 0 , epoll_event_element_size);

	//!--Set free flag
	poll_event->table[fd].fd = -1;
	epoll_ctl(poll_event->epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	return 0;
}

int epoll_event_process(base_epoll_t * poll_event)
{
	static char func[] = "epoll_event_process";

	//!--static allocate
    struct epoll_event* events = _EPOLL_EVENTS ;

    int event_count = epoll_wait(poll_event->epoll_fd, events, _EPOLL_MAX_EVENT, (int)poll_event->timeout);
    int i = 0;
    for( ; i < event_count ; i++)
    {
        epoll_event_element_t * value = NULL;
		if( events[i].data.fd >= _EPOLL_MAX_SESSION || events[i].data.fd < 0  ) //!--Out of range.
		{
			SDFLOG_ERROR( ( B,"FD tables exceed : event_id (%d) , sock(%d) (MAX_SESSION %d)", i, events[i].data.fd , _EPOLL_MAX_SESSION ) );
			continue;
		}
        if ((value = (epoll_event_element_t *) &(poll_event->table[events[i].data.fd])) != NULL)
        {
            int _callback_ret = 0;

            if ( events[i].events & EPOLLOUT )
            {
                if (value->write_callback)
                {
                    _callback_ret = value->write_callback(poll_event, value, events[i], value->fd, value->data);
                }
            }

            if ( events[i].events & EPOLLIN && _callback_ret == 0 )
            {
                if (value->read_callback )
				{
                    _callback_ret = value->read_callback(poll_event, value, events[i] ,  value->fd , value->data );
				}
            }

#ifdef EPOLLRDHUP

            if ( (events[i].events & EPOLLRDHUP) || (events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP))
            {
                if (events[i].events & EPOLLRDHUP)
                {
                    //SDFLOG_DEBUG( ( B,"found EPOLLRDHUP for event id(%d) and sock(%d)", i, events[i].data.fd ));
                    //value->cur_event &= EPOLLRDHUP;
                }
				else

#else

			if ( ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) )
			{

#endif
                {
                    //SDFLOG_DEBUG( ( B,"found EPOLLERR for event id(%d) and sock(%d)", i, events[i].data.fd) );
                    //value->cur_event &= EPOLLERR;
                }
                if (value->close_callback && _callback_ret)
                    _callback_ret = value->close_callback(poll_event, value, events[i] , value->fd , value->data );
            }
        }
        else // not in table
        {
             SDFLOG_DEBUG( ( B,"Not allocate event id(%d) and sock(%d)", i, events[i].data.fd) );
        }
    } // for
    return event_count;
}

void epoll_events_set_option( epoll_event_element_t * elem,int option,... )
{
	va_list args_list;
	va_start(args_list,option);
	
	switch (option)
	{
		case BASE_EPOLL_SET_READ_CALLBACK :
			{
				elem->read_callback = va_arg(args_list,void *);
				break;
			}
		case BASE_EPOLL_SET_WRITE_CALLBACK :
			{
				elem->write_callback = va_arg(args_list,void *);
				break;
			}
		case BASE_EPOLL_SET_CLOSE_CALLBACK :
			{
				elem->close_callback = va_arg(args_list,void *);
				break;
			}
		case BASE_EPOLL_SET_USER_DATA :
			{
				elem->data = va_arg(args_list,void *);
				break;
			}
	}
	va_end(args_list);
}
