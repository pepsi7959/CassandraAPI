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


#include "ldap.h"
#include "ldap_common.h"
#include "sdflib.h"
#include "linkedlist.h"

LDAP_t *
LDAP_clnt_init (const char *group, const char *dn, const char *username, const char *password, const char *host, int port, int wbuffer, int concurrent, int max_client)
{
    LDAP_t *dc = NULL;

    dc = (LDAP_t *) malloc(sizeof(*dc));
    if (dc != NULL)
    {
        (void) memset (dc, 0, sizeof(*dc));
        if (group!=NULL) strncpy(dc->group, group, sizeof(dc->group)-1);
        if (dn!=NULL) strncpy(dc->baseDN, dn, sizeof(dc->baseDN)-1);
        if (username!=NULL) strncpy(dc->username, username, sizeof(dc->username)-1);
        if (password!=NULL) strncpy(dc->password, password, sizeof(dc->password)-1);
        if (host!=NULL) strncpy(dc->host, host, sizeof(dc->host)-1);
        dc->port = port;
        dc->max_wbuffer = wbuffer;
        dc->max_concurrent = concurrent;
        dc->max_client = max_client;
        dc->client_count = 0;
        dc->send_msg_queue = 0;
        dc->sent_count = 0;
        dc->recv_count = 0;
        dc->error_count = 0;
        dc->avg_response_time = 0;
        dc->min_response_time = 0;
        dc->max_response_time = 0;
        dc->total_response_time = 0;
        dc->ready_queue = NULL;
        dc->wait_queue = NULL;
        SDF_random(&(dc->tid), sizeof(dc->tid)-1);
    }

    return dc;
}

void
LDAP_clnt_free (LDAP_t *dc)
{
    if (dc==NULL) return;
    free(dc);
}

int
LDAP_clnt_get_option(LDAP_t *dc, int option, char *output)
{
    int value=-1;
    if (dc==NULL) return -1;
    switch (option)
    {
        case LDAP_CLNT_OPTION_CLIENT_COUNT:
            value = dc->client_count;
            break;

        case LDAP_CLNT_OPTION_QUEUE_COUNT:
            value = dc->queue_count;
            break;

        case LDAP_CLNT_OPTION_SENT_COUNT:
            value = dc->sent_count;
            break;

        case LDAP_CLNT_OPTION_RECV_COUNT:
            value = dc->recv_count;
            break;
            
        case LDAP_CLNT_OPTION_ERROR_COUNT:
            value = dc->error_count;
            break;

        case LDAP_CLNT_OPTION_MAX_CLIENT:
            value = dc->max_client;
            break;

        case LDAP_CLNT_OPTION_NUMBER:
            value = dc->no;
            break;

        case LDAP_CLNT_OPTION_GROUP:
            if (output!=NULL) strcpy(output, dc->group);
            break;

        case LDAP_CLNT_OPTION_HOST:
            if (output!=NULL) strcpy(output, dc->host);
            break;

        case LDAP_CLNT_OPTION_PORT:
            value = dc->port;
            break;

        case LDAP_CLNT_OPTION_READY:
            if (dc->ready_queue != NULL)
                value = 1;
            else
                value = 0;
            break;

        case LDAP_CLNT_OPTION_MAX_RESPONSE:
            value = (int) dc->max_response_time;
            break;

        case LDAP_CLNT_OPTION_MIN_RESPONSE:
            value = (int) dc->min_response_time;
            break;

        case LDAP_CLNT_OPTION_AVG_RESPONSE:
            value = (int) dc->avg_response_time;
            break;

        default:
            value = -1;
            break;
    }

    return value;
}

static int
set_option (LDAP_t *dc, int option, va_list ap)
{
    int n;
    char *c;

    switch (option)
    {
        case LDAP_CLNT_OPTION_NUMBER:
            n = va_arg(ap,int);
            dc->no = n;
            break;
        case LDAP_CLNT_OPTION_GROUP:
            c = va_arg(ap,char *);
            strncpy(dc->group, c, sizeof(dc->group)-1);
            break;
        case LDAP_CLNT_OPTION_HOST:
            c = va_arg(ap,char *);
            strncpy(dc->host, c, sizeof(dc->host)-1);
            break;
        case LDAP_CLNT_OPTION_PORT:
            n = va_arg(ap,int);
            if (n > 0xFFFF) return LDAP_RET_ERR_PARAMETER;
            dc->port = n;
            break;
        case LDAP_CLNT_OPTION_BASEDN:
            c = va_arg(ap,char *);
            strncpy(dc->baseDN, c, sizeof(dc->baseDN)-1);
            break;
        case LDAP_CLNT_OPTION_USERNAME:
            c = va_arg(ap,char *);
            strncpy(dc->username, c, sizeof(dc->username)-1);
            break;
        case LDAP_CLNT_OPTION_PASSWORD:
            c = va_arg(ap,char *);
            strncpy(dc->password, c, sizeof(dc->password)-1);
            break;
        case LDAP_CLNT_OPTION_MAX_CLIENT:
            n = va_arg(ap,int);
            if (n > LDAP_MAX_CLIENT) return LDAP_RET_ERR_PARAMETER;
            dc->max_client = n;
            break;
        case LDAP_CLNT_OPTION_MAX_CONCURRENT:
            n = va_arg(ap,int);
            if (n > LDAP_MAX_CONCURRENT_REQUEST) return LDAP_RET_ERR_PARAMETER;
            dc->max_concurrent = n;
            break;
        case LDAP_CLNT_OPTION_MAX_WBUFFER:
            n = va_arg(ap,int);
            dc->max_wbuffer = n;
            break;
        case LDAP_CLNT_OPTION_CONNECT_TIMEOUT:
            n = va_arg(ap,int);
            dc->connect_timeout = n;
            break;
        case LDAP_CLNT_OPTION_WAIT_TIMEOUT:
            n = va_arg(ap,int);
            dc->wait_timeout = n;
            break;
        case LDAP_CLNT_OPTION_READ_TIMEOUT:
            n = va_arg(ap,int);
            dc->read_timeout = n;
            break;
        case LDAP_CLNT_OPTION_WRITE_TIMEOUT:
            n = va_arg(ap,int);
            dc->write_timeout = n;
            break;
        case LDAP_CLNT_OPTION_KEEPALIVE_TYPE:
            n = va_arg(ap,int);
            dc->heartbeat.type = n;
            break;
        case LDAP_CLNT_OPTION_KEEPALIVE_INTERVAL:
            n = va_arg(ap,int);
            dc->heartbeat.interval = n;
            break;
        case LDAP_CLNT_OPTION_KEEPALIVE_DN:
            c = va_arg(ap,char *);
            strncpy(dc->heartbeat.dn, c, sizeof(dc->heartbeat.dn)-1);
            break;
        case LDAP_CLNT_OPTION_KEEPALIVE_OID:
            c = va_arg(ap,char *);
            strncpy(dc->heartbeat.oid, c, sizeof(dc->heartbeat.oid)-1);
            break;
        case LDAP_CLNT_OPTION_SENT_COUNT:
            n = va_arg(ap,int);
            dc->sent_count = n;
            break;
        case LDAP_CLNT_OPTION_RECV_COUNT:
            n = va_arg(ap,int);
            dc->recv_count = n;
            break;
        case LDAP_CLNT_OPTION_ERROR_COUNT:
            n = va_arg(ap,int);
            dc->error_count = n;
            break;
        case LDAP_CLNT_OPTION_MAX_RESPONSE:
            n = va_arg(ap,int);
            dc->max_response_time = n;
            break;
        case LDAP_CLNT_OPTION_MIN_RESPONSE:
            n = va_arg(ap,int);
            dc->min_response_time = n;
            break;
        case LDAP_CLNT_OPTION_AVG_RESPONSE:
            n = va_arg(ap,int);
            dc->avg_response_time = n;
            break;
        default :
            return LDAP_RET_ERR_PARAMETER;
    }
    return LDAP_RET_OK;
}

int
LDAP_clnt_set_option (LDAP_t *dc, int option, ...)
{
    int r;
    va_list ap;

    va_start(ap,option);
    r = set_option (dc, option, ap);
    va_end(ap);

    return r;
}

int
LDAP_clnt_open(LDAP_t *dc, LDAP_CLNT **lc, char *error)
{
    char buff[256];
    int ff, i, r, sk;
    struct addrinfo hints, *res;
    LDAP_CLNT *ll;

    if (dc==NULL) return LDAP_RET_ERR_PARAMETER;

    if (dc->client_count >= dc->max_client)
        return LDAP_RET_MAX_CONNECTION;

    (void) memset (&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    sprintf(buff, "%d", dc->port);
    r = getaddrinfo(dc->host, buff, &hints, &res);
    if (r != 0)
    {
        if (error!=NULL)
            sprintf(error, "getaddrinfo return error [%s]", gai_strerror(r));
        return LDAP_RET_ERR_CONNECT;
    }
    sk = socket(PF_INET, SOCK_STREAM, 0);
    if (sk < 0)
    {
        if (error!=NULL)
            sprintf(error, "socket return error [%s]", gai_strerror(r));
        freeaddrinfo(res);
        return LDAP_RET_ERR_CONNECT;
    }
    ff = fcntl(sk, F_GETFL, 0);
    if (ff < 0)
    {
        if (error!=NULL)
            sprintf(error, "fcntl return error [%s]", gai_strerror(r));
        close(sk);
        freeaddrinfo(res);
        return LDAP_RET_ERR_CONNECT;
    }
    if (fcntl(sk, F_SETFL, ff | O_NONBLOCK) < 0)
    {
        if (error!=NULL)
            sprintf(error, "fcntl return error [%s]", gai_strerror(r));
        close(sk);
        freeaddrinfo(res);
        return LDAP_RET_ERR_CONNECT;
    }
    if (connect(sk, res->ai_addr, res->ai_addrlen) != 0)
    {
        if (errno != EINPROGRESS)
        {
            if (error!=NULL)
                sprintf(error, "connect return error [%s]", gai_strerror(r));
            close(sk);
            freeaddrinfo(res);
            return LDAP_RET_ERR_CONNECT;
        }
    }
    freeaddrinfo(res);

    for (i = 0; i < LDAP_MAX_CLIENT; ++i)
    {
        if (dc->client_array[i] == NULL)
            break;
    }
    if (i >= LDAP_MAX_CLIENT)
    {
        if (error!=NULL)
            sprintf(error, "ldap connection exceeds [%d]", LDAP_MAX_CLIENT);
        close(sk);
        return LDAP_RET_ERR_UNAVAILABLE;
    }
    dc->client_array[i] = ll = (LDAP_CLNT *) malloc (sizeof(*ll));
    if (ll == NULL)
    {
        if (error!=NULL)
            sprintf(error, "can not allocate memory size (%u)", (unsigned int)sizeof(*ll));
        close(sk);
        return LDAP_RET_ERR_MEMORY;
    }

    struct sockaddr_in local_address;
    unsigned int addr_size = sizeof(local_address);
    getsockname(sk, (struct sockaddr *) &local_address, &addr_size);

    (void) memset(ll, 0, sizeof(*ll));
    ll->group = (void *)dc;
    ll->no = i;
    ll->sock = sk;
    ll->ktid = 0;
    ll->create_time = time(NULL);
    ll->local_port = ntohs(local_address.sin_port);
    ll->state = LDAP_CLNT_STATE_OPEN;
    ll->concurrent_count = 0;
    ll->r_ber_body = -1;
    ll->r_ber_body_length = -1;
    ll->w_buffer = (char *) malloc (LDAP_READ_BUFFER);
    if (ll->w_buffer == NULL)
    {
        if (error!=NULL)
            sprintf(error, "can not allocate memory size (%d)", LDAP_READ_BUFFER);
        close(sk);
        return LDAP_RET_ERR_MEMORY;
    }
    ll->w_size = 0;
    ll->w_curr = 0;
    ll->w_len = 0;

    LINKEDLIST_APPEND(dc->wait_queue, ll);
    ++(dc->client_count);

    if (lc!=NULL) *lc = ll;
    return LDAP_RET_OK;
}

int
LDAP_clnt_close(LDAP_t *dc, int no, char *error)
{
    char buff[16 * 1024];
    int ret=LDAP_RET_OK, i=0;
    LDAP_CLNT *lc = NULL;

    if (dc==NULL) return LDAP_RET_ERR_PARAMETER;

    if ((lc=dc->client_array[no])==NULL)
    {
        if (error!=NULL)
            sprintf(error, "connection not used (%d)", no);
        return LDAP_RET_ERR_UNAVAILABLE;
    }
    dc->client_array[no] = NULL;

    if (lc->rber != NULL)
    {
        free(lc->rber);
        lc->rber = NULL;
    }

    if (lc->bstring != NULL)
    {
        free(lc->bstring);
        lc->bstring = NULL;
    }

    if (lc->state == LDAP_CLNT_STATE_READY)
    {
        LINKEDLIST_REMOVE(dc->ready_queue, lc);
    }
    else
    {
        LINKEDLIST_REMOVE(dc->wait_queue, lc);
    }
    --(dc->client_count);

    if (lc->w_buffer != NULL)
    {
        free(lc->w_buffer);
        lc->w_buffer = NULL;
    }
    if (lc->r_buffer != NULL)
    {
        free(lc->r_buffer);
        lc->r_buffer = NULL;
    }

    (void) shutdown (lc->sock, SHUT_WR);
    while ((recv(lc->sock, buff, sizeof(buff), 0) > 0)&&(i<100)) ++(i);
    if (close(lc->sock) != 0)
    {
        if (error!=NULL)
            sprintf(error, "close socket return error [%s]", strerror(errno));
        ret = LDAP_RET_ERR_CONNECT;
    }
    free(lc);
    
    return ret;
}

void
LDAP_clnt_close_all(LDAP_t *dc, char *error)
{
    if (dc==NULL) return;

    int i;
    for (i = 0; i < LDAP_MAX_CLIENT; ++i)
    {
        if (dc->client_array[i] != NULL)
            LDAP_clnt_close(dc, i, error);
    }
}

int
LDAP_clnt_read(LDAP_t *dc, LDAP_CLNT *lc, char *error)
{
    int n, r;

    if ((dc==NULL)||(lc==NULL))
    {
        if (error!=NULL)
            sprintf(error, "invalid parameters");
        return LDAP_RET_ERR_PARAMETER;
    }
    if (lc->state == LDAP_CLNT_STATE_OPEN)
    {
        if (error!=NULL)
            sprintf(error, "connection isn't ready");
        return LDAP_RET_ERR_CONNECT;
    }
    else if (lc->state == LDAP_CLNT_STATE_CONNECTING)
    {
        int e = 0;
        socklen_t elen = sizeof(e);
        if (getsockopt(lc->sock, SOL_SOCKET, SO_ERROR, (char *)&e, &elen) != 0)
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

        r = ldap_recv_bind_resp(dc, lc);
        if (r != 0)
        {
            if (error!=NULL)
                sprintf(error, "bind resp return error");
            return LDAP_RET_ERR_CONNECT;
        }
        lc->write_time = 0;
        lc->keepalive_time = time(NULL);
        lc->state = LDAP_CLNT_STATE_READY;
        LINKEDLIST_REMOVE(dc->wait_queue, lc);
        LINKEDLIST_APPEND(dc->ready_queue, lc);

        return LDAP_RET_READY;
    }

    if (lc->r_buffer == NULL)//first time
    {
        lc->r_size = LDAP_READ_BUFFER;
        lc->r_buffer = (char *) malloc (lc->r_size);
        if (lc->r_buffer == NULL)
        {
            if (error!=NULL)
                sprintf(error, "can not allocate memory size (%u)", (unsigned int)lc->r_size);
            return LDAP_RET_ERR_MEMORY;
        }
        lc->r_len = 0;
    }
    n = lc->r_size - 1 - lc->r_len;
    if (n <= 0)
    {
        if (error!=NULL)
            sprintf(error, "read buffer full");
        return LDAP_RET_ERR_MEMORY;
    }
    r = (int) recv (lc->sock, lc->r_buffer + lc->r_len, n, 0);
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
        return LDAP_RET_ERR_CONNECT;
    }

    lc->r_len += r;
    lc->read_time = time(NULL);
    return LDAP_RET_OK;
}

int
LDAP_clnt_write(LDAP_t *dc, LDAP_CLNT *lc, char *error)
{
    int n, r;

    if ((dc==NULL)||(lc==NULL))
    {
        if (error!=NULL)
            sprintf(error, "invalid parameters");
        return LDAP_RET_ERR_PARAMETER;
    }
    if (lc->state == LDAP_CLNT_STATE_OPEN)
    {
        int e = 0;
        socklen_t elen = sizeof(e);
        if (getsockopt(lc->sock, SOL_SOCKET, SO_ERROR, (char *)&e, &elen) != 0)
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
        r = ldap_send_bind_rqt(dc, lc);
        if (r != 0)
        {
            if (error!=NULL)
                sprintf(error, "bind request return error");
            return LDAP_RET_ERR_CONNECT;
        }
        lc->write_time = time(NULL);
        lc->state = LDAP_CLNT_STATE_CONNECTING;
        return LDAP_RET_OK;
    }
    else if (lc->state == LDAP_CLNT_STATE_CONNECTING)
    {
        return LDAP_RET_OK;
    }

    if ((lc->w_buffer == NULL)||(lc->w_len==0)) return LDAP_RET_OK;
    n = lc->w_len - lc->w_curr;
    // Limit write buffer
    if (dc->max_wbuffer > 0 && n > dc->max_wbuffer)
        n = dc->max_wbuffer;

    r = (int) send (lc->sock, lc->w_buffer + lc->w_curr, n, 0);
    if (r < 0)
    {
        if (error!=NULL)
            sprintf(error, "send return error [%s]", strerror(errno));
        return LDAP_RET_ERR_SEND;
    }
    lc->w_curr += r;
    if (lc->w_curr < lc->w_len)
        lc->write_time = time(NULL);
    else
    {
        lc->w_curr = 0;
        lc->w_len = 0;
        lc->write_time = 0;
        lc->keepalive_time = time(NULL);
        ++(dc->sent_count);
        return LDAP_RET_SENT;
    }
    return LDAP_RET_OK;
}

int
LDAP_clnt_send_message (LDAP_t *dc, LDAP_CLNT *lc, BER *ber, int (*fd_set_write_callback)(int), char *error)
{
    int len;
    char *buff;

    if (dc==NULL)
    {
        if (error!=NULL) sprintf(error, "invalid parameters");
        return LDAP_RET_ERR_PARAMETER;
    }
    // Specify LDAP connection
    if ((lc!=NULL)&&(ber!=NULL))
    {
        if (lc->w_buffer == NULL)
        {
            if (error!=NULL)
                sprintf(error, "memory error");
            return LDAP_RET_ERR_MEMORY;
        }
        if (lc->w_len+ber->len > lc->w_size)
        {
            len = lc->w_len + ber->len;
            ADJUST_SIZE(len, LDAP_WRITE_BUFFER)
            buff = (char *) realloc (lc->w_buffer, len);
            if (buff == NULL)
            {
                if (error!=NULL)
                    sprintf(error, "memory error");
                return LDAP_RET_ERR_MEMORY;
            }
            lc->w_size = len;
            lc->w_buffer = buff;
            (void) memset (lc->w_buffer+lc->w_len, 0, lc->w_size-lc->w_len);
        }
        (void) memcpy (lc->w_buffer+lc->w_len, ber->berval, ber->len);
        lc->w_len += ber->len;
        lc->write_time = time(NULL);
        ++(lc->concurrent_count);
        free(ber);
        if (fd_set_write_callback != NULL)
        {
            if (fd_set_write_callback(lc->sock)!=0)
            {
                return LDAP_RET_ERR_SEND;
            }
        }
    }
    else if (ber!=NULL)
    {
        LINKEDLIST_APPEND(dc->send_msg_queue, ber);
        ++(dc->queue_count);
    }

    // Flush send buffer
    while ((lc = dc->ready_queue) != NULL && (ber = dc->send_msg_queue) != NULL)
    {
        if (lc->w_buffer == NULL)
        {
            if (error!=NULL)
                sprintf(error, "memory error");
            return LDAP_RET_ERR_MEMORY;
        }
        if (lc->w_len+ber->len > lc->w_size)
        {
            len = lc->w_len + ber->len;
            ADJUST_SIZE(len, LDAP_WRITE_BUFFER)
            buff = (char *) realloc (lc->w_buffer, len);
            if (buff == NULL)
            {
                if (error!=NULL)
                    sprintf(error, "memory error");
                return LDAP_RET_ERR_MEMORY;
            }
            lc->w_size = len;
            lc->w_buffer = buff;
            (void) memset (lc->w_buffer+lc->w_len, 0, lc->w_size-lc->w_len);
        }
        (void) memcpy (lc->w_buffer+lc->w_len, ber->berval, ber->len);
        lc->w_len += ber->len;
        lc->write_time = time(NULL);
        gettimeofday(&(lc->timestamp_write),0);
        ++(lc->concurrent_count);
        if (lc->concurrent_count >= dc->max_concurrent)
        {
            lc->state = LDAP_CLNT_STATE_WAITING;
            LINKEDLIST_REMOVE(dc->ready_queue, lc);
            LINKEDLIST_APPEND(dc->wait_queue, lc);
        }
        else // Round robin the connection
        {
            LINKEDLIST_REMOVE(dc->ready_queue, lc);
            LINKEDLIST_APPEND(dc->ready_queue, lc);
        }

        LINKEDLIST_REMOVE(dc->send_msg_queue, ber);
        free(ber);
        --(dc->queue_count);
        if (fd_set_write_callback != NULL)
        {
            if (fd_set_write_callback(lc->sock)!=0)
            {
                return LDAP_RET_ERR_SEND;
            }
        }
    }
    return LDAP_RET_OK;
}

int
LDAP_clnt_keep_alive(LDAP_t *dc, char *error)
{
    int i;
    LDAP_CLNT *lc=NULL;
    BER *ber=NULL;
    time_t ct=time(NULL), ht;

    for (i = 0; i < LDAP_MAX_CLIENT; ++i)
    {
        if ((lc=dc->client_array[i])!=NULL)
        {
            ber = NULL;
            if ((lc->write_time==0)&&(lc->read_time==0))
                ht = lc->keepalive_time;
            else
                ht = 0;
            if ((ht > 0)&&(ct - ht >= dc->heartbeat.interval))
            {
                if (dc->heartbeat.type==LDAP_KEEPALIVE_SEARCH)
                {
                    lc->ktid = LDAP_gen_search_request(dc, &ber, dc->heartbeat.dn, "base", "*", NULL, 0, LDAP_DEREF_ALWAYS, NULL, NULL);
                    if (lc->ktid==0)
                    {
                        if (error!=NULL) sprintf(error, "LDAP_gen_search_request return error");
                        LDAP_clnt_close(dc, lc->no, NULL);
                        return LDAP_RET_ERR_KEEPALIVE;
                    }
                }
                else if (dc->heartbeat.type==LDAP_KEEPALIVE_EXTENDED)
                {
                    lc->ktid = LDAP_gen_extended_request(dc, &ber, dc->heartbeat.oid, NULL, 0, NULL, NULL);
                    if (lc->ktid==0)
                    {
                        if (error!=NULL) sprintf(error, "LDAP_gen_extended_request return error");
                        LDAP_clnt_close(dc, lc->no, NULL);
                        return LDAP_RET_ERR_KEEPALIVE;
                    }
                }

                if ((ber!=NULL)&&(ldap_send_keepalive_rqt(dc, lc, ber, NULL)!=LDAP_RET_OK))
                {
                    if (error!=NULL) sprintf(error, "ldap_send_keepalive_rqt return error");
                    LDAP_clnt_close(dc, lc->no, NULL);
                    return LDAP_RET_ERR_KEEPALIVE;
                }
            }
        }
    }

    return LDAP_RET_OK;
}

int
LDAP_clnt_check_connection(LDAP_t *dc, char *error)
{
    int i;
    LDAP_CLNT *lc=NULL;
    time_t ct=time(NULL);

    for (i = 0; i < LDAP_MAX_CLIENT; ++i)
    {
        if ((lc=dc->client_array[i])!=NULL)
        {
            if (lc->read_time > 0 && dc->read_timeout > 0 && (ct - lc->read_time) >= dc->read_timeout)
            {
                if (error!=NULL) sprintf(error, "LDAP read timeout while in state [%s]", state_to_string(lc->state));
                (void)LDAP_clnt_close(dc, lc->no, NULL);
                continue;
            }
            if (lc->write_time > 0 && dc->write_timeout > 0 && (ct - lc->write_time) >= dc->write_timeout)
            {
                if (error!=NULL) sprintf(error, "LDAP write timeout while in state [%s]", state_to_string(lc->state));
                (void)LDAP_clnt_close(dc, lc->no, NULL);
                continue;
            }
            if (lc->state == LDAP_CLNT_STATE_CONNECTING && dc->connect_timeout > 0 && (ct - lc->create_time) >= dc->connect_timeout)
            {
                if (error!=NULL) sprintf(error, "LDAP connect timeout");
                (void)LDAP_clnt_close(dc, lc->no, NULL);
                continue;
            }
        }
    }
    
    return LDAP_RET_OK;
}

int
LDAP_clnt_decode(LDAP_t *dc, LDAP_CLNT *lc, char *error)
{
    int n, ret=LDAP_RET_OK;
    char *p = NULL;

    do
    {
        if (lc->r_buffer == NULL) break;
        if (lc->r_len < 5) break;

        //lc->r_buffer[lc->r_len] = '\0';
        if (lc->r_ber_body < 0)
        {
            p = lc->r_buffer;
            while (*p != 0x30)
                p++;

            lc->r_ber_body = (int)(p - lc->r_buffer);
        }
        int pp = 0;
        int length_size = 0;
        if (lc->r_ber_body_length < 0)
        {
            lc->r_ber_body_length = 1;//sequence 0x30 above!
            p += 1;
            lc->r_ber_body_length += 1;
            if (p[0] > 0xffffff00)
            {
                length_size = p[0] - 0xffffff80;
                while (length_size > 0)
                {
                    p += 1;
                    lc->r_ber_body_length += 1;
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
                lc->r_ber_body_length += pp;//length
            }
            else
            {
                lc->r_ber_body_length = p[0] + 2;//length
            }
        }
        if ((n = lc->r_ber_body + lc->r_ber_body_length + 1) > lc->r_size)
        {
            p = realloc(lc->r_buffer, n);
            if (p == NULL)
            {
                return LDAP_RET_ERR_MEMORY;
            }
            lc->r_buffer = p;
            lc->r_size = n;
        }

        if ((lc->r_ber_body + lc->r_ber_body_length) > lc->r_len)//continue to read
            return LDAP_RET_OK;

        //lc->read_time = 0;
        lc->result_code = -1;
        lc->matchedDN[0] = 0;
        lc->diagnosticMessage[0] = 0;
        ret = ldap_decode_response(lc, 0, error);
        if ((lc->result_code!=0)&&(lc->diagnosticMessage[0]==0))
        {
            strcpy(lc->diagnosticMessage, error_to_string(lc->result_code));
        }

        n = lc->r_len - (lc->r_ber_body + lc->r_ber_body_length);
        if (n > 0)
        {
            memmove(lc->r_buffer, lc->r_buffer + lc->r_ber_body + lc->r_ber_body_length, n);
            lc->r_len = n;
        }
        else
        {
            lc->r_len = 0;
            lc->read_time = 0;
            lc->keepalive_time = time(NULL);
        }
        lc->r_ber_body = -1;
        lc->r_ber_body_length = -1;

    } while (ret==LDAP_RET_WAIT_MORE_DATA);

    if (ret==LDAP_RET_DATA_AVAILABLE)
    {
        if (lc->concurrent_count > 0){
            --(lc->concurrent_count);
        }else
        {
            if (error!=NULL) sprintf(error, "LDAP received unexpected data");
            return LDAP_RET_ERR_UNEXPECTED_RECEIVE;
        }

        if ((lc->state == LDAP_CLNT_STATE_WAITING) && (lc->concurrent_count < dc->max_concurrent))
        {
            lc->state = LDAP_CLNT_STATE_READY;
            LINKEDLIST_REMOVE(dc->wait_queue, lc);
            LINKEDLIST_APPEND(dc->ready_queue, lc);
        }
        ++(dc->recv_count);
        struct timeval overall_time;
        struct timeval stoptime;
        /* Find Max, Min, Avg response time for Overall */
        gettimeofday(&stoptime, 0);
        timeval_subtract(&overall_time, &stoptime, &(lc->timestamp_write));
        unsigned long process_time = overall_time.tv_sec*1000000+overall_time.tv_usec;
        dc->total_response_time += process_time;
        dc->max_response_time = (process_time > dc->max_response_time)?process_time:dc->max_response_time;
        dc->min_response_time = (dc->recv_count > 1 )?((process_time < dc->min_response_time)?process_time:dc->min_response_time):process_time;
        dc->avg_response_time = (dc->recv_count > 0)?(dc->total_response_time/dc->recv_count):0;

    }
    else if (ret==LDAP_RET_KEEPALIVE)
    {
        if (dc->wait_queue!=NULL)
        {
            LDAP_CLNT *ka, *kb;
            ka = kb = dc->wait_queue;
            do
            {
                if (lc==kb)
                {   // Reset deadlock queue
                    LINKEDLIST_REMOVE(dc->wait_queue, lc);
                    LINKEDLIST_APPEND(dc->ready_queue, lc);
                    lc->w_len = 0;
                    lc->w_curr = 0;
                    break;
                }
                kb = kb->next;
            } while (ka!=kb);
        }
    }
    return ret;
}

int
LDAP_clnt_string_free(LDAP_CLNT *lc, char *error)
{
    if (lc->bstring != NULL)
    {
        free(lc->bstring);
        lc->bstring = NULL;
    }
    return LDAP_RET_OK;
}

