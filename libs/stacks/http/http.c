#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>
#include <grp.h>


#include "http.h"
#include "sdflib.h"
#include "linkedlist.h"
#include "urlencode.h"

#define ADJUST_SIZE(_l, _s)			\
{									\
register int _r_ = (_l) % (_s);     \
if (_r_ > 0)                        \
(_l) += (_s) - _r_;                 \
}

extern int errno;
static int http_alloc_buffer(HSDATA **data, int len);
const char * http_method_to_string(int method);

HTTP_t *
HTTP_svr_init(int port, int backlog, int max_session, char *http_version)
{
    HTTP_t *hs = NULL;
    int i;

    hs = (HTTP_t *)malloc(sizeof(*hs));
    if (hs==NULL) return NULL;

    (void)memset(hs, 0, sizeof(*hs));
    hs->port = port;
    hs->backlog = backlog;
    if (max_session < HTTP_MAX_SESSION)
        hs->max_session = max_session;
    else
        hs->max_session = HTTP_MAX_SESSION;
    if (http_version!=NULL) strcpy(hs->header_version, http_version);
	hs->last_session = -1;
	hs->session_count = 0;
    hs->avg_response_time = 0;
    hs->min_response_time = 0;
    hs->max_response_time = 0;
    hs->total_response_time = 0;
    SDF_random (&(hs->version), sizeof(hs->version));
	for (i = 0; i < hs->max_session; ++i)
	{
		hs->session[i].no = i;
		hs->session[i].sock = -1;
		hs->session[i].r_buffer = NULL;
		hs->session[i].w_buffer = NULL;
		hs->session[i].request_uri = (char*)malloc(HTTP_MAX_QUERY_STRING_LEN);
        (void)memset(hs->session[i].request_uri, 0, HTTP_MAX_QUERY_STRING_LEN);
		hs->session[i].http_method = HTTP_METHOD_POST;
		hs->session[i].error_response = 0;
        hs->session[i].block_request = 0;
	}

	return hs;
}

void
HTTP_svr_free(HTTP_t *hs)
{
    int i;
    if (hs==NULL) return;
	for (i = 0; i < hs->max_session; ++i)
	{
		free(hs->session[i].request_uri);
	}
    free(hs);
}

int
HTTP_svr_get_option(HTTP_t *hs, int option, char *output)
{
    int value=-1;
    if (hs==NULL) return -1;
    switch (option)
    {
        case HTTP_OPTION_MAX_SESSION:
            value = hs->max_session;
            break;
        case HTTP_OPTION_PORT:
            value = hs->port;
            break;
        case HTTP_OPTION_LISTENER:
            value = hs->listen_sock;
            break;
        case HTTP_OPTION_SESSION_COUNT:
            value = hs->session_count;
            break;
        case HTTP_OPTION_SENT_COUNT:
            value = (int) hs->sent_msg_count;
            break;
        case HTTP_OPTION_SENT_GET_COUNT:
            value = (int) hs->GET_status.sent_msg_count;
            break;
        case HTTP_OPTION_SENT_PUT_COUNT:
            value = (int) hs->PUT_status.sent_msg_count;
            break;
        case HTTP_OPTION_SENT_POST_COUNT:
            value = (int) hs->POST_status.sent_msg_count;
            break;
        case HTTP_OPTION_SENT_DELETE_COUNT:
            value = (int) hs->DELETE_status.sent_msg_count;
            break;
        case HTTP_OPTION_SENT_METHOD_COUNT:
            value = (int) hs->METHOD_status.sent_msg_count;
            break;
        case HTTP_OPTION_RECV_COUNT:
            value = (int) hs->recv_msg_count;
            break;
        case HTTP_OPTION_RECV_GET_COUNT:
            value = (int) hs->GET_status.recv_msg_count;
            break;
        case HTTP_OPTION_RECV_PUT_COUNT:
            value = (int) hs->PUT_status.recv_msg_count;
            break;
        case HTTP_OPTION_RECV_POST_COUNT:
            value = (int) hs->POST_status.recv_msg_count;
            break;
        case HTTP_OPTION_RECV_DELETE_COUNT:
            value = (int) hs->DELETE_status.recv_msg_count;
            break;
        case HTTP_OPTION_RECV_METHOD_COUNT:
            value = (int) hs->METHOD_status.recv_msg_count;
            break;
        case HTTP_OPTION_ERROR_COUNT:
            value = (int) hs->error_msg_count;
            break;
        case HTTP_OPTION_BUSY_COUNT:
            value = (int) hs->busy_msg_count;
            break;
        case HTTP_OPTION_MAX_RESPONSE:
            value = (int) hs->max_response_time;
            break;
        case HTTP_OPTION_MAX_GET_RESPONSE:
            value = (int) hs->GET_status.max_response_time;
            break;
        case HTTP_OPTION_MAX_PUT_RESPONSE:
            value = (int) hs->PUT_status.max_response_time;
            break;
        case HTTP_OPTION_MAX_POST_RESPONSE:
            value = (int) hs->POST_status.max_response_time;
            break;
        case HTTP_OPTION_MAX_DELETE_RESPONSE:
            value = (int) hs->DELETE_status.max_response_time;
            break;
        case HTTP_OPTION_MAX_METHOD_RESPONSE:
            value = (int) hs->METHOD_status.max_response_time;
            break;
        case HTTP_OPTION_MIN_RESPONSE:
            value = (int) hs->min_response_time;
            break;
        case HTTP_OPTION_MIN_GET_RESPONSE:
            value = (int) hs->GET_status.min_response_time;
            break;
        case HTTP_OPTION_MIN_PUT_RESPONSE:
            value = (int) hs->PUT_status.min_response_time;
            break;
        case HTTP_OPTION_MIN_POST_RESPONSE:
            value = (int) hs->POST_status.min_response_time;
            break;
        case HTTP_OPTION_MIN_DELETE_RESPONSE:
            value = (int) hs->DELETE_status.min_response_time;
            break;
        case HTTP_OPTION_MIN_METHOD_RESPONSE:
            value = (int) hs->METHOD_status.min_response_time;
            break;
        case HTTP_OPTION_AVG_RESPONSE:
            value = (int) hs->avg_response_time;
            break;
        case HTTP_OPTION_AVG_GET_RESPONSE:
            value = (int) hs->GET_status.avg_response_time;
            break;
        case HTTP_OPTION_AVG_PUT_RESPONSE:
            value = (int) hs->PUT_status.avg_response_time;
            break;
        case HTTP_OPTION_AVG_POST_RESPONSE:
            value = (int) hs->POST_status.avg_response_time;
            break;
        case HTTP_OPTION_AVG_DELETE_RESPONSE:
            value = (int) hs->DELETE_status.avg_response_time;
            break;
        case HTTP_OPTION_AVG_METHOD_RESPONSE:
            value = (int) hs->METHOD_status.avg_response_time;
            break;
        default:
            value = -1;
            break;
    }

    return value;
}

static int
set_option (HTTP_t *hs, int option, va_list ap)
{
    int n;

    switch (option)
    {
        case HTTP_OPTION_PORT:
            n = va_arg(ap,int);
            if (n > 0xFFFF) return HTTP_RET_ERR_PARAMETER;
            hs->port = n;
            break;
        case HTTP_OPTION_MAX_SESSION:
            n = va_arg(ap,int);
            if (n > HTTP_MAX_SESSION) return HTTP_RET_ERR_PARAMETER;
            hs->max_session = n;
            break;
        case HTTP_OPTION_IDLE_TIMEOUT:
            n = va_arg(ap,int);
            hs->idle_timeout = n;
            break;
        case HTTP_OPTION_READ_TIMEOUT:
            n = va_arg(ap,int);
            hs->read_timeout = n;
            break;
        case HTTP_OPTION_WRITE_TIMEOUT:
            n = va_arg(ap,int);
            hs->write_timeout = n;
            break;
        case HTTP_OPTION_MAX_WRITE_LENGHT:
            n = va_arg(ap,int);
            hs->max_w_len = n;
            break;
        case HTTP_OPTION_SENT_COUNT:
            n = va_arg(ap,int);
            hs->sent_msg_count = n;
            break;
        case HTTP_OPTION_RECV_COUNT:
            n = va_arg(ap,int);
            hs->recv_msg_count = n;
            break;
        case HTTP_OPTION_ERROR_COUNT:
            n = va_arg(ap,int);
            hs->error_msg_count = n;
            break;
        case HTTP_OPTION_BUSY_COUNT:
            n = va_arg(ap,int);
            hs->busy_msg_count = n;
            break;
        case HTTP_OPTION_SENT_GET_COUNT:
            n = va_arg(ap,int);
            hs->GET_status.sent_msg_count = n;
            break;
        case HTTP_OPTION_SENT_PUT_COUNT:
            n = va_arg(ap,int);
            hs->PUT_status.sent_msg_count = n;
            break;
        case HTTP_OPTION_SENT_POST_COUNT:
            n = va_arg(ap,int);
            hs->POST_status.sent_msg_count = n;
            break;
        case HTTP_OPTION_SENT_DELETE_COUNT:
            n = va_arg(ap,int);
            hs->DELETE_status.sent_msg_count = n;
            break;
        case HTTP_OPTION_SENT_METHOD_COUNT:
            n = va_arg(ap,int);
            hs->METHOD_status.sent_msg_count = n;
            break;
        case HTTP_OPTION_RECV_GET_COUNT:
            n = va_arg(ap,int);
            hs->GET_status.recv_msg_count = n;
            break;
        case HTTP_OPTION_RECV_PUT_COUNT:
            n = va_arg(ap,int);
            hs->PUT_status.recv_msg_count = n;
            break;
        case HTTP_OPTION_RECV_POST_COUNT:
            n = va_arg(ap,int);
            hs->POST_status.recv_msg_count = n;
            break;
        case HTTP_OPTION_RECV_DELETE_COUNT:
            n = va_arg(ap,int);
            hs->DELETE_status.recv_msg_count = n;
            break;
        case HTTP_OPTION_RECV_METHOD_COUNT:
            n = va_arg(ap,int);
            hs->METHOD_status.recv_msg_count = n;
            break;
        case HTTP_OPTION_MAX_RESPONSE:
            n = va_arg(ap,int);
            hs->max_response_time = n;
            break;
        case HTTP_OPTION_MAX_GET_RESPONSE:
            n = va_arg(ap,int);
            hs->GET_status.max_response_time = n;
            break;
        case HTTP_OPTION_MAX_PUT_RESPONSE:
            n = va_arg(ap,int);
            hs->PUT_status.max_response_time = n;
            break;
        case HTTP_OPTION_MAX_POST_RESPONSE:
            n = va_arg(ap,int);
            hs->POST_status.max_response_time = n;
            break;
        case HTTP_OPTION_MAX_DELETE_RESPONSE:
            n = va_arg(ap,int);
            hs->DELETE_status.max_response_time = n;
            break;
        case HTTP_OPTION_MAX_METHOD_RESPONSE:
            n = va_arg(ap,int);
            hs->METHOD_status.max_response_time = n;
            break;
        case HTTP_OPTION_MIN_RESPONSE:
            n = va_arg(ap,int);
            hs->min_response_time = n;
            break;
        case HTTP_OPTION_MIN_GET_RESPONSE:
            n = va_arg(ap,int);
            hs->GET_status.min_response_time = n;
            break;
        case HTTP_OPTION_MIN_PUT_RESPONSE:
            n = va_arg(ap,int);
            hs->PUT_status.min_response_time = n;
            break;
        case HTTP_OPTION_MIN_POST_RESPONSE:
            n = va_arg(ap,int); 
            hs->POST_status.min_response_time = n;
            break;
        case HTTP_OPTION_MIN_DELETE_RESPONSE:
            n = va_arg(ap,int);
            hs->DELETE_status.min_response_time = n;
            break;
        case HTTP_OPTION_MIN_METHOD_RESPONSE:
            n = va_arg(ap,int);
            hs->METHOD_status.min_response_time = n;
            break;
        case HTTP_OPTION_AVG_RESPONSE:
            n = va_arg(ap,int);
            hs->avg_response_time = n;
            break;
        case HTTP_OPTION_AVG_GET_RESPONSE:
            n = va_arg(ap,int);
            hs->GET_status.avg_response_time = n;
            break;
        case HTTP_OPTION_AVG_PUT_RESPONSE:
            n = va_arg(ap,int);
            hs->PUT_status.avg_response_time = n;
            break;
        case HTTP_OPTION_AVG_POST_RESPONSE:
            n = va_arg(ap,int);
            hs->POST_status.avg_response_time = n;
            break;
        case HTTP_OPTION_AVG_DELETE_RESPONSE:
            n = va_arg(ap,int);
            hs->DELETE_status.avg_response_time = n;
            break;
        case HTTP_OPTION_AVG_METHOD_RESPONSE:
            n = va_arg(ap,int);
            hs->METHOD_status.avg_response_time = n;
            break;
        default :
            return HTTP_RET_ERR_PARAMETER;
    }
    return HTTP_RET_OK;
}

int
HTTP_svr_set_option (HTTP_t *hs, int option, ...)
{
    int r;
    va_list ap;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
    va_start(ap,option);
    r = set_option (hs, option, ap);
    va_end(ap);
    
    return r;
}

int
HTTP_svr_open(HTTP_t *hs, int *sock, char *error)
{
	socklen_t len;
	struct sockaddr_in addr;
	int sk, v;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
 	sk = socket(AF_INET, SOCK_STREAM, 0);
	if (sk < 0)
	{
		if (error!=NULL) sprintf(error, "socket return eror [%s]", strerror(errno));
		return HTTP_RET_ERR_CONNECT;
	}

	v = 1;
	len = sizeof(v);
	if (setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &v, len) != 0)
	{
		if (error!=NULL) sprintf(error, "setsockopt return eror [%s]", strerror(errno));
		close(sk);
		return HTTP_RET_ERR_CONNECT;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(hs->port);
	addr.sin_addr.s_addr = htonl(0);
	len = sizeof(addr);
	if (bind(sk, (struct sockaddr *)&addr, len) != 0)
	{
        if (error!=NULL) sprintf(error, "bind to port (%d) return eror [%s]", hs->port, strerror(errno));
		close(sk);
		return HTTP_RET_ERR_CONNECT;
	}

	if (listen(sk, hs->backlog) != 0)
	{
        if (error!=NULL) sprintf(error, "listen return eror [%s]", strerror(errno));
		close(sk);
		return HTTP_RET_ERR_CONNECT;
	}
    hs->listen_sock = sk;
    if (sock!=NULL) *sock = sk;
    return HTTP_RET_OK;
}

static int
HTTP_svr_close_listener(HTTP_t *hs, int (*fd_clear_callback)(int), char *error)
{
	int ret = HTTP_RET_OK;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
	if (hs->listen_sock >= 0)
	{
        if (fd_clear_callback!=NULL) fd_clear_callback(hs->listen_sock);
		if (close(hs->listen_sock) != 0)
		{
            if (error!=NULL) sprintf(error, "close listen socket return error [%s]", strerror(errno));
			ret = -1;
		}
		hs->listen_sock = -1;
	}
	return ret;
}

static void
HTTP_flush_read_buffer(int sock)
{
	//!---FLUSH!!!!!!!!!!!!! with select awkward :).
	//for (;;)
	//{
	//	tv.tv_sec = 0;
	//	tv.tv_usec = 0;
	//	FD_ZERO(&rfd);
	//	FD_SET(sock, &rfd);
	//	r = select(sock + 1, &rfd, NULL, NULL, &tv);
	//	if (r <= 0)
	//	break;
	//	if (recv(sock, buff, sizeof(buff), 0) <= 0)
	//	break;
	//}
}

int
HTTP_svr_close(HTTP_t *hs, int (*fd_clear_callback)(int), char *error)
{
	int i, ret = HTTP_RET_OK;
	HSDATA *wbuff;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
	for (i = 0; i <= hs->last_session; ++i)
	{
		if (hs->session[i].r_buffer != NULL)
		{
            free(hs->session[i].r_buffer);
			hs->session[i].r_buffer = NULL;
		}
		while ((wbuff = hs->session[i].w_buffer) != NULL)
		{
			LINKEDLIST_REMOVE(hs->session[i].w_buffer, wbuff);
			free(wbuff);
		}
		if (hs->session[i].sock >= 0)
		{
            if (fd_clear_callback!=NULL) fd_clear_callback(hs->session[i].sock);
			shutdown(hs->session[i].sock, SHUT_WR);
			HTTP_flush_read_buffer(hs->session[i].sock);
			if (close (hs->session[i].sock) != 0)
			{
                if (error!=NULL) sprintf(error, "close socket return error [%s]", strerror(errno));
				ret = -1;
			}
			hs->session[i].sock = -1;
		}
	}
	hs->last_session = -1;
	hs->session_count = 0;
	if (HTTP_svr_close_listener(hs, fd_clear_callback, error) != 0)
		ret = -1;

	return ret;
}

int
HTTP_sess_open(HTTP_t *hs, HOST_EQUIV *authen, HSSESSION **sess, char *error)
{
	int i, sk;
	socklen_t len;
	struct sockaddr_in addr;
    struct hostent *shostl;
    char *remote_address;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
	for (i = 0; i < hs->max_session; ++i)
	{
		if (hs->session[i].sock < 0)
			break;
	}
	if (i >= hs->max_session)
	{
        if (error!=NULL) sprintf(error, "too many sessions");
		return HTTP_RET_MAX_SESSION;
	}
    
accept_again:
	len = sizeof(addr);
	sk = accept(hs->listen_sock, (struct sockaddr *)(&addr), &len);
	if (sk < 0)
	{
		if (errno == EINTR)
			goto accept_again;
#ifdef EPROTO
		if (errno == EPROTO)
			return 0;
#endif
#ifdef ECONNABORTED
		if (errno == ECONNABORTED)
			return 0;
#endif
        if (error!=NULL) sprintf(error, "accept return error [%s]", strerror(errno));
		return HTTP_RET_ERR_CONNECT;
	}

    shostl = gethostbyaddr((char *)&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);
    if(shostl)
    {
        remote_address = shostl->h_name;
    }
    else
    {
        remote_address = inet_ntoa(addr.sin_addr);
        if (error!=NULL) sprintf(error, "can not resolve hostname for ip %s", remote_address);
    }
    if(rauthen_verify(authen, addr.sin_addr.s_addr, remote_address, error)!=RAUTHEN_PASS)
    {
        if (error!=NULL) sprintf(error, "deny remote host[%s]", remote_address);
        close(sk);
        return HTTP_RET_ERR_AUTHEN;
    }

	hs->session[i].sock = sk;
	hs->session[i].version = ++(hs->version);
	http_alloc_buffer(&(hs->session[i].r_buffer), HTTP_DATA_BUFFER);
	hs->session[i].w_buffer = NULL;
	hs->session[i].w_curr = 0;
	hs->session[i].active_time = time(NULL);
	hs->session[i].read_time = 0;
	hs->session[i].write_time = 0;
	hs->session[i].header_length = -1;
	hs->session[i].body_length = -1;
	strcpy(hs->session[i].remote_ip, inet_ntoa(addr.sin_addr));
	hs->session[i].remote_port = addr.sin_port;
	if( hs->session[i].request_uri != NULL )
		hs->session[i].request_uri[0] = 0;
	hs->session[i].http_method = HTTP_METHOD_POST;
	hs->session[i].error_response = 0;
    hs->session[i].block_request = 0;
    if (sess!=NULL) *sess = &(hs->session[i]);

	if (hs->last_session < i)
	{
		hs->last_session = i;
	}
	++(hs->session_count);
	return HTTP_RET_OK;
}

int
HTTP_sess_close(HTTP_t *hs, int no, int (*fd_clear_callback)(int), char *error)
{
	int ret=HTTP_RET_OK;
    HSSESSION *sess=NULL;
	HSDATA *wbuff;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
	if (hs->last_session == no)
	{
		while (--(hs->last_session) >= 0 && hs->session[hs->last_session].sock < 0)
			;
	}
    sess = &(hs->session[no]);
    if (sess->sock < 0)
    {
        if (error!=NULL) sprintf(error, "connection not used (%d)", no);
        return HTTP_RET_ERR_UNAVAILABLE;
    }

	if (sess->r_buffer != NULL)
	{
		free(sess->r_buffer);
		sess->r_buffer = NULL;
	}
	while ((wbuff = sess->w_buffer) != NULL)
	{
		LINKEDLIST_REMOVE(sess->w_buffer, wbuff)
		free(wbuff);
	}

    //!--reset block request.
    sess->block_request = 0;

	if (sess->sock >= 0)
	{
        if (fd_clear_callback!=NULL) fd_clear_callback(sess->sock);
		shutdown(sess->sock, SHUT_WR);
		HTTP_flush_read_buffer(sess->sock);
		if (close(sess->sock) != 0)
		{
            if (error!=NULL) sprintf(error, "close socket return error [%s]", strerror(errno));
			ret = HTTP_RET_ERR_UNKNOWN;
		}
		sess->sock = -1;
		--(hs->session_count);
	}
	return ret;
}

int
HTTP_sess_read(HTTP_t *hs, HSSESSION *sess, char *error)
{
	int n,r;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
	if (sess->header_length + sess->body_length > sess->r_buffer->len)
	{
		if (http_alloc_buffer(&(sess->r_buffer), sess->header_length + sess->body_length) != 0)
		{
			return HTTP_RET_ERR_RECEIVE;
		}
	}
	n = sess->r_buffer->size - sess->r_buffer->len;
	r = (int) recv (sess->sock, sess->r_buffer->data + sess->r_buffer->len, n, 0);

	if (r < 0)
	{
		return HTTP_RET_ERR_READ;
	}
	if (r == 0)
	{
		return HTTP_RET_ERR_CLOSE;
	}

    sess->recv_count++;
	sess->r_buffer->len += r;
	sess->active_time = sess->read_time = time(NULL);
	return HTTP_RET_OK;
}


int
HTTP_sess_write(HTTP_t *hs, HSSESSION *sess, int (*fd_set_write_callback)(int), char *error)
{
	int n, r;
	HSDATA *wbuff;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
	if (sess->w_buffer == NULL)
	{
		sess->write_time = 0;
		return 0;
	}
	n = sess->w_buffer->len - sess->w_curr;
	if (hs->max_w_len > 0 && n > hs->max_w_len)
		n = hs->max_w_len;
	r = (int) send (sess->sock, sess->w_buffer->data + sess->w_curr, n, 0);
	if (r < 0)
	{
        if (error!=NULL) sprintf(error, "session(%d), send return error [%s]", sess->no, strerror(errno));
		return HTTP_RET_ERR_SEND;
	}
	sess->w_curr += r;
	sess->active_time = sess->write_time = time(NULL);
    ++(hs->sent_msg_count);
	if (sess->w_curr >= sess->w_buffer->len)
	{
		sess->w_curr = 0;
		wbuff = sess->w_buffer;
		LINKEDLIST_REMOVE(sess->w_buffer, wbuff)
		SDF_FREE(wbuff);
		if (sess->w_buffer == NULL)
		{
			sess->write_time = 0;
		}

		// Close connection immediately in case synchronous blocking
		if (hs->idle_timeout == 0) return HTTP_RET_SENT_CLOSE;
        return HTTP_RET_SENT;
	}
    else
	{
        if (fd_set_write_callback!=NULL)
        {
            if(fd_set_write_callback(sess->sock) != 0)
            {
                if (error!=NULL) sprintf(error, "fd_set_write error for socket (%d)", sess->sock);
                return HTTP_RET_ERR_SEND;
            }
        }
        sess->write_count++;
        return HTTP_RET_SENT_FRAGMENT;
	}

	return HTTP_RET_OK;
}


int
HTTP_sess_decode(HTTP_t *hs, HSSESSION *sess, char *error)
{
	char *p, *pp;
	int i;
	long ll;

    if (hs==NULL) return HTTP_RET_ERR_PARAMETER;
	if (sess->r_buffer->len <= 0) return HTTP_RET_WAIT_MORE_DATA;

	//decode header
	if (sess->header_length < 0)
	{
		//skip extra CRLF
		for (i = 0; i < sess->r_buffer->len; ++i)
		{
			if (sess->r_buffer->data[i] != '\r' && sess->r_buffer->data[i] != '\n')
				break;
		}
		if (i > 0)
		{
			if (i >= sess->r_buffer->len)   //have only \r or \n
			{
				sess->r_buffer->len = 0;
				sess->read_time = 0;
				return HTTP_RET_WAIT_MORE_DATA;
			}
			memmove(sess->r_buffer->data, sess->r_buffer->data + i, sess->r_buffer->len - i);
			sess->r_buffer->len -= i;
			sess->r_buffer->data[sess->r_buffer->len] = '\0';   // pad with '\0'
		}
		if (sess->r_buffer->len <= 0) return HTTP_RET_WAIT_MORE_DATA;
		if ((p = strstr(sess->r_buffer->data, "\r\n\r\n")) == NULL)
		{
			if (sess->r_buffer->len >= HTTP_MAX_HEADER_SIZE)
			{
                if (error!=NULL) sprintf(error, "session(%d), HTTP header is too big", sess->no);
				return HTTP_RET_ERR_DECODE;
			}
			return HTTP_RET_WAIT_MORE_DATA;
		}
		p += 2;
		p[0] = '\0';	//separate header string from body
		p += 2;
        sess->header_length = (int) (p - sess->r_buffer->data);

		//!--Error trap check header type.
		if( sess->header_length < 10 )
		{
            if (error!=NULL) sprintf(error, "session(%d), HTTP header is too small", sess->no);
			return HTTP_RET_ERR_DECODE;
		}

		//!--Reduce strstr() CPU spin.
		//!--Backup char.
#define __BACK_CHAR_IDX__ 9
		char _backup_char = sess->r_buffer->data[__BACK_CHAR_IDX__]; //!--Fixed 9 length.
		sess->r_buffer->data[__BACK_CHAR_IDX__] = 0;

		char * _http_method_get_pch			= strstr( sess->r_buffer->data, "GET " );
		char * _http_method_post_pch		= strstr( sess->r_buffer->data, "POST " );
		char * _http_method_put_pch			= strstr( sess->r_buffer->data, "PUT " );
		char * _http_method_delete_pch		= strstr( sess->r_buffer->data, "DELETE " );
		char * _http_method_trace_pch		= strstr( sess->r_buffer->data, "TRACE " );
		char * _http_method_head_pch		= strstr( sess->r_buffer->data, "HEAD " );
        char * _http_method_options_pch     = strstr( sess->r_buffer->data, "OPTIONS ");
        char * _http_method_connect_pch     = strstr( sess->r_buffer->data, "CONNECT ");

		char * _http_method_pch				= NULL;
		char _http_method_len				= 4;
		//!--Restore.
		sess->r_buffer->data[__BACK_CHAR_IDX__] = _backup_char;

		//!--Reset on new request.
		sess->error_response	= 0;

		if( _http_method_get_pch == sess->r_buffer->data )
		{
			_http_method_pch	= _http_method_get_pch;
			sess->http_method	= HTTP_METHOD_GET;
            ++(hs->GET_status.recv_msg_count);
		}
		else if( _http_method_trace_pch == sess->r_buffer->data )
		{
			_http_method_len	= 6;
			_http_method_pch	= _http_method_trace_pch;
			sess->http_method	= HTTP_METHOD_TRACE;
		}
		else if( _http_method_head_pch == sess->r_buffer->data )
		{
			_http_method_len	= 5;
			_http_method_pch	= _http_method_head_pch;
			sess->http_method	= HTTP_METHOD_HEAD;
		}
		else if( _http_method_delete_pch == sess->r_buffer->data )
		{
			_http_method_len	= 7;
			_http_method_pch	= _http_method_delete_pch;
			sess->http_method	= HTTP_METHOD_DELETE;
            ++(hs->DELETE_status.recv_msg_count);
		}
		else if( _http_method_post_pch == sess->r_buffer->data )
		{
			_http_method_len	= 5;
			_http_method_pch	= _http_method_post_pch;
			sess->http_method	= HTTP_METHOD_POST;
            ++(hs->POST_status.recv_msg_count);
		}
		else if( _http_method_put_pch == sess->r_buffer->data )
		{
			_http_method_pch	= _http_method_put_pch;
			sess->http_method	= HTTP_METHOD_PUT;
            ++(hs->PUT_status.recv_msg_count);
		}
        else if (_http_method_options_pch == sess->r_buffer->data)
        {
			_http_method_len	= 8;
            _http_method_pch    = _http_method_options_pch;
            sess->http_method   = HTTP_METHOD_OPTIONS;
        }
        else if (_http_method_connect_pch == sess->r_buffer->data)
        {
			_http_method_len	= 8;
            _http_method_pch    = _http_method_connect_pch;
            sess->http_method   = HTTP_METHOD_CONNECT;
        }
		else
		{
			//!--501 Method Not Implemented.
            if (error!=NULL) sprintf(error, "501 Method Not Implemented: The request method used is invalid (could be caused by a typing error). [session_no:%d]", sess->no);
            sess->r_buffer->data[sess->header_length] = 0;
			sess->error_response = 501;
            ++(hs->error_msg_count);
			return HTTP_RET_ERROR_AVAILABLE;
		}

		_http_method_pch += _http_method_len;
		char *_http_version_pch = strstr( _http_method_pch, " HTTP/1." ) ; //!-- 15-07-2013 just check only 1.0 or 1.1.
		if(_http_version_pch == NULL || _http_version_pch < _http_method_pch)
		{
			//!--Unreachable-> 400 Bad Request.
            if (error!=NULL) sprintf(error, "400 Bad Request: Server could not interpret or understand the request, probably syntax error in the request message. [session_no:%d]", sess->no);
            sess->r_buffer->data[sess->header_length] = 0;
			sess->error_response = 400;
            ++(hs->error_msg_count);
			return HTTP_RET_ERROR_AVAILABLE;
		}
        memcpy(sess->header_version, _http_version_pch+1, 8);

		//!--Get query string
		//!!--Set default when not found Uri.
		if( sess->request_uri != NULL )
		{
			strcpy( sess->request_uri , "/" );
		}
        int _request_uri_len = (int)(_http_version_pch - _http_method_pch);
		//!-Handled from "/"
		if( _request_uri_len >= 1 && _request_uri_len  < HTTP_MAX_QUERY_STRING_LEN )
		{
			if( sess->request_uri != NULL )
			{
                char token = _http_method_pch[_request_uri_len];
                _http_method_pch[_request_uri_len] = 0;
                char *uri_decode = url_decode_init(_http_method_pch);
                if(uri_decode){
                    strcpy( sess->request_uri , uri_decode);
                    url_decode_free(uri_decode);
                }else{
                    /* Compatible old code */
                    strncpy(sess->request_uri, _http_method_pch,_request_uri_len);
                    sess->request_uri[_request_uri_len] = 0;
                }
                _http_method_pch[_request_uri_len] = token;
				//!--Return with no body ,excepted PUT/POST.
                if (sess->http_method != HTTP_METHOD_PUT && sess->http_method != HTTP_METHOD_POST)
				{
					//!--Return with body length 0
					sess->body_length = (int)0;
                    ++(hs->recv_msg_count);
					return HTTP_RET_DATA_AVAILABLE;
				}
			}
			else
			{
                if (error!=NULL) sprintf(error, "allocate memory for querystring failed. [session_no:%d]", sess->no);
				return HTTP_RET_ERR_DECODE;
			}
		}
		else
		{
			//!--Unreachable-> 400 Bad Request.
            if (error!=NULL) sprintf(error, "400 Bad Request: Server could not interpret or understand the request, probably syntax error in the request message. [session_no:%d]", sess->no);
			sess->error_response = 400;
            ++(hs->error_msg_count);
			return HTTP_RET_ERROR_AVAILABLE;
		}

		//!--Method POST/PUT no request data.
		if ((p = strstr(sess->r_buffer->data, HTTP_CONTENT_LENGTH ":")) == NULL)
		{
            if (error!=NULL) sprintf(error, "session(%d), HTTP " HTTP_CONTENT_LENGTH ": not found - Data[%.*s...] ", sess->no , 4096 , sess->r_buffer->data);
			sess->body_length = (int)0;
			return HTTP_RET_ERR_DECODE;
		}

		pp = p + sizeof(HTTP_CONTENT_LENGTH ":") - 1;
		if (*pp == ' ')
			++pp;
		ll = strtol(pp, &pp, 10);
		if (ll == LONG_MAX || !(pp[0] == '\r' && pp[1] == '\n'))
		{
            if (error!=NULL) sprintf(error, "session(%d), Invalid [%s]", sess->no, p);
			return HTTP_RET_ERR_DECODE;
		}
		sess->body_length = (int)ll;
	}
    
	//continue read(wait for body)
	if (sess->r_buffer->len < (sess->header_length + sess->body_length))
		return HTTP_RET_WAIT_MORE_DATA;

    ++(hs->recv_msg_count);
	return HTTP_RET_DATA_AVAILABLE;
}

int
HTTP_verify_connection(HTTP_t *hs, int (*fd_clear_callback)(int), char *error)
{
#define HS_IS_SESSION_IDLE(_sess)   \
(                                   \
    (_sess)->w_buffer == NULL &&    \
    (_sess)->header_length < 0 &&   \
    ((_sess)->r_buffer == NULL ||   \
    (_sess)->r_buffer->len <= 0)    \
)
    int i;
	time_t ct = time(NULL);
    HSSESSION *sess=NULL;

	for (i = 0; i <= hs->last_session; ++i)
	{
		if (hs->session[i].sock >= 0)
		{
			sess = &(hs->session[i]);
			if (sess->read_time > 0 && hs->read_timeout > 0 && (ct - sess->read_time) >= hs->read_timeout)
			{
                HTTP_sess_close(hs, sess->no, fd_clear_callback, error);
                if (error!=NULL) sprintf(error, "session(%d), HTTP server read timeout (disconnected)", i);
				return HTTP_RET_TIMEOUT;
			}
			if (sess->write_time > 0 && hs->write_timeout > 0 && (ct - sess->write_time) >= hs->write_timeout)
			{
                HTTP_sess_close(hs, sess->no, fd_clear_callback, error);
                if (error!=NULL) sprintf(error, "session(%d), HTTP server write timeout (disconnected)", i);
				return HTTP_RET_TIMEOUT;
			}
			if (HS_IS_SESSION_IDLE(sess) && hs->idle_timeout > 0 && (ct - sess->active_time) >= hs->idle_timeout)
			{
                HTTP_sess_close(hs, sess->no, fd_clear_callback, error);
                if (error!=NULL) sprintf(error, "session(%d), HTTP server idle timeout (disconnected)", i);
				return HTTP_RET_TIMEOUT;
			}
		}
	}

    return HTTP_RET_OK;
}

static int
http_alloc_buffer(HSDATA **data, int len)
{
	if (*data == NULL)
	{
		ADJUST_SIZE(len, HTTP_DATA_BUFFER)
		*data = (HSDATA*)malloc(sizeof(HSDATA) + len);
		if (*data == NULL) return -1;
		(void)memset((*data), 0, sizeof(HSDATA)+len);
		(*data)->size = len;
		(*data)->len = 0;
	}
	else if (((*data)->len + len) > (*data)->size)
	{
		HSDATA *x;
		len += (*data)->len;
		ADJUST_SIZE(len, HTTP_DATA_BUFFER)
		x = (HSDATA*)realloc((*data), sizeof(HSDATA) + len);
		if (x == NULL) return -1;
		x->size = len;
		*data = x;
		(void)memset((*data)->data+(*data)->len, 0, (*data)->size-(*data)->len);
	}
	return 0;
}


#define _HGH_STORE_STRING_CONST(_str)                                   \
{																		\
if (http_alloc_buffer(data, sizeof((_str)) - 1) != 0)                   \
{																		\
    return -1;															\
}																		\
memcpy((*data)->data + (*data)->len, (_str), sizeof((_str)) - 1);		\
(*data)->len += sizeof((_str)) - 1;										\
}

#define _HGH_STORE_STRING(_str)                                         \
{                                                                       \
int _l_ = (int)strlen((_str));                                          \
if (http_alloc_buffer (data, _l_) != 0)                                 \
{                                                                       \
    return -1;                                                          \
}                                                                       \
memcpy((*data)->data + (*data)->len, (_str), _l_);                      \
(*data)->len += _l_;                                                    \
}

int
http_gen_success_header(HSSESSION *sess, HSDATA **data)
{
    static char version[] = "Server: Damocles/" SDF_VERSION;
    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" 200 OK\r\n")
    _HGH_STORE_STRING(version)
    _HGH_STORE_STRING_CONST("\r\n")
    _HGH_STORE_STRING_CONST("Content-Type: application/json\r\n")
    _HGH_STORE_STRING_CONST("Connection: close\r\n")

	return 0;
}

int
http_gen_created_header(HSSESSION *sess, HSDATA **data)
{
    static char version[] = "Server: Damocles/" SDF_VERSION;
    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" 201 Created\r\n")
    _HGH_STORE_STRING(version)
    _HGH_STORE_STRING_CONST("\r\n")
    _HGH_STORE_STRING_CONST("Content-Type: application/json\r\n")
    _HGH_STORE_STRING_CONST("Connection: close\r\n")

	return 0;
}

int
http_gen_forbidden_page(HSSESSION *sess, HSDATA **data)
{
    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" 403 Forbidden\r\n\r\n")
	return 0;
}

/*int
 http_gen_url_too_long_page(HSSESSION *sess, HSDATA **data)
 {
 _HGH_STORE_STRING(sess->header_version)
 _HGH_STORE_STRING_CONST(" 414 Request-url Too Long\r\n")
 return 0;
 }*/

int
http_gen_internal_error_page(HSSESSION *sess, HSDATA **data)
{
    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" 500 Internal Server Error\r\n\r\n")
    return 0;
}

int
http_gen_not_implemented_page(HSSESSION *sess, HSDATA **data)
{
    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" 501 Method Not Implemented\r\n\r\n")
    return 0;
}

int
http_gen_busy_page(HSSESSION *sess, HSDATA **data)
{
    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" 503 Service Unavailable\r\n\r\n")
	return 0;
}

int
http_gen_gateway_timeout_page(HSSESSION *sess, HSDATA **data)
{
    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" 504 Gateway Timeout\r\n\r\n")
    return 0;
}

static void
http_status_to_string(int status, char *buff)
{
	switch (status)
	{
		case 100:
			strcpy(buff, "100 Continue");
            break;
		case 101:
			strcpy(buff, "101 Switching Protocols");
            break;
		case 300:
			strcpy(buff, "300 Multiple Choices");
            break;
		case 301:
			strcpy(buff, "301 Moved Permanently");
            break;
		case 302:
			strcpy(buff, "302 Found");
            break;
		case 303:
			strcpy(buff, "303 See Other");
            break;
		case 304:
			strcpy(buff, "304 Not Modified");
            break;
		case 305:
			strcpy(buff, "305 Use Proxy");
            break;
		case 306:
			strcpy(buff, "306 Unused");
            break;
		case 307:
			strcpy(buff, "307 Temporary Redirect");
            break;
		case 400:
			strcpy(buff, "400 Bad Request");
            break;
		case 401:
			strcpy(buff, "401 Unauthorized");
            break;
		case 402:
			strcpy(buff, "402 Payment Required");
            break;
		case 403:
			strcpy(buff, "403 Forbidden");
            break;
		case 404:
			strcpy(buff, "404 Not Found");
            break;
		case 405:
			strcpy(buff, "405 Method Not Allowed");
            break;
		case 406:
			strcpy(buff, "406 Not Acceptable");
            break;
		case 407:
			strcpy(buff, "407 Proxy Authentication Required");
            break;
		case 408:
			strcpy(buff, "408 Request Timeout");
            break;
		case 409:
			strcpy(buff, "409 Conflict");
            break;
		case 410:
			strcpy(buff, "410 Gone");
            break;
		case 411:
			strcpy(buff, "411 Length Required");
            break;
		case 412:
			strcpy(buff, "412 Precondition Failed");
            break;
		case 413:
			strcpy(buff, "413 Request Entity Too Large");
            break;
		case 414:
			strcpy(buff, "414 Request-url Too Long");
            break;
		case 415:
			strcpy(buff, "415 Unsupported Media Type");
            break;
		case 417:
			strcpy(buff, "417 Expectation Failed");
            break;
		case 500:
			strcpy(buff, "500 Internal Server Error");
            break;
		case 501:
			strcpy(buff, "501 Not Implemented");
            break;
		case 502:
			strcpy(buff, "502 Bad Gateway");
            break;
		case 503:
			strcpy(buff, "503 Service Unavailable");
            break;
		case 504:
			strcpy(buff, "504 Gateway Timeout");
            break;
		case 505:
			strcpy(buff, "505 HTTP Version Not Supported");
            break;
		default:
			sprintf(buff, "%d", status);
            break;
	}
}

int
http_gen_error_page(HSSESSION *sess, HSDATA **data, int status, const char *desc)
{
	char buff[1028], c_len[32];
    static char version[] = "Server: Damocles/" SDF_VERSION;

    _HGH_STORE_STRING(sess->header_version)
	_HGH_STORE_STRING_CONST(" ")

    if (desc == NULL)
    {
        http_status_to_string(status, buff);
        sprintf(c_len, "%d", (int)strlen(buff));
        _HGH_STORE_STRING(buff)
    }
    else
    {
        sprintf(buff, "%d %s", status, desc);
        sprintf(c_len, "%d", (int)strlen(buff));
        _HGH_STORE_STRING(buff)
    }
	
	_HGH_STORE_STRING_CONST("\r\n")
    _HGH_STORE_STRING(version)
    _HGH_STORE_STRING_CONST("\r\n")
    _HGH_STORE_STRING_CONST("Content-Type: text/plain; charset=utf-8\r\n")
    _HGH_STORE_STRING_CONST("Connection: close\r\n")
    
	return 0;
}

const char * http_method_to_string( int method){
    switch( method ){
        case HTTP_METHOD_GET:
            return "GET";
        case HTTP_METHOD_POST:
            return "POST";
        case HTTP_METHOD_PUT:
            return "PUT";
        case HTTP_METHOD_DELETE:
            return "DELETE";
        default :
            return "UNKNOWN";
    }
}
