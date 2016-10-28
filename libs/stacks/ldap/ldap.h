
/*
 Copyright (C) 2014, TORO, MIMO Tech Co., Ltd.

 ldap.h

 Revision history
 1.0.2   Increased size of attribute value.
 1.0.1   (1) Added LDAP_RET_ERR_UNEXPECTED_RECEIVE in LDAP_RET_CODE enum.
         (2) Fixed bug coredump when received message without request before.
 1.0.0   Initialize.

 */
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>

#include "common.h"
#include "rauthen.h"

#ifndef _LDAP_H_
#define _LDAP_H_

#define LDAP_MAX_CLIENT                 1024
#define LDAP_MAX_BASE_DN                512
#define LDAP_MAX_STRING_GROUP           128
#define LDAP_MAX_STRING_HOST            128
#define LDAP_MAX_STRING_USERNAME        256
#define LDAP_MAX_STRING_PASSWORD        LDAP_MAX_STRING_USERNAME
#define LDAP_MAX_CONCURRENT_REQUEST     1024
#define LDAP_MAX_DIAG_MESSAGE           1024

#define LDAP_MAX_SESSION                1024

#define LDAP_MAX_DN_SIZE                1024
#define LDAP_MAX_OID_SIZE               1024
#define LDAP_MAX_ATTR_NAME_SIZE         256
#define LDAP_MAX_ATTR_VALUE_SIZE        8192 // (8 * 1024)
#define LDAP_MAX_STRING_SIZE            8192

#define LDAP_DEFAULT_GROUP              "DS" //Directory Server
#define LDAP_DEFAULT_CONCURRENT         16
#define LDAP_DEFAULT_KEEPALIVE_DATA     4096
#define LDAP_READ_BUFFER				131072 //(128*1024)
#define LDAP_WRITE_BUFFER				131072 //(128*1024)
#define LDAP_DATA_BLOCK					65536  //(64*1024)

#define BER_BUFFER                      131072 //(128*1024)
#define BER_APPLICATION_PRIMITIVE       0x40
#define BER_APPLICATION_CONSTRUCTED     0x60
#define BER_CONTEXT_PRIMITIVE           0x80
#define BER_CONTEXT_CONSTRUCTED         0xA0

#define MAX_SIZE_INDEXING               256
#define LDAP_CONTROL_USE_ALIAS_ON_UPDATE    "1.3.6.1.4.1.15573.1.2.1.2"

enum LBER_DECODE_FORMAT
{
    LBER_DECODE_PRINTABLE_STRING = 0,
    LBER_DECODE_JSON,
    LBER_DECODE_XML,

    LBER_DECODE_LAST
};

enum LDAP_KEEPALIVE_TYPE
{
    LDAP_KEEPALIVE_NONE = 0,
    LDAP_KEEPALIVE_SEARCH,
    LDAP_KEEPALIVE_EXTENDED,

    LDAP_KEEPALIVE_LAST
};

enum LDAP_PROTOCOL_OP
{
    LDAP_PROTOCOL_BIND_REQUEST = 0,
    LDAP_PROTOCOL_BIND_RESPONSE,
    LDAP_PROTOCOL_UNBIND_REQUEST,
    LDAP_PROTOCOL_SEARCH_REQUEST,
    LDAP_PROTOCOL_SEARCH_ENTRY,
    LDAP_PROTOCOL_SEARCH_DONE,
    LDAP_PROTOCOL_MODIFY_REQUEST,
    LDAP_PROTOCOL_MODIFY_RESPONSE,
    LDAP_PROTOCOL_ADD_REQUEST,
    LDAP_PROTOCOL_ADD_RESPONSE,
    LDAP_PROTOCOL_DELETE_REQUEST,
    LDAP_PROTOCOL_DELETE_RESPONSE,
    LDAP_PROTOCOL_MODIFYDN_REQUEST,
    LDAP_PROTOCOL_MODIFYDN_RESPONSE,
    LDAP_PROTOCOL_COMPARE_REQUEST,
    LDAP_PROTOCOL_COMPARE_RESPONSE,
    LDAP_PROTOCOL_ABANDON_REQUEST,
    LDAP_PROTOCOL_SEARCH_RESULT_REF = 19,
    LDAP_PROTOCOL_EXTENDED_REQUEST = 23,
    LDAP_PROTOCOL_EXTENDED_RESPONSE = 24,

    LDAP_PROTOCOL_LAST
};

enum LDAP_RESULT_CODE
{
    LDAP_RESULT_SUCCESS = 0,
    LDAP_RESULT_OPERATION_ERROR,
    LDAP_RESULT_PROTOCOL_ERROR,
    LDAP_RESULT_TIME_LIMIT_EXCEEDED,
    LDAP_RESULT_SIZE_LIMIT_EXCEEDED,
    LDAP_RESULT_COMPARE_FALSE,
    LDAP_RESULT_COMPARE_TRUE,
    LDAP_RESULT_AUTH_METHOD_NOT_SUPPORTED,
    LDAP_RESULT_STRONG_AUTH_REQUIRED,
    LDAP_RESULT_PARTIAL_RESULTS,
    LDAP_RESULT_REFERRAL,
    LDAP_RESULT_ADMIN_LIMIT_EXCEEDED,
    LDAP_RESULT_UNAVAILABLE_CRITICAL_EXTENSION ,
    LDAP_RESULT_CONFIDENTIALITY_REQUIRED,
    LDAP_RESULT_SASL_BIND_IN_PROGRESS,
    LDAP_RESULT_RESERVED_1,
    LDAP_RESULT_NO_SUCH_ATTRIBUTE,
    LDAP_RESULT_UNDEFINED_ATTRIBUTE_TYPE,
    LDAP_RESULT_INAPPROPRIATE_MATCHING,
    LDAP_RESULT_CONSTRAINT_VIOLATION,
    LDAP_RESULT_ATTRIBUTE_OR_VALUE_EXISTS,
    LDAP_RESULT_INVALID_ATTRIBUTE_SYNTAX,
    LDAP_RESULT_NO_SUCH_OBJECT = 32,
    LDAP_RESULT_ALIAS_PROBLEM,
    LDAP_RESULT_INVALID_DN_SYNTAX,
    LDAP_RESULT_IS_LEAF,
    LDAP_RESULT_ALIAS_DEREFERENCING_PROBLEM,
    LDAP_RESULT_INAPPROPRIATE_AUTHENTICATION = 48,
    LDAP_RESULT_INVALID_CREDENTIALS,
    LDAP_RESULT_INSUFFICIENT_ACCESS_RIGHTS,
    LDAP_RESULT_BUSY,
    LDAP_RESULT_UNAVAILABLE,
    LDAP_RESULT_UNWILLING_TO_PERFORM,
    LDAP_RESULT_LOOP_DEFECT,
    LDAP_RESULT_NAMING_VIOLATION = 64,
    LDAP_RESULT_OBJECT_CLASS_VIOLATION,
    LDAP_RESULT_NOT_ALLOWED_ON_NONLEAF,
    LDAP_RESULT_NOT_ALLOWED_ON_RDN,
    LDAP_RESULT_ENTRY_ALREADY_EXISTS,
    LDAP_RESULT_OBJECT_CLASS_MODS_PROHIBITED,
    LDAP_RESULT_RESERVED_2,
    LDAP_RESULT_AFFECTS_MULTIPLE_DSAS,
    LDAP_RESULT_OTHER = 80,
    LDAP_RESULT_SERVER_DOWN,
    LDAP_RESULT_LDAP_TIMEOUT = 85,
    LDAP_RESULT_PARAM_ERROR = 89,
    LDAP_RESULT_CONNECT_ERROR = 91,
    LDAP_RESULT_LDAP_NOT_SUPPORTED,
    LDAP_RESULT_CONTROL_NOT_FOUND,
    LDAP_RESULT_NO_RESULTS_RETURNED,
    LDAP_RESULT_MORE_RESULTS_TO_RETURN,
    LDAP_RESULT_CLIENT_LOOP,
    LDAP_RESULT_REFERRAL_LIMIT_EXCEEDED,

    //User define Return Code 2xx
    LDAP_RESULT_CLOSED_CONNECTION = 200,
    LDAP_RESULT_PACKAGE_EXCEEDED = 201,
    LDAP_RESULT_PACKAGE_FULL = 202,
    
    LDAP_RESULT_LAST
};

enum LDAP_RET_CODE
{
    LDAP_RET_ERR_UNKNOWN = -1000,
    LDAP_RET_ERR_PARAMETER,
    LDAP_RET_ERR_CONNECT,
    LDAP_RET_ERR_MAX_SESSION,
    LDAP_RET_ERR_LDAPVERSION,
    LDAP_RET_ERR_AUTHEN_METHOD,
    LDAP_RET_ERR_AUTHEN,
    LDAP_RET_ERR_IDLE_TIMEOUT,
    LDAP_RET_ERR_UNAVAILABLE,
    LDAP_RET_ERR_MEMORY,
    LDAP_RET_ERR_SEND,
    LDAP_RET_ERR_RECEIVE,
    LDAP_RET_ERR_ENCODE,
    LDAP_RET_ERR_DECODE,
    LDAP_RET_ERR_KEEPALIVE,
    LDAP_RET_ERR_UNEXPECTED_RECEIVE,
    LDAP_RET_ERR_CLOSED,

    LDAP_RET_OK = 0,
    LDAP_RET_DATA_AVAILABLE,
    LDAP_RET_WAIT_MORE_DATA,
    LDAP_RET_KEEPALIVE,
    LDAP_RET_MAX_CONNECTION,
    LDAP_RET_READY,
    LDAP_RET_SENT,
    LDAP_RET_SENT_FRAGMENT,
    LDAP_RET_RECV_UNBIND,

    LDAP_RET_LAST
};

enum LDAP_CLNT_OPTION
{
    LDAP_CLNT_OPTION_NUMBER = 0,
    LDAP_CLNT_OPTION_GROUP,
    LDAP_CLNT_OPTION_HOST,
    LDAP_CLNT_OPTION_PORT,
    LDAP_CLNT_OPTION_BASEDN,
    LDAP_CLNT_OPTION_USERNAME,
    LDAP_CLNT_OPTION_PASSWORD,
    LDAP_CLNT_OPTION_MAX_CLIENT,
    LDAP_CLNT_OPTION_MAX_CONCURRENT,
    LDAP_CLNT_OPTION_MAX_WBUFFER,
    LDAP_CLNT_OPTION_CONNECT_TIMEOUT,
    LDAP_CLNT_OPTION_WAIT_TIMEOUT,
    LDAP_CLNT_OPTION_READ_TIMEOUT,
    LDAP_CLNT_OPTION_WRITE_TIMEOUT,
    LDAP_CLNT_OPTION_KEEPALIVE_TYPE,
    LDAP_CLNT_OPTION_KEEPALIVE_INTERVAL,
    LDAP_CLNT_OPTION_KEEPALIVE_DN,
    LDAP_CLNT_OPTION_KEEPALIVE_OID,
    LDAP_CLNT_OPTION_READY,

    LDAP_CLNT_OPTION_CLIENT_COUNT,
    LDAP_CLNT_OPTION_QUEUE_COUNT,
    LDAP_CLNT_OPTION_SENT_COUNT,
    LDAP_CLNT_OPTION_RECV_COUNT,
    LDAP_CLNT_OPTION_ERROR_COUNT,

    LDAP_CLNT_OPTION_MAX_RESPONSE,
    LDAP_CLNT_OPTION_MIN_RESPONSE,
    LDAP_CLNT_OPTION_AVG_RESPONSE,

    LDAP_CLNT_OPTION_RESERVED
};

enum LDAP_SERV_OPTION
{
    LDAP_SERV_OPTION_PORT = 0,
    LDAP_SERV_OPTION_BACKLOG,
    LDAP_SERV_OPTION_MAX_SESSION,
    LDAP_SERV_OPTION_MAX_CONCURRENT,
    LDAP_SERV_OPTION_CONNECT_TIMEOUT,
    LDAP_SERV_OPTION_IDLE_TIMEOUT,
    LDAP_SERV_OPTION_READ_TIMEOUT,
    LDAP_SERV_OPTION_WRITE_TIMEOUT,
    LDAP_SERV_OPTION_MAX_WRITE_LENGHT,
    LDAP_SERV_OPTION_LISTENER,

    LDAP_SERV_OPTION_SESSION_COUNT,
    LDAP_SERV_OPTION_SENT_COUNT,
    LDAP_SERV_OPTION_RECV_COUNT,
    LDAP_SERV_OPTION_ERROR_COUNT,
    LDAP_SERV_OPTION_BUSY_COUNT,
    
    LDAP_SERV_OPTION_MAX_RESPONSE,
    LDAP_SERV_OPTION_MIN_RESPONSE,
    LDAP_SERV_OPTION_AVG_RESPONSE,

    LDAP_SERV_OPTION_RESERVED
};


enum LDAP_MODIFY_TYPE
{
    LDAP_MODIFY_ADD = 0,
    LDAP_MODIFY_DELETE,
    LDAP_MODIFY_REPLACE,

    LDAP_MODIFY_RESERVED
};

enum LDAP_CTRL_FLAG
{
    LDAP_CTRL_ENABLE = 0,
    LDAP_CTRL_DISABLE,

    LDAP_CTRL_RESERVED
};

/* DO NOT CHANGE ORDER */
enum LDAP_DEREF_TYPE{
    LDAP_DEREF_NEVER = 0,
    LDAP_DEREF_SEARCH,
    LDAP_DEREF_FIND,
    LDAP_DEREF_ALWAYS,

    LDAP_DEREF_TYPE_LAST
};

enum LDAP_CLNT_STATE
{
    LDAP_CLNT_STATE_OPEN = 0,
    LDAP_CLNT_STATE_CONNECTING,
    LDAP_CLNT_STATE_READY,
    LDAP_CLNT_STATE_WAITING,
    //LDAP_CLNT_STATE_WRITING,
    //LDAP_CLNT_STATE_READING,

    LDAP_CLNT_STATE_LAST
};

enum LDAP_SERV_STATE
{
    LDAP_SERV_STATE_OPEN = 0,
    LDAP_SERV_STATE_READY,
    LDAP_SERV_STATE_WAITING,
    //LDAP_SERV_STATE_WRITING,
    //LDAP_SERV_STATE_READING,

    LDAP_SERV_STATE_LAST
};

typedef struct _ber_
{
    struct _ber_        *prev;
    struct _ber_        *next;
    int                 size;
    int                 len;
    char                berval[1];
} BER;

typedef struct _ber_value_list_
{
    struct _ber_value_list_ *prev;
    struct _ber_value_list_ *next;
    int                 len;
    char                value[LDAP_MAX_ATTR_VALUE_SIZE];
} BER_VALLIST;

typedef struct _ber_attr_list_
{
    struct _ber_attr_list_ *prev;
    struct _ber_attr_list_ *next;
    int                 len;
    char                name[LDAP_MAX_ATTR_NAME_SIZE];
    BER_VALLIST         *vals;
} BER_ATTRLIST;

typedef struct _ber_mod_list_
{
    struct _ber_mod_list_ *prev;
    struct _ber_mod_list_ *next;
    int                 operation;
    int                 len;
    char                name[LDAP_MAX_ATTR_NAME_SIZE];
    BER_VALLIST         *vals;
} BER_MODLIST;

typedef struct _keep_alive_
{
    struct _keep_alive_ *prev;
    struct _keep_alive_ *next;

    char                oid[LDAP_MAX_OID_SIZE];
    char                dn[LDAP_MAX_DN_SIZE];

    int                 type;
    int                 interval;

    int                 blen;
    char                bulk[LDAP_DEFAULT_KEEPALIVE_DATA];
} LDAP_KEEPALIVE;

typedef struct _lclnt_
{
    struct _lclnt_      *prev;
    struct _lclnt_      *next;
    void                *group;
    int                 no;
    int                 sock;
    int                 local_port;
    int                 state;

    char                *w_buffer;
    int                 w_size;
    int                 w_curr;
    int                 w_len;

    char                *r_buffer;
    int                 r_len;
    int                 r_size;
    int                 r_ber_body;
    int                 r_ber_body_length;

    time_t              create_time;
    time_t              wait_time;
    time_t              read_time;
    time_t              write_time;
    time_t              keepalive_time;
    struct timeval      timestamp_read;
    struct timeval      timestamp_write;

    int                 concurrent_count;

    unsigned int        tid;
    unsigned int        ktid; // TID for keepalive
    BER                 *ber;
    BER                 *rber;
    BER                 *bstring;
    int                 result_code;
    char                matchedDN[LDAP_MAX_BASE_DN];
    char                diagnosticMessage[LDAP_MAX_DIAG_MESSAGE];
} LDAP_CLNT;

typedef struct _ldap_
{
    int                 no;
    char                group[LDAP_MAX_STRING_GROUP];
    char                host[LDAP_MAX_STRING_HOST];
    int                 port;
    char                baseDN[LDAP_MAX_BASE_DN];
    char                username[LDAP_MAX_STRING_USERNAME];
    char                password[LDAP_MAX_STRING_PASSWORD];

    int                 max_client;
    int                 max_concurrent;
    int                 max_wbuffer;

    int                 wait_timeout;
    int                 connect_timeout;
    int                 read_timeout;
    int                 write_timeout;

    unsigned long       total_response_time;
    unsigned long       max_response_time;
    unsigned long       avg_response_time;
    unsigned long       min_response_time;

    int                 client_count;
    int                 queue_count;
    int                 sent_count;
    int                 recv_count;
    int                 error_count;
    LDAP_CLNT           *client_array[LDAP_MAX_CLIENT];
    LDAP_CLNT           *ready_queue;
    LDAP_CLNT           *wait_queue;
    BER                 *send_msg_queue;
    int                 tid;
    LDAP_KEEPALIVE      heartbeat;
} LDAP_t;

// This structure was introduced during server implementation,
// it should be used in client implementation either.
enum LDAP_SEARCH_SCOPE
{
    BASE_OBJECT = 0,
    SINGLE_LEVEL,
    WHOLE_SUBTREE
};

enum LDAP_DEREF_ALIASES
{
    DEREF_NEVER = 0,
    DEREF_INSEARCHING,
    DEREF_FINDING_BASEOBJ,
    DEREF_ALWAYS
};

typedef struct _substrings_
{
    struct _substrings_ *prev;
    struct _substrings_ *next;
    int                         choice;
#define SUBSTRING_INITIAL       0
#define SUBSTRING_ANY           1
#define SUBSTRING_FINAL         2

    union
    {
        BER_VALLIST             initial;
        BER_VALLIST             any;
        BER_VALLIST             final;
    } alt;
} Substrings;

typedef struct _substring_filter_
{
    int flag[2];
#define SUBSTRING_TYPE          0
#define SUBSTRING_VALUE         1
    struct
    {
        BER_VALLIST             type;

        struct
        {
            unsigned int        elementCount;
            Substrings          *array_p;
        } substrings;
    } comps;
} SubstringFilter;

typedef struct _matching_rule_assertion_
{
    int flag[4];
#define ASSERT_MATCHINGRULE     0
#define ASSERT_TYPE             1
#define ASSERT_MATCHVALUE       2
#define ASSERT_DNATTRIBUTES     3
    struct
    {
        BER_VALLIST             matching_rule;
        BER_VALLIST             type;
        BER_VALLIST             match_value;
        int                     dn_attributes;
    } comps;
} MatchingRuleAssertion;

typedef struct _filter_
{
    struct _filter_ *prev;
    struct _filter_ *next;
    int    choice;
#define FAND                    0
#define FOR                     1
#define FNOT                    2
#define FEQUALITYMATCH          3
#define FSUBSTRINGS             4
#define FGREATEROREQUAL         5
#define FLESSOREQUAL            6
#define FPRESENT                7
#define FAPPROXMATCH            8
#define FEXTENSIBLEMATCH        9

    union
    {
        struct
        {
            unsigned int        elementCount;
            struct _filter_     *array_p;

        } and;

        struct
        {
            unsigned int        elementCount;
            struct _filter_     *array_p;
        } or;

        struct _filter_         *not;
        BER_ATTRLIST            equalityMatch;
        SubstringFilter         substrings;
        BER_ATTRLIST            greaterOrEqual;
        BER_ATTRLIST            lessOrEqual;
        BER_VALLIST             present;
        BER_ATTRLIST            approxMatch;
        MatchingRuleAssertion   extensibleMatch;

    } alt;
} SearchFilter;

typedef struct _bind_request_
{
    long                version;
    char                name[LDAP_MAX_STRING_USERNAME];
    // support only simple authentication
    char                password[LDAP_MAX_STRING_PASSWORD];
} BindRequest;

typedef struct _search_request_
{
    int                 flags[8];
#define BASEOBJECT                  0
#define SCOPE                       1
#define DEREFALIASES                2
#define SIZELIMIT                   3
#define TIMELIMIT                   4
#define TYPESONLY                   5
#define FILTER                      6
#define ATTRIBUTES                  7

    struct
    {
        char            base_object[LDAP_MAX_DN_SIZE];
        int             scope;
        int             deref_aliases;
        long            size_limit;
        long            time_limit;
        int             typesOnly;
        SearchFilter    filter;

        BER_VALLIST     *attributes;

    } comps;
} SearchRequest;

typedef struct _change_type_
{
    struct _change_type_    *prev;
    struct _change_type_    *next;
    int                 flags[2];
#define OPERATION                   0
#define MODIFICATION                1

    struct
    {
        int             operation;
#define ADD                         0
#define DELETE                      1
#define REPLACE                     2

        BER_ATTRLIST    *modification;
    } comps;
} ChangeType;

typedef struct _modify_request_
{
    int                 flags[2];
#define OBJECT                      0
#define CHANGETYPE                  1

    struct
    {
        char            object[LDAP_MAX_OID_SIZE];
        ChangeType      *changes;
    } comps;
} ModifyRequest;

typedef struct _add_request_
{
    int                 flags[2];
#define ADD_ENTRY                   0
#define ADD_ATTRIBUTES              1

    struct
    {
        char            entry[LDAP_MAX_OID_SIZE];
        BER_ATTRLIST    *attributes;
    }      comps;

} AddRequest;

typedef struct _delete_request_
{
    char                ldap_dn[LDAP_MAX_OID_SIZE];
} DeleteRequest;

typedef struct _moddn_request_
{
    struct
    {
        char            entry[LDAP_MAX_OID_SIZE];
        char            new_rdn[LDAP_MAX_DN_SIZE];
        char            new_superior[LDAP_MAX_DN_SIZE];
        unsigned int    delete_old_rdn;
    } comps;

} ModDnRequest;

typedef struct _extended_request_
{
    int                 flags[2];
#define REQUESTNAME                 0
#define REQUESTVALUE                1

    struct
    {
        char            request_name[LDAP_MAX_OID_SIZE];
        BER             *request_value;
    } comps;
} ExtendedRequest;

typedef struct _control_
{
    struct _control_       *prev;
    struct _control_       *next;

    int    flags[3];
#define CONTROLTYPE                 0
#define CRITICALITY                 1
#define CONTROLVALUE                2

    struct
    {
        char                control_type[LDAP_MAX_OID_SIZE];
        int                 criticality;
        BER_VALLIST         control_value;
    } comps;

} Control;

typedef struct _ldap_message_
{
    struct _ldap_message_	*prev;
    struct _ldap_message_	*next;
    int                 choice; //LDAP_PROTOCOL_OP
	unsigned int		message_id;
    union
    {
        BindRequest     bind_request;
        SearchRequest   search_request;
        ModifyRequest   modify_request;
        AddRequest      add_request;
        ModDnRequest    mod_dn_request;
        DeleteRequest   delete_request;
        ExtendedRequest extended_request;
    } alt;
    Control             *controls;
} LDAPMessage_t;

typedef struct _ldap_user_
{
    struct _ldap_user_  *prev;
    struct _ldap_user_  *next;

    char                name[LDAP_MAX_STRING_USERNAME];
    char                password[LDAP_MAX_STRING_PASSWORD];
} DSUSER;

typedef struct _ldap_session_
{
    unsigned int        version;
    int					no;
    int					sock;
    char				remote_ip[40];
    unsigned short		remote_port;
    DSUSER              user;
    int                 state;
    int                 concurrent_count;

    char				*w_buffer;
    int                 w_size;
    int                 w_curr;
    int                 w_len;

    char				*r_buffer;
    int                 r_size;
    int                 r_len;
    int                 r_ber_body;
    int                 r_ber_body_length;

    LDAPMessage_t       *rdata;

    //no wait time_t state_time;
    time_t				read_time;
    time_t				write_time;
    time_t				active_time;//no wait

    int					error_response;

    struct timeval      timestamp_read;
    struct timeval      timestamp_write;
    long long           total_wait_write_io;
    long long           total_wait_read_io;
    unsigned long       recv_count;
    unsigned long       write_count;
    int                 format;
} DSSESSION;

typedef struct _directory_server_
{
    /* authentication */
    HOST_EQUIV          *h_equiv;

    /* directory server */
    unsigned int        version;
    char                baseDN[LDAP_MAX_BASE_DN];
    DSUSER              *user_list;
    int					port;
    int					listen_sock;
    int					backlog;
    int                 max_session;
    int                 max_concurrent;
    DSSESSION           session[LDAP_MAX_SESSION];
    int					session_count;
    int					last_session;

    unsigned long		recv_msg_count;
    unsigned long		sent_msg_count;
    unsigned long		error_msg_count;
    unsigned long		busy_msg_count;
    unsigned long		max_response_time;
    unsigned long		avg_response_time;
    unsigned long		min_response_time;
    unsigned long		total_response_time;

    int                 read_timeout;
    int                 write_timeout;
    int                 idle_timeout;
    int                 max_w_len;
} DS_t;


typedef struct _ldap_object
{
    struct _ldap_object *next;
    struct _ldap_object *prev;
    char                name[LDAP_MAX_ATTR_NAME_SIZE];             //rdn name
    char                value[LDAP_MAX_ATTR_VALUE_SIZE];           //rdn value
    char                dn[4096];                                  //DN
    char                object_class[LDAP_MAX_ATTR_NAME_SIZE];     //objectClass
    char                error[LDAP_MAX_ATTR_VALUE_SIZE];           //error message
    BER_ATTRLIST        *alist;                            //attibute list
    BER_ATTRLIST        *atable[MAX_SIZE_INDEXING];        //make index for attibute
} LDAP_OBJECT;

LDAP_t * LDAP_clnt_init (const char *group, const char *dn, const char *username, const char *password, const char *host, int port, int wbuffer, int concurrent, int max_client);
void LDAP_clnt_free (LDAP_t *dc);
int LDAP_clnt_get_option (LDAP_t *dc, int option, char *output);
int LDAP_clnt_set_option (LDAP_t *dc, int option, ...);
int LDAP_clnt_open (LDAP_t *dc, LDAP_CLNT **lc, char *error);
int LDAP_clnt_close (LDAP_t *dc, int no, char *error);
int LDAP_clnt_read (LDAP_t *dc, LDAP_CLNT *lc, char *error);
int LDAP_clnt_write (LDAP_t *dc, LDAP_CLNT *lc, char *error);
int LDAP_clnt_send_message (LDAP_t *dc, LDAP_CLNT *lc, BER *ber, int (*fd_set_write_callback)(int), char *error);
int LDAP_clnt_keep_alive (LDAP_t *dc, char *error);
int LDAP_clnt_check_connection (LDAP_t *dc, char *error);
void LDAP_clnt_close_all(LDAP_t *dc, char *error);

DS_t * LDAP_svr_init (const char *dn, int port, int backlog, int max_session, int max_concurrent);
void LDAP_svr_free (DS_t *ds);
int LDPA_svr_user_add (DS_t *ds, const char *username, const char *password);
int LDAP_svr_get_option (DS_t *ds, int option, char *output);
int LDAP_svr_set_option (DS_t *ds, int option, ...);
int LDAP_svr_open (DS_t *ds, int *sock, char *error);
int LDAP_svr_close (DS_t *ds, int (*fd_clear_callback)(int), char *error);
int LDAP_sess_open (DS_t *ds, HOST_EQUIV *authen, DSSESSION **sess, char *error);
int LDAP_sess_close (DS_t *ds, int no, int (*fd_clear_callback)(int), char *error);
int LDAP_sess_examine (DS_t *ds, int (*fd_clear_callback)(int), char *error);
int LDAP_sess_read (DS_t *ds, DSSESSION *sess, char *error);
int LDAP_sess_write (DS_t *ds, DSSESSION *sess, int (*fd_set_write_callback)(int), char *error);
int LDAP_sess_decode (DS_t *ds, DSSESSION *sess, char *error);
int LDAP_sess_send_message (DS_t *ds, DSSESSION *sess, BER *ber, int (*fd_set_write_callback)(int), char *error);

int LDAP_gen_search_request (LDAP_t *dc, BER **ber, char *dn, char *scope, char *object_input, char *attrs, int alen, int deref_value, Control *control, char *error);
int LDAP_gen_search_entry (DS_t *ds, BER **ber, int mid, char *object, char *attrs, int alen, char *error);
int LDAP_gen_search_done (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error);
int LDAP_gen_add_request (LDAP_t *dc, BER **ber, char *dn, char *attrs, int alen, Control *control, char *error);
int LDAP_gen_add_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error);
int LDAP_gen_modify_request (LDAP_t *dc, BER **ber, char *dn, char *attrs, int alen, Control *control, char *error);
int LDAP_gen_modify_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error);
int LDAP_gen_moddn_request (LDAP_t *dc, BER **ber, char *dn, char *rdn, int delete_flag, char *sup, Control *control, char *error);
int LDAP_gen_moddn_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error);
int LDAP_gen_delete_request (LDAP_t *dc, BER **ber, char *dn, Control *control, char *error);
int LDAP_gen_delete_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error);
int LDAP_gen_extended_request (LDAP_t *dc, BER **ber, char *oid, char *ext_attr, int elen, Control *control, char *error);
int LDAP_gen_extended_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *oid, char *ext_attr, int elen, char *error);
int LDAP_gen_extended_common_request (LDAP_t *dc, BER **ber, char *oid, char *ext_attr, int elen, Control *control, char *error);
int LDAP_gen_extended_common_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *oid, char *ext_attr, int elen, char *error);
int LDAP_gen_ber_string (char attrs[][128], BER **ber, char *error);
int LDAP_gen_ber_attrlist (BER_ATTRLIST *attrlist, BER **ber, int free_flag, char *error);
int LDAP_gen_ber_modlist (BER_MODLIST *modlist, BER **ber, int free_flag, char *error);

void LDAP_ber_free (BER *ber);

int LDAP_clnt_decode (LDAP_t *dc, LDAP_CLNT *lc, char *error);
int LDAP_clnt_string_free (LDAP_CLNT *lc, char *error);

int LDAP_filter_to_string(SearchFilter *filter, char *expression);
int LDAP_string_to_filter(char *expression, SearchFilter *filter);

int LBER_object_decode (LDAP_CLNT *lc, LDAP_OBJECT **obj, char *error);
int LBER_attr_decode (LDAP_CLNT *lc, int format, char *error);
int LBER_attrlist_add (BER_ATTRLIST **attrlist, char name[LDAP_MAX_ATTR_NAME_SIZE], BER_VALLIST *vallist, char *error);
int LBER_modlist_add (BER_MODLIST **modlist, int operation, char name[LDAP_MAX_ATTR_NAME_SIZE], BER_VALLIST *vallist, char *error);
int LBER_valuelist_add (BER_VALLIST **vallist, char value[LDAP_MAX_ATTR_VALUE_SIZE], char *error);
int ber_to_attrs(BER *ber, BER_ATTRLIST **alists, char *error);

#endif

