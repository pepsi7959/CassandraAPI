#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>


#include "ldap.h"
#include "ldap_common.h"
#include "sdflib.h"
#include "linkedlist.h"

DS_t *
LDAP_svr_init(const char *dn, int port, int backlog, int max_session, int max_concurrent)
{
    DS_t *ds = NULL;
    int i;

    ds = (DS_t *)malloc(sizeof(*ds));
    if (ds==NULL) return NULL;

    (void)memset(ds, 0, sizeof(*ds));
    if (dn!=NULL) strncpy(ds->baseDN, dn, sizeof(ds->baseDN)-1);
    ds->user_list = NULL;
    ds->port = port;
    ds->backlog = backlog;
    if (max_session < LDAP_MAX_SESSION)
        ds->max_session = max_session;
    else
        ds->max_session = LDAP_MAX_SESSION;
    if (max_concurrent < LDAP_MAX_CONCURRENT_REQUEST)
        ds->max_concurrent = max_concurrent;
    else
        ds->max_concurrent = LDAP_MAX_CONCURRENT_REQUEST;
    ds->last_session = -1;
    ds->session_count = 0;
    ds->avg_response_time = 0;
    ds->min_response_time = 0;
    ds->max_response_time = 0;
    ds->total_response_time = 0;
    SDF_random (&(ds->version), sizeof(ds->version));
    for (i = 0; i < ds->max_session; ++i)
    {
        ds->session[i].no = i;
        ds->session[i].sock = -1;
        ds->session[i].r_buffer = NULL;
        ds->session[i].w_buffer = NULL;
        ds->session[i].error_response = 0;
    }

    return ds;
}

void
LDAP_svr_free(DS_t *ds)
{
    DSUSER *user=NULL;
    if (ds==NULL) return;
    while ((user=ds->user_list)!=NULL) {
        LINKEDLIST_REMOVE(ds->user_list, user);
        free(user);
    }
    free(ds);
}

int LDPA_svr_user_add (DS_t *ds, const char *username, const char *password)
{
    DSUSER *user=NULL;
    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    user = (DSUSER *) malloc (sizeof(*user));
    if (user==NULL) return LDAP_RET_ERR_MEMORY;
    (void) memset(user, 0, sizeof(*user));

    if (username!=NULL) strncpy(user->name, username, sizeof(user->name)-1);
    if (password!=NULL) strncpy(user->password, password, sizeof(user->password)-1);

    LINKEDLIST_APPEND(ds->user_list, user);

    return LDAP_RET_OK;
}

int
LDAP_svr_get_option(DS_t *ds, int option, char *output)
{
    int value=-1;
    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    switch (option)
    {
        case LDAP_SERV_OPTION_MAX_SESSION:
            value = ds->max_session;
            break;
        case LDAP_SERV_OPTION_PORT:
            value = ds->port;
            break;
        case LDAP_SERV_OPTION_LISTENER:
            value = ds->listen_sock;
            break;
        case LDAP_SERV_OPTION_SESSION_COUNT:
            value = ds->session_count;
            break;
        case LDAP_SERV_OPTION_SENT_COUNT:
            value = (int) ds->sent_msg_count;
            break;
        case LDAP_SERV_OPTION_RECV_COUNT:
            value = (int) ds->recv_msg_count;
            break;
        case LDAP_SERV_OPTION_ERROR_COUNT:
            value = (int) ds->error_msg_count;
            break;
        case LDAP_SERV_OPTION_BUSY_COUNT:
            value = (int) ds->busy_msg_count;
            break;
        case LDAP_SERV_OPTION_MAX_RESPONSE:
            value = (int) ds->max_response_time;
            break;
        case LDAP_SERV_OPTION_MIN_RESPONSE:
            value = (int) ds->min_response_time;
            break;
        case LDAP_SERV_OPTION_AVG_RESPONSE:
            value = (int) ds->avg_response_time;
            break;
        default:
            value = -1;
            break;
    }

    return value;
}

static int
set_option (DS_t *ds, int option, va_list ap)
{
    int n;
    switch (option)
    {
        case LDAP_SERV_OPTION_READ_TIMEOUT:
            n = va_arg(ap,int);
            ds->read_timeout = n;
            break;
        case LDAP_SERV_OPTION_WRITE_TIMEOUT:
            n = va_arg(ap,int);
            ds->write_timeout = n;
            break;
        case LDAP_SERV_OPTION_IDLE_TIMEOUT:
            n = va_arg(ap,int);
            ds->idle_timeout = n;
            break;
        case LDAP_SERV_OPTION_MAX_WRITE_LENGHT:
            n = va_arg(ap,int);
            ds->max_w_len = n;
            break;
        default :
            return LDAP_RET_ERR_PARAMETER;
    }
    return LDAP_RET_OK;
}

int
LDAP_svr_set_option (DS_t *ds, int option, ...)
{
    int r;
    va_list ap;
    
    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    va_start(ap,option);
    r = set_option (ds, option, ap);
    va_end(ap);
    
    return r;
}

int
LDAP_svr_open(DS_t *ds, int *sock, char *error)
{
    socklen_t len;
    struct sockaddr_in addr;
    int sk, v;

    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    sk = socket(AF_INET, SOCK_STREAM, 0);
    if (sk < 0)
    {
        if (error!=NULL) sprintf(error, "socket return eror [%s]", strerror(errno));
        return LDAP_RET_ERR_CONNECT;
    }

    v = 1;
    len = sizeof(v);
    if (setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &v, len) != 0)
    {
        if (error!=NULL) sprintf(error, "setsockopt return eror [%s]", strerror(errno));
        close(sk);
        return LDAP_RET_ERR_CONNECT;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ds->port);
    addr.sin_addr.s_addr = htonl(0);
    len = sizeof(addr);
    if (bind(sk, (struct sockaddr *)&addr, len) != 0)
    {
        if (error!=NULL) sprintf(error, "bind to port (%d) return eror [%s]", ds->port, strerror(errno));
        close(sk);
        return LDAP_RET_ERR_CONNECT;
    }

    if (listen(sk, ds->backlog) != 0)
    {
        if (error!=NULL) sprintf(error, "listen return eror [%s]", strerror(errno));
        close(sk);
        return LDAP_RET_ERR_CONNECT;
    }
    ds->listen_sock = sk;
    if (sock!=NULL) *sock = sk;
    return LDAP_RET_OK;
}

static int
ldap_svr_close_listener(DS_t *ds, int (*fd_clear_callback)(int), char *error)
{
    int ret = LDAP_RET_OK;

    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    if (ds->listen_sock >= 0)
    {
        if (fd_clear_callback!=NULL) fd_clear_callback(ds->listen_sock);
        if (close(ds->listen_sock) != 0)
        {
            if (error!=NULL) sprintf(error, "close listen socket return error [%s]", strerror(errno));
            ret = -1;
        }
        ds->listen_sock = -1;
    }
    return ret;
}

static void
ldap_flush_read_buffer(int sock)
{
}

int
LDAP_svr_close(DS_t *ds, int (*fd_clear_callback)(int), char *error)
{
    int i, ret = LDAP_RET_OK;
//    HSDATA *wbuff;

    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    for (i = 0; i <= ds->last_session; ++i)
    {
        if (ds->session[i].r_buffer != NULL)
        {
            free(ds->session[i].r_buffer);
            ds->session[i].r_buffer = NULL;
        }
        if (ds->session[i].w_buffer != NULL)
        {
            free(ds->session[i].w_buffer);
            ds->session[i].w_buffer = NULL;
        }
/*        while ((wbuff = ds->session[i].w_buffer) != NULL)
        {
            LINKEDLIST_REMOVE(ds->session[i].w_buffer, wbuff);
            free(wbuff);
        }*/
        if (ds->session[i].sock >= 0)
        {
            if (fd_clear_callback!=NULL) fd_clear_callback(ds->session[i].sock);
            shutdown(ds->session[i].sock, SHUT_WR);
            ldap_flush_read_buffer(ds->session[i].sock);
            if (close (ds->session[i].sock) != 0)
            {
                if (error!=NULL) sprintf(error, "close socket return error [%s]", strerror(errno));
                ret = -1;
            }
            ds->session[i].sock = -1;
        }
    }
    ds->last_session = -1;
    ds->session_count = 0;
    if (ldap_svr_close_listener(ds, fd_clear_callback, error) != 0)
        ret = -1;
    
    return ret;
}

int
LDAP_sess_open(DS_t *ds, HOST_EQUIV *authen, DSSESSION **sess, char *error)
{
    int i, sk;
    socklen_t len;
    struct sockaddr_in addr;
    struct hostent *shostl;
    char *remote_address;

    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    for (i = 0; i < ds->max_session; ++i)
    {
        if (ds->session[i].sock < 0)
            break;
    }
    if (i >= ds->max_session)
    {
        if (error!=NULL) sprintf(error, "too many sessions");
        return LDAP_RET_ERR_MAX_SESSION;
    }

accept_again:
    len = sizeof(addr);
    sk = accept(ds->listen_sock, (struct sockaddr *)(&addr), &len);
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
        return LDAP_RET_ERR_CONNECT;
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
        return LDAP_RET_ERR_AUTHEN;
    }

    ds->session[i].sock = sk;
    ds->session[i].version = ++(ds->version);
    ds->session[i].r_buffer = NULL;
    ds->session[i].r_ber_body = -1;
    ds->session[i].r_ber_body_length = -1;
    ds->session[i].w_buffer = NULL;
    ds->session[i].w_curr = 0;
    ds->session[i].active_time = time(NULL);
    ds->session[i].read_time = 0;
    ds->session[i].write_time = 0;
    strcpy(ds->session[i].remote_ip, inet_ntoa(addr.sin_addr));
    ds->session[i].remote_port = addr.sin_port;
    ds->session[i].error_response = 0;
    ds->session[i].state = LDAP_SERV_STATE_OPEN;
    ds->session[i].rdata = NULL;
    ds->session[i].concurrent_count = 0;
    if (sess!=NULL) *sess = &(ds->session[i]);

    if (ds->last_session < i)
    {
        ds->last_session = i;
    }
    ++(ds->session_count);
    return LDAP_RET_OK;
}

int
LDAP_sess_close(DS_t *ds, int no, int (*fd_clear_callback)(int), char *error)
{
    int ret=LDAP_RET_OK;
    DSSESSION *sess=NULL;
//    HSDATA *wbuff;

    if (ds==NULL) return LDAP_RET_ERR_PARAMETER;
    if (ds->last_session == no)
    {
        while (--(ds->last_session) >= 0 && ds->session[ds->last_session].sock < 0)
            ;
    }
    sess = &(ds->session[no]);
    if (sess->sock < 0)
    {
        if (error!=NULL) sprintf(error, "connection not used (%d)", no);
        return LDAP_RET_ERR_UNAVAILABLE;
    }

    if (sess->r_buffer != NULL)
    {
        free(sess->r_buffer);
        sess->r_buffer = NULL;
    }
    if (sess->w_buffer != NULL)
    {
        free(sess->w_buffer);
        sess->w_buffer = NULL;
    }
/*    while ((wbuff = sess->w_buffer) != NULL)
    {
        LINKEDLIST_REMOVE(sess->w_buffer, wbuff)
        free(wbuff);
    }*/

    if (sess->sock >= 0)
    {
        if (fd_clear_callback!=NULL) fd_clear_callback(sess->sock);
        shutdown(sess->sock, SHUT_WR);
        ldap_flush_read_buffer(sess->sock);
        if (close(sess->sock) != 0)
        {
            if (error!=NULL) sprintf(error, "close socket return error [%s]", strerror(errno));
            ret = LDAP_RET_ERR_UNKNOWN;
        }
        sess->sock = -1;
        --(ds->session_count);
    }
    return ret;
}

int
LDAP_sess_examine(DS_t *ds, int (*fd_clear_callback)(int), char *error)
{
#define DS_IS_SESSION_IDLE(_sess)   \
(                                   \
(_sess)->w_buffer == NULL &&        \
(_sess)->r_buffer == NULL           \
)
    int i;
    time_t ct = time(NULL);
    DSSESSION *sess=NULL;

    for (i = 0; i <= ds->last_session; ++i)
    {
        if ((ds->session[i].sock >= 0) && (ds->session[i].state == LDAP_SERV_STATE_READY))
        {
            sess = &(ds->session[i]);
            if (sess->read_time > 0 && ds->read_timeout > 0 && (ct - sess->read_time) >= ds->read_timeout)
            {
                LDAP_sess_close(ds, sess->no, fd_clear_callback, error);
                if (error!=NULL) sprintf(error, "session(%d), HTTP server read timeout (disconnected)", i);
                return LDAP_RET_ERR_IDLE_TIMEOUT;
            }
            if (sess->write_time > 0 && ds->write_timeout > 0 && (ct - sess->write_time) >= ds->write_timeout)
            {
                LDAP_sess_close(ds, sess->no, fd_clear_callback, error);
                if (error!=NULL) sprintf(error, "session(%d), HTTP server write timeout (disconnected)", i);
                return LDAP_RET_ERR_IDLE_TIMEOUT;
            }
            if (DS_IS_SESSION_IDLE(sess) && ds->idle_timeout > 0 && (ct - sess->active_time) >= ds->idle_timeout)
            {
                LDAP_sess_close(ds, sess->no, fd_clear_callback, error);
                if (error!=NULL) sprintf(error, "session(%d), HTTP server idle timeout (disconnected)", i);
                return LDAP_RET_ERR_IDLE_TIMEOUT;
            }
        }
    }
    
    return LDAP_RET_OK;
}

int
LDAP_sess_read(DS_t *ds, DSSESSION *sess, char *error)
{
    int n,r;
    int res;

    if ((ds==NULL)||(sess==NULL))
    {
        if (error!=NULL)
            sprintf(error, "invalid parameters");
        return LDAP_RET_ERR_PARAMETER;
    }
    if (sess->state == LDAP_SERV_STATE_OPEN)
    {
        int e = 0;
        socklen_t elen = sizeof(e);
        if (getsockopt(sess->sock, SOL_SOCKET, SO_ERROR, (char *)&e, &elen) != 0)
        {
            if (error!=NULL)
                sprintf(error, "getsockopt return error [%s]", strerror(errno));
            return LDAP_RET_ERR_CONNECT;
        }
        if (e != 0)
        {
            if (error!=NULL)
                sprintf(error, "connect error [%s]", strerror(e));
            return LDAP_RET_ERR_CONNECT;
        }

        r = ldap_recv_bind_rqt(ds, sess);
        if (r != LDAP_RET_OK)
        {
            if (error!=NULL)
                sprintf(error, "bind request return error (%d)", r);
            if (r == LDAP_RET_ERR_AUTHEN) res = LDAP_RESULT_INVALID_CREDENTIALS;
            else if (r == LDAP_RET_ERR_AUTHEN_METHOD) res = LDAP_RESULT_AUTH_METHOD_NOT_SUPPORTED;
            else  res = LDAP_RESULT_INAPPROPRIATE_AUTHENTICATION;
            ldap_send_bind_resp(ds, sess, res);
            return r;
        }

        r = ldap_send_bind_resp(ds, sess, LDAP_RESULT_SUCCESS);
        if (r != LDAP_RET_OK)
        {
            if (error!=NULL)
                sprintf(error, "can not send bind response");
            return LDAP_RET_ERR_CONNECT;
        }

        sess->active_time = time(NULL);
        sess->state = LDAP_SERV_STATE_READY;

        return LDAP_RET_READY;
    }

    if (sess->r_buffer == NULL)//first time
    {
        sess->r_size = LDAP_READ_BUFFER;
        sess->r_buffer = (char *) malloc (sess->r_size);
        if (sess->r_buffer == NULL)
        {
            if (error!=NULL)
                sprintf(error, "can not allocate memory size (%u)", (unsigned int)sess->r_size);
            return LDAP_RET_ERR_MEMORY;
        }
        sess->r_len = 0;
    }
    n = sess->r_size - 1 - sess->r_len;
    if (n <= 0)
    {
        if (error!=NULL)
            sprintf(error, "read buffer full");
        return LDAP_RET_ERR_MEMORY;
    }
    r = (int) recv (sess->sock, sess->r_buffer + sess->r_len, n, 0);
    if (r < 0)
    {
        if (error!=NULL)
            sprintf(error, "recv return error [%s]", strerror(errno));
        return LDAP_RET_ERR_CONNECT;
    }
    if (r == 0)
    {
        if (error!=NULL)
            sprintf(error, "socket has been closed");
        return LDAP_RET_ERR_CLOSED;
    }
    sess->r_len += r;

    ++(ds->recv_msg_count);
    ++(sess->recv_count);
    sess->active_time = sess->read_time = time(NULL);
    return LDAP_RET_OK;
}

int
LDAP_sess_write(DS_t *ds, DSSESSION *sess, int (*fd_set_write_callback)(int), char *error)
{
    int n, r;

    if ((ds==NULL)||(sess==NULL))
    {
        if (error!=NULL)
            sprintf(error, "invalid parameters");
        return LDAP_RET_ERR_PARAMETER;
    }

    if (sess->state == LDAP_SERV_STATE_OPEN)
    {
        if (error!=NULL)
            sprintf(error, "connection isn't ready");
        return LDAP_RET_ERR_CONNECT;
    }

    if ((sess->w_buffer == NULL)||(sess->w_len==0)) return LDAP_RET_OK;

    // Limit write buffer
    n = sess->w_len - sess->w_curr;
    if (n > LDAP_WRITE_BUFFER) n = LDAP_WRITE_BUFFER;

    r = (int) send (sess->sock, sess->w_buffer + sess->w_curr, n, 0);
    if (r < 0)
    {
        if (error!=NULL)
            sprintf(error, "send return error [%s]", strerror(errno));
        return LDAP_RET_ERR_SEND;
    }
    sess->w_curr += r;
    if (sess->w_curr < sess->w_len)
    {
        sess->write_time = time(NULL);
        if (fd_set_write_callback!=NULL)
        {
            if(fd_set_write_callback(sess->sock) != 0)
            {
                if (error!=NULL) sprintf(error, "fd_set_write error for socket (%d)", sess->sock);
                return LDAP_RET_ERR_SEND;
            }
        }
        return LDAP_RET_SENT_FRAGMENT;
    }
    else
    {
        sess->w_curr = 0;
        sess->w_len = 0;
        sess->write_time = 0;
        ++(ds->sent_msg_count);
        ++(sess->write_count);
        return LDAP_RET_SENT;
    }

    return LDAP_RET_OK;
}

int
LDAP_sess_decode(DS_t *ds, DSSESSION *sess, char *error)
{
    int n, ret=LDAP_RET_OK;
    char *p = NULL;
    LDAPMessage_t *msg = NULL;

    do
    {
        if (sess->r_buffer == NULL) break;
        if (sess->r_len < 5) break;

        //sess->r_buffer[lc->r_len] = '\0';
        if (sess->r_ber_body < 0)
        {
            p = sess->r_buffer;
            while (*p != 0x30) p++;

            sess->r_ber_body = (int)(p - sess->r_buffer);
        }
        int pp = 0;
        int length_size = 0;
        if (sess->r_ber_body_length < 0)
        {
            sess->r_ber_body_length = 1;//sequence 0x30 above!
            p += 1;
            sess->r_ber_body_length += 1;
            if (p[0] > 0xffffff00)
            {
                length_size = p[0] - 0xffffff80;
                while (length_size > 0)
                {
                    p += 1;
                    sess->r_ber_body_length += 1;
                    if ((int)p[0] > 0xffffff00)
                    {
                        pp = pp + (int)p[0] - 0xffffff00;
                    }
                    else
                    {
                        pp = pp + (int)p[0];
                    }

                    if (length_size > 1)
                        pp = pp << (1 * 8);
                    (--length_size);
                }
                sess->r_ber_body_length += pp;//length
            }
            else
            {
                sess->r_ber_body_length = p[0] + 2;//length
            }
        }
        if ((n = sess->r_ber_body + sess->r_ber_body_length + 1) > sess->r_size)
        {
            p = realloc(sess->r_buffer, n);
            if (p == NULL)
            {
                if (error!=NULL) sprintf(error, "cannot allocation memory (%d)", n);
                return LDAP_RET_ERR_MEMORY;
            }
            sess->r_buffer = p;
            sess->r_size = n;
        }

        if ((sess->r_ber_body + sess->r_ber_body_length) > sess->r_len)//continue to read
            return LDAP_RET_OK;

        //sess->read_time = 0;
        ret = ldap_decode_request(sess, 0, error);
        n = sess->r_len - (sess->r_ber_body + sess->r_ber_body_length);
        if (n > 0)
        {
            memmove(sess->r_buffer, sess->r_buffer + sess->r_ber_body + sess->r_ber_body_length, n);
            sess->r_len = n;
        }
        else
        {
            sess->r_len = 0;
            sess->read_time = 0;
        }
        sess->r_ber_body = -1;
        sess->r_ber_body_length = -1;

    } while (ret==LDAP_RET_WAIT_MORE_DATA);

    if (ret==LDAP_RET_DATA_AVAILABLE)
    {
        if (sess->concurrent_count < ds->max_concurrent){
            ++(sess->concurrent_count);
        }
        else
        {
            if (error!=NULL) sprintf(error, "LDAP received data at limited concurrent (max: %d)", ds->max_concurrent);
            msg = sess->rdata->prev; // remove the latest message
            ldap_send_generic_resp(ds, sess, LDAP_RESULT_SUCCESS, msg);
            LINKEDLIST_REMOVE(sess->rdata, msg);
            SDF_FREE(msg);
            return LDAP_RET_ERR_UNEXPECTED_RECEIVE;
        }

        ++(ds->recv_msg_count);
        struct timeval overall_time;
        struct timeval stoptime;
        /* Find Max, Min, Avg response time for Overall */
        gettimeofday(&stoptime, 0);
        timeval_subtract(&overall_time, &stoptime, &(sess->timestamp_write));
        unsigned long process_time = overall_time.tv_sec*1000000+overall_time.tv_usec;
        ds->total_response_time += process_time;
        ds->max_response_time = (process_time > ds->max_response_time)?process_time:ds->max_response_time;
        ds->min_response_time = (ds->recv_msg_count > 1 )?((process_time < ds->min_response_time)?process_time:ds->min_response_time):process_time;
        ds->avg_response_time = (ds->recv_msg_count > 0)?(ds->total_response_time/ds->recv_msg_count):0;
    }
    else if (ret==LDAP_RET_RECV_UNBIND)
    {
        return LDAP_RET_RECV_UNBIND;
    }
    else if (ret==LDAP_RET_KEEPALIVE)
    {   // TODO:
        ldap_send_keepalive_resp(ds, sess, NULL, NULL);
    }
    return ret;
}

int
LDAP_sess_send_message (DS_t *ds, DSSESSION *sess, BER *ber, int (*fd_set_write_callback)(int), char *error)
{
    int len;
    char *buff;

    if ((ds==NULL)||(sess==NULL)||(ber==NULL))
    {
        if (error!=NULL) sprintf(error, "invalid parameters");
        return LDAP_RET_ERR_PARAMETER;
    }

    if (sess->w_buffer == NULL)//first time
    {
        sess->w_size = LDAP_WRITE_BUFFER;
        sess->w_buffer = (char *) malloc (sess->w_size);
        if (sess->w_buffer == NULL)
        {
            if (error!=NULL)
                sprintf(error, "can not allocate memory size (%u)", (unsigned int)sess->r_size);
            return LDAP_RET_ERR_MEMORY;
        }
        sess->w_len = 0;
    }
    if (sess->w_len + ber->len > sess->w_size)
    {
        len = sess->w_len + ber->len;
        ADJUST_SIZE(len, LDAP_WRITE_BUFFER)
        buff = (char *) realloc (sess->w_buffer, len);
        if (buff == NULL)
        {
            if (error!=NULL)
                sprintf(error, "memory error");
            return LDAP_RET_ERR_MEMORY;
        }
        sess->w_size = len;
        sess->w_buffer = buff;
        (void) memset (sess->w_buffer+sess->w_len, 0, sess->w_size-sess->w_len);
    }
    (void) memcpy (sess->w_buffer+sess->w_len, ber->berval, ber->len);
    sess->w_len += ber->len;
    sess->write_time = time(NULL);
    --(sess->concurrent_count);
    free(ber);

    if (fd_set_write_callback != NULL)
    {
        if (fd_set_write_callback(sess->sock)!=0)
        {
            if (error!=NULL)
                sprintf(error, "fd_set_write_callback return error");
            return LDAP_RET_ERR_SEND;
        }
    }

    return LDAP_RET_OK;
}
