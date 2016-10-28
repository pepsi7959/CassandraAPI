
/*
   Copyright (C) 2014, TORO, MIMO Tech Co., Ltd.

   http.h

   Revision history

   1.0.0   Initialize.

*/
#include <sys/time.h>

#include "common.h"
#include "rauthen.h"

#ifndef _HTTP_H_
#define _HTTP_H_

#define HTTP_VERSION                            "HTTP/1.1"
#define HTTP_METHOD                             "POST"
#define HTTP_CONTENT_LENGTH                     "Content-Length"
#define HTTP_CONTENT_TYPE                       "Content-Type"
#define HTTP_CONTENT_TYPE_VALUE                 "text/xml"
#define HTTP_HOST								"Host"
#define HTTP_COOKIE                             "Cookie"
#define HTTP_COOKIE_SET                         "Set-Cookie"
#define HTTP_PRAGMA                             "Pragma"

#define HTTP_MAX_SESSION                        4096
#define HTTP_MAX_HEADER_SIZE                    16384
#define HTTP_MAX_COOKIE_HOST_SIZE               1024 //!--Chrome request overflow
#define HTTP_MAX_COOKIE_PORT_SIZE               8
#define HTTP_MAX_COOKIE_VIA_SIZE                64
#define HTTP_MAX_PRAGMA_END_SIZE                64
#define HTTP_MAX_PRAGMA_HOP_SIZE                64
#define HTTP_MAX_PRAGMA_SESSION_SIZE            128
#define HTTP_MAX_CONTENT_TYPE_SIZE              64
#define HTTP_MAX_OVERRIDE_HEADER_LEN            64
#define HTTP_MAX_OVERRIDE_HEADER_VALUE_LEN      1024
#define HTTP_MAX_QUERY_STRING_LEN               1024

#define HTTP_DATA_BUFFER                        65536

enum HTTP_RET_CODE
{
    HTTP_RET_ERR_UNKNOWN = -1000,
    HTTP_RET_ERR_PARAMETER,
    HTTP_RET_ERR_CONNECT,
    HTTP_RET_ERR_UNAVAILABLE,
    HTTP_RET_ERR_AUTHEN,
    HTTP_RET_ERR_MEMORY,
    HTTP_RET_ERR_SEND,
    HTTP_RET_ERR_RECEIVE,
    HTTP_RET_ERR_ENCODE,
    HTTP_RET_ERR_DECODE,
    HTTP_RET_ERR_READ,
    HTTP_RET_ERR_WRITE,
    HTTP_RET_ERR_CLOSE,

    HTTP_RET_OK = 0,
    HTTP_RET_DATA_AVAILABLE,
    HTTP_RET_ERROR_AVAILABLE,
    HTTP_RET_WAIT_MORE_DATA,
    HTTP_RET_MAX_SESSION,
    HTTP_RET_READY,
    HTTP_RET_SENT,
    HTTP_RET_SENT_CLOSE,
    HTTP_RET_SENT_FRAGMENT,
    HTTP_RET_TIMEOUT,

    HTTP_RET_LAST
};

enum HTTP_OPTION
{
    HTTP_OPTION_PORT = 0,
    HTTP_OPTION_BACKLOG,
    HTTP_OPTION_MAX_SESSION,
    HTTP_OPTION_CONNECT_TIMEOUT,
    HTTP_OPTION_IDLE_TIMEOUT,
    HTTP_OPTION_READ_TIMEOUT,
    HTTP_OPTION_WRITE_TIMEOUT,
    HTTP_OPTION_MAX_WRITE_LENGHT,
    HTTP_OPTION_LISTENER,

    HTTP_OPTION_SESSION_COUNT,
    HTTP_OPTION_SENT_COUNT,
    HTTP_OPTION_RECV_COUNT,
    HTTP_OPTION_ERROR_COUNT,
    HTTP_OPTION_BUSY_COUNT,
    HTTP_OPTION_MAX_RESPONSE,
    HTTP_OPTION_MIN_RESPONSE,
    HTTP_OPTION_AVG_RESPONSE,

    HTTP_OPTION_RECV_GET_COUNT,
    HTTP_OPTION_RECV_PUT_COUNT,
    HTTP_OPTION_RECV_POST_COUNT,
    HTTP_OPTION_RECV_DELETE_COUNT,
    HTTP_OPTION_RECV_METHOD_COUNT,

    HTTP_OPTION_SENT_GET_COUNT,
    HTTP_OPTION_SENT_PUT_COUNT,
    HTTP_OPTION_SENT_POST_COUNT,
    HTTP_OPTION_SENT_DELETE_COUNT,
    HTTP_OPTION_SENT_METHOD_COUNT,

    HTTP_OPTION_MAX_GET_RESPONSE,
    HTTP_OPTION_MAX_PUT_RESPONSE,
    HTTP_OPTION_MAX_POST_RESPONSE,
    HTTP_OPTION_MAX_DELETE_RESPONSE,
    HTTP_OPTION_MAX_METHOD_RESPONSE,

    HTTP_OPTION_MIN_GET_RESPONSE,
    HTTP_OPTION_MIN_PUT_RESPONSE,
    HTTP_OPTION_MIN_POST_RESPONSE,
    HTTP_OPTION_MIN_DELETE_RESPONSE,
    HTTP_OPTION_MIN_METHOD_RESPONSE,

    HTTP_OPTION_AVG_GET_RESPONSE,
    HTTP_OPTION_AVG_PUT_RESPONSE,
    HTTP_OPTION_AVG_POST_RESPONSE,
    HTTP_OPTION_AVG_DELETE_RESPONSE,
    HTTP_OPTION_AVG_METHOD_RESPONSE,

    HTTP_OPTION_RESERVED
};

typedef struct _hsdata_
{
    struct _hsdata_ *prev;
    struct _hsdata_ *next;
    int size;
    int len;
    char data[1];
} HSDATA;

enum E_NORMAL_STATUS
{
	ENS_NONE = 0,
	ENS_ERROR,
	ENS_OK
};

enum HTTP_METHOD_TYPE
{
	HTTP_METHOD_POST = 0,
	HTTP_METHOD_GET,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_TRACE,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_CONNECT,

    HTTP_METHOD_LAST
};

enum E_FORCE_DATA_RESULT
{
    EFDR_NONE = 0,
    EFDR_TEXT,
    EFDR_PCDATA,
};

typedef struct _hs_session_
{
    struct _hs_session_ *prev;
    struct _hs_session_ *next;
    unsigned int        version;
    int					no;
    int					sock;
    //no wait state;
    HSDATA*				r_buffer;
    int					w_curr;
    HSDATA*				w_buffer;
    int					header_length;
    int					body_length;
    //no wait time_t state_time;
    time_t				read_time;
    time_t				write_time;
    time_t				active_time;//no wait

    char				remote_ip[40];
    unsigned short		remote_port;
    char*				request_uri;
    int					http_method;
    int					error_response;
    int                 block_request;

    char                header_version[32];
    struct timeval      timestamp_read;
    struct timeval      timestamp_write;
    long long           total_wait_write_io;
    long long           total_wait_read_io;
    unsigned char       recv_count;
    unsigned char       write_count;
    int                 tid;
    int                 format;
} HSSESSION;

//HTTP HEADER linked-list node
typedef struct _hheader_
{
	struct _hheader_ *prev;
	struct _hheader_ *next;
	int len;
	char header[1];
} HHEADER;

typedef struct _cmd_status{
    unsigned long			recv_msg_count;
    unsigned long			sent_msg_count;
    unsigned long			error_msg_count;
    unsigned long			total_response_time;
    unsigned long			max_response_time;
    unsigned long			avg_response_time;
    unsigned long			min_response_time;
}CMD_STATUS;

typedef struct _http_
{
    /* authentication */
    HOST_EQUIV*				h_equiv;

    /* http server */
    unsigned int            version;
    int						port;
    int						listen_sock;
    int						backlog;
    int						session_count;
    int						last_session;
    int                     max_session;
    char					header_version[32];
    HSSESSION				session[HTTP_MAX_SESSION];
    unsigned long			recv_msg_count;
    unsigned long			sent_msg_count;
    unsigned long			error_msg_count;
    unsigned long			busy_msg_count;
    unsigned long			max_response_time;
    unsigned long			avg_response_time;
    unsigned long			min_response_time;
    unsigned long			total_response_time;
    unsigned long			recv_msg_GET_count;
    unsigned long			recv_msg_POST_count;
    unsigned long			recv_msg_PUT_count;
    unsigned long			recv_msg_DELETE_count;
    unsigned long			recv_msg_METHOD_count;

    CMD_STATUS              GET_status;
    CMD_STATUS              PUT_status;
    CMD_STATUS              POST_status;
    CMD_STATUS              DELETE_status;
    CMD_STATUS              METHOD_status;

    int                     read_timeout;
    int                     write_timeout;
    int                     idle_timeout;
    int                     max_w_len;
} HTTP_t;

HTTP_t * HTTP_svr_init(int port, int backlog, int max_session, char *http_version);
void HTTP_svr_free(HTTP_t *hs);
int HTTP_svr_get_option(HTTP_t *hs, int option, char *output);
int HTTP_svr_set_option (HTTP_t *hs, int option, ...);
int HTTP_svr_open(HTTP_t *hs, int *sock, char *error);
int HTTP_svr_close(HTTP_t *hs, int (*fd_clear_callback)(int), char *error);

int HTTP_sess_open(HTTP_t *hs, HOST_EQUIV *authen, HSSESSION **sess, char *error);
int HTTP_sess_close(HTTP_t *hs, int no, int (*fd_clear_callback)(int), char *error);
int HTTP_sess_read(HTTP_t *hs, HSSESSION *sess, char *error);
int HTTP_sess_write(HTTP_t *hs, HSSESSION *sess, int (*fd_set_write_callback)(int), char *error);
int HTTP_sess_decode(HTTP_t *hs, HSSESSION *sess, char *error);
int HTTP_verify_connection(HTTP_t *hs, int (*fd_clear_callback)(int), char *error);

int http_gen_success_header(HSSESSION *sess, HSDATA **data);
int http_gen_created_header(HSSESSION *sess, HSDATA **data);
int http_gen_forbidden_page(HSSESSION *sess, HSDATA **data);
int http_gen_internal_error_page(HSSESSION *sess, HSDATA **data);
int http_gen_not_implemented_page(HSSESSION *sess, HSDATA **data);
int http_gen_busy_page(HSSESSION *sess, HSDATA **data);
int http_gen_gateway_timeout_page(HSSESSION *sess, HSDATA **data);
int http_gen_error_page(HSSESSION *sess, HSDATA **data, int status, const char *desc);
const char *http_method_to_string(int method);


#endif

