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

int
timeval_subtract (struct timeval *result, struct timeval *x,struct timeval *y)
{
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    return x->tv_sec < y->tv_sec;
}

int
tid_increment(LDAP_t *dc)
{
    int tid;
    if (dc==NULL)
    {
        SDF_random(&tid, sizeof(tid)-1);
        if (tid == 0) ++(tid);
        return tid;
    }

    tid = dc->tid;
    ++(tid);
    if (tid == 0) ++(tid);
    if (tid >= 0x7FFFFFFF) tid = 1;
    dc->tid = tid;

    return tid;
}

int
ldap_send_bind_rqt(LDAP_t *dc, LDAP_CLNT *lc)
{
    char msgElem, msgLen = 0, usrLen, pwdLen;
    int ldapIndex, msgIndex, opIndex;
    char b_bind[128];
    int i = 0, r, tid;

    if ((dc==NULL)||(lc==NULL)) return LDAP_RET_ERR_PARAMETER;

    tid = tid_increment(dc);

    b_bind[i++] = 0x30;
    ldapIndex = i++;

    b_bind[i++] = 0x02;
    msgIndex = i++;
    for (r = 3; r >= 0; r--)
    {
        msgElem = (char)(tid >> (8 * r));
        if (msgElem > 0)
        {
            b_bind[i++] = msgElem;
            msgLen++;
        }
    }
    b_bind[msgIndex] = msgLen;

    b_bind[i++] = 0x60;
    opIndex = i++;

    b_bind[i++] = 0x02;
    b_bind[i++] = 0x01;
    b_bind[i++] = 0x03;

    b_bind[i++] = 0x04;
    usrLen = strlen(dc->username);
    b_bind[i++] = (char)usrLen;
    memcpy((b_bind + i), dc->username, usrLen);
    i += usrLen;

    b_bind[i++] = 0x80;
    pwdLen = strlen(dc->password);
    b_bind[i++] = pwdLen;
    memcpy((b_bind + i), dc->password, pwdLen);
    i += pwdLen;

    b_bind[opIndex] = i - (opIndex + 1);
    b_bind[ldapIndex] = i - (ldapIndex + 1);

    r = (int) send(lc->sock, b_bind, i, 0);
    if (r <= 0)
    {
        return -1;
    }

    return LDAP_RET_OK;
}

int
ldap_send_bind_resp(DS_t *ds, DSSESSION *sess, int result_code)
{
    unsigned char msgElem, msgLen = 0;
    int ldapIndex, msgIndex;
    char b_bind[128];
    int i = 0, r;

    if ((ds==NULL)||(sess==NULL)||(sess->rdata==NULL)) return LDAP_RET_ERR_PARAMETER;

    b_bind[i++] = 0x30;
    ldapIndex = i++;

    b_bind[i++] = 0x02;
    msgIndex = i++;
    if (sess->rdata->message_id < 0x100) r = 0;
    else if (sess->rdata->message_id < 0x10000) r = 1;
    else if (sess->rdata->message_id < 0x1000000) r = 2;
    else r = 3;
    for (; r >= 0; r--)
    {
        msgElem = (unsigned char)(sess->rdata->message_id >> (8 * r));
        b_bind[i++] = msgElem;
        msgLen++;
    }
    SDF_FREE(sess->rdata);
    sess->rdata = NULL;

    b_bind[msgIndex] = msgLen;

    b_bind[i++] = 0x61; // bindResponse
    b_bind[i++] = 0x07;
    b_bind[i++] = 0x0A; // resultCode
    b_bind[i++] = 0x01;
    b_bind[i++] = result_code;
    b_bind[i++] = 0x04; // matchedDN
    b_bind[i++] = 0x00;
    b_bind[i++] = 0x04; // errorMessage
    b_bind[i++] = 0x00;

    b_bind[ldapIndex] = i - (ldapIndex + 1);

    r = (int) send(sess->sock, b_bind, i, 0);
    if (r <= 0)
    {
        return -1;
    }

    SDF_FREE(sess->rdata);
    sess->rdata = NULL;
    return LDAP_RET_OK;
}

int
ldap_recv_bind_rqt(DS_t *ds, DSSESSION *sess)
{
    char buff[512];
    int j, r, i = 0, authen = 0;
    unsigned int msg_id = 0, msg_id_len, len;
    DSUSER *user=NULL;

    if ((ds==NULL)||(sess==NULL)) return LDAP_RET_ERR_PARAMETER;

    r = (int) recv(sess->sock, buff, 512, 0);
    if (r < 0)
    {
        return -1;
    }
    else if (r == 0)
    {
        return -1;
    }
    if (buff[i++] == 0x30)
    {
        i++;
        if (buff[i++] == 0x02)
        {
            sess->rdata = (LDAPMessage_t *) SDF_MALLOC(sizeof(LDAPMessage_t));
            if (sess->rdata==NULL) return LDAP_RET_ERR_MEMORY;
            msg_id_len = buff[i++];
            j = i;
            i += msg_id_len;
            while (msg_id_len > 0)
            {
                --msg_id_len;
                msg_id |= (unsigned int)(((unsigned char)buff[j]) << (8 * msg_id_len));
                j++;
            }
            sess->rdata->choice = LDAP_PROTOCOL_BIND_REQUEST;
            sess->rdata->message_id = msg_id;
            if (buff[i++] == 0x60)
            {
                i += 3; // bindRqt len + int + int len
                if (buff[i++] != 3)
                {
                    return LDAP_RET_ERR_LDAPVERSION;
                }
                sess->rdata->alt.bind_request.version = buff[i-1];
                if (buff[i++] == 0x04) // name
                {
                    len = buff[i++];
                    memcpy(sess->user.name, &(buff[i]), len);
                    sess->user.name[len] = 0;
                    i += len;
                    if ((unsigned char) buff[i++] == 0x80) // simple authentication
                    {
                        len = buff[i++];
                        memcpy(sess->user.password, &(buff[i]), len);
                        sess->user.password[len] = 0;
                        user = ds->user_list;
                        authen = 0;
                        do {
                            if ((strcmp(sess->user.name, user->name)==0) &&
                                (strcmp(sess->user.password, user->password)==0))
                            {
                                authen = 1;
                                break;
                            }
                            user = user->next;
                        } while (user!=ds->user_list);
                        if (authen == 0) return LDAP_RET_ERR_AUTHEN;

                        return LDAP_RET_OK;
                    }
                    else
                    {
                        return LDAP_RET_ERR_AUTHEN_METHOD;
                    }
                }
            }
        }
    }

    return -1;
}

int
ldap_recv_bind_resp(LDAP_t *dc, LDAP_CLNT *lc)
{
    char buff[128];
    int j, r, i = 0;
    int msg_id = 0, msg_id_len;

    if ((dc==NULL)||(lc==NULL)) return LDAP_RET_ERR_PARAMETER;

    r = (int) recv(lc->sock, buff, 128, 0);
    if (r < 0)
    {
        return -1;
    }
    else if (r == 0)
    {
        return -1;
    }
    if (buff[i++] == 0x30)
    {
        i++;
        if (buff[i++] == 0x02)
        {
            msg_id_len = buff[i++];
            j = i;
            i += msg_id_len;
            while (msg_id_len > 0)
            {
                --msg_id_len;
                msg_id += (int)(((unsigned char)buff[j]) << (8 * msg_id_len));
                j++;
            }
            if (buff[i++] == 0x61)
            {
                i++;
                if (buff[i++] == 0x0A)
                {
                    i++;
                    if (buff[i++] == 0x00)
                    {
                        return LDAP_RET_OK;
                    }
                }
            }
        }
    }
    
    return -1;
}

int
ldap_send_generic_resp(DS_t *ds, DSSESSION *sess, int result_code, LDAPMessage_t *msg)
{
    BER *ber=NULL;
    int r;

    if ((ds==NULL)||(sess==NULL)||(msg==NULL)) return LDAP_RET_ERR_PARAMETER;

    switch (msg->choice) {
        case LDAP_PROTOCOL_SEARCH_REQUEST:
            LDAP_gen_search_done(ds, &ber, msg->message_id, result_code, NULL, NULL, NULL);
            break;
        
        case LDAP_PROTOCOL_MODIFY_REQUEST:
            LDAP_gen_modify_response(ds, &ber, msg->message_id, result_code, NULL, NULL, NULL);
            break;
        
        case LDAP_PROTOCOL_ADD_REQUEST:
            LDAP_gen_add_response(ds, &ber, msg->message_id, result_code, NULL, NULL, NULL);
            break;
        
        case LDAP_PROTOCOL_DELETE_REQUEST:
            LDAP_gen_delete_response(ds, &ber, msg->message_id, result_code, NULL, NULL, NULL);
            break;
        
        case LDAP_PROTOCOL_MODIFYDN_REQUEST:
            LDAP_gen_moddn_response(ds, &ber, msg->message_id, result_code, NULL, NULL, NULL);
            break;

        case LDAP_PROTOCOL_EXTENDED_REQUEST:
            LDAP_gen_extended_response(ds, &ber, msg->message_id, result_code, NULL, NULL,
                                       msg->alt.extended_request.comps.request_name, NULL, 0, NULL);
            break;
        default:
            break;
    }

    r = (int) send(sess->sock, ber->berval, ber->len, 0);
    if (r <= 0)
    {
        return -1;
    }

    LDAP_ber_free(ber);
    return LDAP_RET_OK;
}

int
ldap_send_keepalive_rqt (LDAP_t *dc, LDAP_CLNT *lc, BER *ber, char *error)
{
    int len;
    char *buff;

    if (dc==NULL || lc==NULL || ber==NULL)
    {
        if (error!=NULL) sprintf(error, "invalid parameters");
        return LDAP_RET_ERR_PARAMETER;
    }

    if (lc->w_buffer == NULL)
    {
        if (error!=NULL) sprintf(error, "memory error");
        return LDAP_RET_ERR_MEMORY;
    }
    if (lc->w_len+ber->len > lc->w_size)
    {
        len = lc->w_len + ber->len;
        ADJUST_SIZE(len, LDAP_WRITE_BUFFER)
        buff = (char *) realloc (lc->w_buffer, len);
        if (buff == NULL)
        {
            if (error!=NULL) sprintf(error, "memory error");
            return LDAP_RET_ERR_MEMORY;
        }
        lc->w_size = len;
        lc->w_buffer = buff;
        (void) memset (lc->w_buffer+lc->w_len, 0, lc->w_size-lc->w_len);
    }
    (void) memcpy (lc->w_buffer+lc->w_len, ber->berval, ber->len);
    lc->w_len += ber->len;
    lc->write_time = time(NULL);

    free(ber);
    if (LDAP_clnt_write(dc, lc, error) != LDAP_RET_SENT)
    {
        return LDAP_RET_ERR_SEND;
    }
    --(dc->sent_count); // compensate for LDAP_clnt_write since keepalive message doesn't has to be counted.

    return LDAP_RET_OK;
}

int ldap_send_keepalive_resp (DS_t *ds, DSSESSION *sess, BER *ber, char *error)
{
    return LDAP_RET_OK;
}

char *
state_to_string(int state)
{
    static char buff[256];

    switch (state)
    {
        case LDAP_CLNT_STATE_CONNECTING:
            return "STATE_CONNECTING";
        case LDAP_CLNT_STATE_READY:
            return "STATE_READY";
        case LDAP_CLNT_STATE_WAITING:
            return "STATE_WAITING";
    }
    (void)sprintf(buff, "UNKNOWN_STATE(%d)", state);
    return buff;
}

char *
error_to_string(int result)
{
    static char buff[256];

    switch (result)
    {
        case LDAP_RESULT_OPERATION_ERROR:
            return "Operation Error";
        case LDAP_RESULT_PROTOCOL_ERROR:
            return "Protocol Error";
        case LDAP_RESULT_TIME_LIMIT_EXCEEDED:
            return "Time Limit Exceeded";
        case LDAP_RESULT_SIZE_LIMIT_EXCEEDED:
            return "Size Limit Exceeded";
        case LDAP_RESULT_COMPARE_FALSE:
            return "Compare False";
        case LDAP_RESULT_COMPARE_TRUE:
            return "Compare True";
        case LDAP_RESULT_AUTH_METHOD_NOT_SUPPORTED:
            return "Authentication Method Not Supported";
        case LDAP_RESULT_STRONG_AUTH_REQUIRED:
            return "Strong Authentication Required";
        case LDAP_RESULT_PARTIAL_RESULTS:
            return "Partial Results";
        case LDAP_RESULT_REFERRAL:
            return "Referral";
        case LDAP_RESULT_ADMIN_LIMIT_EXCEEDED:
            return "Admin Limit Exceeded";
        case LDAP_RESULT_UNAVAILABLE_CRITICAL_EXTENSION:
            return "Unavailable Critical Extension";
        case LDAP_RESULT_CONFIDENTIALITY_REQUIRED:
            return "Confidentaility Required";
        case LDAP_RESULT_SASL_BIND_IN_PROGRESS:
            return "SASL Bind in Progress";
        case LDAP_RESULT_NO_SUCH_ATTRIBUTE:
            return "No Such Attribute";
        case LDAP_RESULT_UNDEFINED_ATTRIBUTE_TYPE:
            return "Undefined Attribute Type";
        case LDAP_RESULT_INAPPROPRIATE_MATCHING:
            return "Inappropriate Matching";
        case LDAP_RESULT_CONSTRAINT_VIOLATION:
            return "Constaint Violation";
        case LDAP_RESULT_ATTRIBUTE_OR_VALUE_EXISTS:
            return "Attribute or Value Exists";
        case LDAP_RESULT_INVALID_ATTRIBUTE_SYNTAX:
            return "Invalid Attribute Syntax";
        case LDAP_RESULT_NO_SUCH_OBJECT:
            return "No Such Object";
        case LDAP_RESULT_ALIAS_PROBLEM:
            return "Alias Problem";
        case LDAP_RESULT_INVALID_DN_SYNTAX:
            return "Invalid DN Syntax";
        case LDAP_RESULT_IS_LEAF:
            return "Is Leaf";
        case LDAP_RESULT_ALIAS_DEREFERENCING_PROBLEM:
            return "Alias Dereferencing Problem";
        case LDAP_RESULT_INAPPROPRIATE_AUTHENTICATION:
            return "Inappropriate Authentication";
        case LDAP_RESULT_INVALID_CREDENTIALS:
            return "Invalid Credentials";
        case LDAP_RESULT_INSUFFICIENT_ACCESS_RIGHTS:
            return "Insufficient Access Rights";
        case LDAP_RESULT_BUSY:
            return "Busy";
        case LDAP_RESULT_UNAVAILABLE:
            return "Unavailable";
        case LDAP_RESULT_UNWILLING_TO_PERFORM:
            return "Unwilling to Perform";
        case LDAP_RESULT_LOOP_DEFECT:
            return "Loop Defect";
        case LDAP_RESULT_NAMING_VIOLATION:
            return "Naming Violation";
        case LDAP_RESULT_OBJECT_CLASS_VIOLATION:
            return "Object Class Violation";
        case LDAP_RESULT_NOT_ALLOWED_ON_NONLEAF:
            return "Not Allowed on Nonleaf";
        case LDAP_RESULT_NOT_ALLOWED_ON_RDN:
            return "Not Allowed on RDN";
        case LDAP_RESULT_ENTRY_ALREADY_EXISTS:
            return "Entry Already Exists";
        case LDAP_RESULT_OBJECT_CLASS_MODS_PROHIBITED:
            return "Object Class Mods Prohibited";
        case LDAP_RESULT_AFFECTS_MULTIPLE_DSAS:
            return "Affects Multiple DSAs";
        case LDAP_RESULT_OTHER:
            return "Other";
        case LDAP_RESULT_SERVER_DOWN:
            return "Server Down";
        case LDAP_RESULT_LDAP_TIMEOUT:
            return "LDAP Timeout";
        case LDAP_RESULT_PARAM_ERROR:
            return "Param Error";
        case LDAP_RESULT_CONNECT_ERROR:
            return "Connect Error";
        case LDAP_RESULT_LDAP_NOT_SUPPORTED:
            return "LDAP Not Supported";
        case LDAP_RESULT_CONTROL_NOT_FOUND:
            return "Control Not Found";
        case LDAP_RESULT_NO_RESULTS_RETURNED:
            return "No Results Returned";
        case LDAP_RESULT_MORE_RESULTS_TO_RETURN:
            return "More Results to Return";
        case LDAP_RESULT_CLIENT_LOOP:
            return "Client Loop";
        case LDAP_RESULT_REFERRAL_LIMIT_EXCEEDED:
            return "Referral Limit Exceeded";
    }
    (void)sprintf(buff, "UNKNOWN_LDAPERROR(%d)", result);
    return buff;
}


#define INSERT_LENGTH(_data, _i, _len)                  \
{                                                       \
if (_len < 128)                                         \
{                                                       \
    _data[_i++] = _len;                                 \
}                                                       \
else                                                    \
{                                                       \
    if (_len < 256)                                     \
    {                                                   \
        _data[_i++] = 0x80 + 1;                         \
        _data[_i++] = _len;                             \
    }                                                   \
    else                                                \
    {                                                   \
        r = 0;                                          \
        while ((_len >> (r * 8)) > 0)                   \
            r++;                                        \
        _data[_i++] = 0x80 + r;                         \
        while (r > 0)                                   \
        {                                               \
            r--;                                        \
            _data[_i++] = (char)(_len >> (r * 8));      \
        }                                               \
    }                                                   \
}                                                       \
}

#define FIND_LENGTH(_data, _len)                        \
{                                                       \
pp = 0;                                                 \
p = &(_data[i]);                                        \
if (p[0] > 0xffffff00)                                  \
{                                                       \
    register int _size = p[0] - 0xffffff80;             \
    i += _size;                                         \
    while (_size > 0)                                   \
    {                                                   \
        p++;                                            \
        if ((int)p[0] > 0xffffff00)                     \
            pp = pp + (int)p[0] - 0xffffff00;           \
        else                                            \
            pp = pp + (int)p[0];                        \
        if (_size > 1)                                  \
            pp = pp << (1 * 8);                         \
        _size--;                                        \
    }                                                   \
    _len = pp;                                          \
}                                                       \
else                                                    \
{                                                       \
    _len = p[0];                                        \
}                                                       \
i++;                                                    \
}

#define COPY_TO_BER(_opt, _len)                         \
{                                                       \
ber_alloc_buffer(ber, sum);                             \
(*ber)->len = 0;                                        \
memcpy((*ber)->berval, b_ldap, ldap_len);               \
(*ber)->len += ldap_len;                                \
memcpy((*ber)->berval + (*ber)->len, b_msg, msg_len);	\
(*ber)->len += msg_len;                                 \
memcpy((*ber)->berval + (*ber)->len, _opt, _len);		\
(*ber)->len += _len;                                    \
}

#define COPY_DN											\
{                                                       \
memcpy((*ber)->berval + (*ber)->len, b_dn, dn_len);     \
(*ber)->len += dn_len;                                  \
}

#define CREATE_LDAP                                     \
{                                                       \
b_ldap[ldap_len++] = 0x30;                              \
INSERT_LENGTH(b_ldap, ldap_len, sum)                    \
sum += ldap_len;                                        \
}

#define CREATE_DN                                       \
{                                                       \
b_dn[dn_len++] = 0x04;                                  \
r = (int)strlen(dn);                                    \
INSERT_LENGTH(b_dn, dn_len, r)                          \
memcpy(&b_dn[dn_len], dn, r);                           \
dn_len += r;                                            \
sum += dn_len;                                          \
}

void
bin_to_hex(char *buff, char *val, int vlen, int type)
{
    const char *hex_letter;
    int i, len = 0;
    uint8_t byte;
    if (type == LDAP_NUM_UHEX) {
        static const char letters[] = "0123456789ABCDEF";
        hex_letter = letters;
    } else {
        static const char letters[] = "0123456789abcdef";
        hex_letter = letters;
    }
    for (i = 0; i < vlen; i++) {
        byte = val[i];
        buff[len++] = hex_letter[byte >> 4];
        buff[len++] = hex_letter[byte & 0xF];
    }
    buff[len] = 0x0;
}

void
bin_to_bcd(char *buff, char *val, int vlen, int type)
{
    const char *hex_letter;
    int i, len = 0;
    uint8_t byte;
    if (type == LDAP_NUM_UBCD) {
        static const char letters[] = "0123456789ABCDEF";
        hex_letter = letters;
    } else {
        static const char letters[] = "0123456789abcdef";
        hex_letter = letters;
    }
    for (i = 0; i < vlen; i++, len += 2) {
        byte = val[i];
        buff[len] = hex_letter[byte & 0xF];
        buff[len+1] = hex_letter[byte >> 4];
    }
    buff[len] = 0x0;
}

int
ber_alloc_buffer(BER **ber, int len)
{
    if (*ber == NULL)
    {
        ADJUST_SIZE(len, BER_BUFFER)
        *ber = (BER *) malloc (sizeof(*ber)+len);
        if (*ber == NULL) return -1;
        //(void) memset ((*ber), 0, sizeof(*ber)+len);
        (*ber)->size = len;
        (*ber)->len = 0;
    }
    else if (((*ber)->len + len) > (*ber)->size)
    {
        BER *x;
        len += (*ber)->len;
        ADJUST_SIZE(len, BER_BUFFER)
        x = (BER *) realloc ((*ber), sizeof(*x)+len);
        if (x == NULL) return -1;
        x->size = len;
        *ber = x;
        //(void) memset ((*ber)->berval+(*ber)->len, 0, (*ber)->size-(*ber)->len);
    }
    return 0;
}

/*int
vals_decode(char *buff, char *val, int vlen, int type, char *error)
{
    if (type == LDAP_NUM_ASCII) {
        memcpy(buff, val, vlen);
        buff[vlen] = 0x0;
    } else if (type == LDAP_NUM_LHEX || type == LDAP_NUM_UHEX) {
        bin_to_hex(buff, val, vlen, type);
    } else if (type == LDAP_NUM_LBCD || type == LDAP_NUM_UBCD) {
        bin_to_bcd(buff, val, vlen, type);
    } else {
        if (error!=NULL)
            sprintf(error, "Non support decode type [%d]", type);
        return LDAP_RET_ERR_DECODE;
    }
    return LDAP_RET_OK;
}*/

int
ber_tid_create(long tid, char *b_msg)
{
    int i = 0, r = 0, d, min = 1, max = 1;

    b_msg[i++] = 0x02;
    while ((tid >> (r * 8)) > 0)
        r++;
    for (d = 1; d <= 8 * r - 1; d++)
        min *= 2;
    for (d = 1; d <= 8 * r; d++)
        max *= 2;
    if ((min <= tid) && (tid < max))
    {
        b_msg[i++] = r + 1;
        b_msg[i++] = 0;
    }
    else
    {
        b_msg[i++] = r;
    }
    while (r > 0)
    {
        r--;
        b_msg[i++] = tid >> (r * 8);
    }
    return i;
}

typedef struct _object_list_
{
    struct _object_list_ *prev;
    struct _object_list_ *next;

    char name[LDAP_MAX_DN_SIZE];
    char *string;
    int len;
    int size;
} olist_t;

int
ber_attr_to_obj(LDAP_CLNT *lc, LDAP_OBJECT **object, char *error)
{

    int i = 0, j = 0,pp = 0;
    int optLen, attrLen, partLen, valLen, bufLen;
    char attr[256];
    char buff[LDAP_MAX_DN_SIZE];
    char *p = NULL;
    if (lc->rber==NULL)
    {
        if (error!=NULL) strcpy(error, "buffer doesn't exist");
        return LDAP_RET_ERR_DECODE;
    }
    int len = lc->rber->len;
    char *data = lc->rber->berval;

    LDAP_OBJECT *obj= NULL, *tmp_obj;
    BER_VALLIST *vlist = NULL;
    BER_ATTRLIST *alist = NULL;

    while (len > 0)
    {
        j = i;
        if (data[i] == 0x64)      //opt tag
        {
            i++;
            FIND_LENGTH(data, optLen);
            len -= (optLen + i - j);
            if (data[i++] == 0x04)//object name
            {
                FIND_LENGTH(data, bufLen);
                memcpy(buff, &data[i], bufLen);
                buff[bufLen] = 0;
                i += bufLen;

                tmp_obj = (LDAP_OBJECT*)SDF_MALLOC(sizeof(LDAP_OBJECT));  //create new object
                sprintf(tmp_obj->name,"%s", buff);          //set object name
                tmp_obj->value[0] = '\0';

                if (data[i++] == 0x30)                      //sequence
                {
                    FIND_LENGTH(data, attrLen);
                    while (attrLen > 0)
                    {
                        j = i;
                        if (data[i++] == 0x30)              //sequence
                        {
                            FIND_LENGTH(data, partLen);
                            attrLen -= (partLen + i - j);
                            while (partLen > 0)
                            {
                                j = i;
                                if (data[i++] == 0x04)      //attr name
                                {
                                    FIND_LENGTH(data, bufLen);
                                    partLen -= (bufLen + i - j);
                                    memcpy(attr, &data[i], bufLen);
                                    attr[bufLen] = 0;
                                    i += bufLen;
                                    j = i;

                                    if (data[i++] == 0x31)  //set
                                    {
                                        FIND_LENGTH(data, valLen);
                                        partLen -= (valLen + i - j);

                                        vlist = NULL;
                                        while (valLen > 0)
                                        {
                                            j = i;
                                            if (data[i++] == 0x04)//attr val
                                            {
                                                FIND_LENGTH(data, bufLen);
                                                valLen -= (bufLen + i - j);
                                                memcpy(buff, &data[i], bufLen);
                                                buff[bufLen] = 0;
                                                i += bufLen;

                                                // create new vlist
                                                // ignore object class
                                                if(strcmp(attr, "objectClass") != 0){
                                                    LBER_valuelist_add(&vlist, buff, error);
                                                }
                                            }
                                        }
                                        if( strcmp(attr, "objectClass") == 0){
                                            sprintf(tmp_obj->object_class,"%s",buff);
                                        }else{
                                            alist = NULL;
                                            LBER_attrlist_add(&alist, attr, vlist, error);
                                            LINKEDLIST_APPEND(tmp_obj->alist,alist);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    LINKEDLIST_APPEND(obj, tmp_obj) //add new object to object list
                }
            }
        }
        else if(data[i] == 0x78)      //extended response
        {
            i++;
            FIND_LENGTH(data, optLen);
            len -= (optLen + i - j);
            tmp_obj = (LDAP_OBJECT*)SDF_MALLOC(sizeof(LDAP_OBJECT));    //create new object
            tmp_obj->value[0] = '\0';
            tmp_obj->name[0] = '\0';
            tmp_obj->error[0] = '\0';
            strcpy(tmp_obj->object_class, "extended");
            LINKEDLIST_APPEND(obj,tmp_obj);
            j = i;
            if ( data[i++] == 0x0a )   //result code
            {
                FIND_LENGTH(data, bufLen);
                memcpy(buff, &data[i], bufLen);
                buff[bufLen] = 0;
                i += bufLen;
                len -= (i - j);
            }
            j = i;
            if ( data[i++] == 0x04)    //match dn
            {
                FIND_LENGTH(data, bufLen);
                memcpy(buff, &data[i], bufLen);
                buff[bufLen] = 0;
                i += bufLen;
                len -= (i - j);
            }
            j = i;
            if ( data[i++] == 0x04 )    //errorMessage
            {
                FIND_LENGTH(data, bufLen);
                memcpy(buff, &data[i], bufLen);
                buff[bufLen] = 0;
                i += bufLen;
                strcpy(tmp_obj->error, buff);
                len -= (i - j);
            }
            j = i;
            if ( (unsigned char) data[i++] == 0x8a )     //response name
            {
                FIND_LENGTH(data, bufLen);
                memcpy(buff, &data[i], bufLen);
                buff[bufLen] = 0;
                i += bufLen;
                strcpy(tmp_obj->name, buff);    //copy reponse name to name of object
                len -= (i - j);
            }
            j = i;
            if ( (unsigned char) data[i++] == 0x8b )     //reponse data
            {
                FIND_LENGTH(data, bufLen);
                //memcpy(buff, &data[i], bufLen);
                //buff[bufLen] = 0;
                //i += bufLen;
                //strcpy(tmp_obj->value, buff);   //copy response value to value of object
                len -= (i - j);
                j = i;
                if (data[i++] == 0x30)                      //sequence
                {
                    FIND_LENGTH(data, attrLen);
                    len -= (i - j);
                    while (attrLen > 0)
                    {
                        j = i;
                        if (data[i++] == 0x30)              //sequence
                        {
                            FIND_LENGTH(data, partLen);
                            attrLen -= (partLen + i - j);
                            while (partLen > 0)
                            {
                                j = i;
                                if (data[i++] == 0x04)      //attr name
                                {
                                    FIND_LENGTH(data, bufLen);
                                    partLen -= (bufLen + i - j);
                                    memcpy(attr, &data[i], bufLen);
                                    attr[bufLen] = 0;
                                    i += bufLen;
                                    j = i;

                                    if (data[i++] == 0x31)  //set
                                    {
                                        FIND_LENGTH(data, valLen);
                                        partLen -= (valLen + i - j);

                                        vlist = NULL;
                                        while (valLen > 0)
                                        {
                                            j = i;
                                            if (data[i++] == 0x04)//attr val
                                            {
                                                FIND_LENGTH(data, bufLen);
                                                valLen -= (bufLen + i - j);
                                                memcpy(buff, &data[i], bufLen);
                                                buff[bufLen] = 0;
                                                i += bufLen;

                                                // create new vlist
                                                LBER_valuelist_add(&vlist, buff, error);
                                            }
                                        }
                                        alist = NULL;
                                        LBER_attrlist_add(&alist, attr, vlist, error);
                                        LINKEDLIST_APPEND(tmp_obj->alist,alist);
                                    }
                                }
                            }
                        }
                    }
                    LINKEDLIST_APPEND(obj, tmp_obj) //add new object to object list
                }
            }
        }else{
            ++i;
        }
    }

    *object = obj;   //copy object list to object.
    return LDAP_RET_OK;
}

int ber_to_attrs(BER *ber, BER_ATTRLIST **alists, char *error){

    int attrLen = 0;
    int partLen = 0;
    int bufLen  = 0;
    int valLen  = 0;
    int i = 0, j = 0,pp = 0;
    char *p     = NULL;
    char attr[LDAP_MAX_ATTR_NAME_SIZE];
    char buff[LDAP_MAX_ATTR_VALUE_SIZE];
    BER_ATTRLIST *alist = NULL;
    BER_VALLIST *vlist  = NULL;
    char *data  = ber->berval;

    if( ber == NULL ){
        return -1;
    }

    if( ber->len <= 0){
        return -1;
    }

    if (data[i++] == 0x30 )                      //sequence
    {
        FIND_LENGTH(data, attrLen);
        while (attrLen > 0)
        {
            j = i;
            if (data[i++] == 0x30)              //sequence
            {
                FIND_LENGTH(data, partLen);
                attrLen -= (partLen + i - j);
                while (partLen > 0)
                {
                    j = i;
                    if (data[i++] == 0x04)      //attr name
                    {
                        FIND_LENGTH(data, bufLen);
                        partLen -= (bufLen + i - j);
                        memcpy(attr, &data[i], bufLen);
                        attr[bufLen] = 0;
                        i += bufLen;
                        j = i;

                        if (data[i++] == 0x31)  //set
                        {
                            FIND_LENGTH(data, valLen);
                            partLen -= (valLen + i - j);

                            vlist = NULL;
                            while (valLen > 0)
                            {
                                j = i;
                                if (data[i++] == 0x04)//attr val
                                {
                                    FIND_LENGTH(data, bufLen);
                                    valLen -= (bufLen + i - j);
                                    memcpy(buff, &data[i], bufLen);
                                    buff[bufLen] = 0;
                                    i += bufLen;

                                    LBER_valuelist_add(&vlist, buff, error);
                                }// end if
                            }// end while value list
                        } //end if set
                        LBER_attrlist_add(&alist, attr, vlist, error);
                    }//end if attr
                }//end while attrs
            }//end if sequence
        }//end while
    }//end sequence
    (*alists) = alist;
    return 0;
}

int
ber_attr_to_string(LDAP_CLNT *lc, int format, char *error)
{

#define APPEND_STRING(_olist, _dst, _dlen)                          \
do{                                                                 \
    if (_olist->len + LDAP_MAX_STRING_SIZE > ol->size)              \
    {                                                               \
        int nlen = (int) _olist->len + LDAP_MAX_STRING_SIZE;        \
        ADJUST_SIZE(nlen, LDAP_MAX_STRING_SIZE)                     \
        _olist->string = (char *) realloc (_olist->string, nlen);   \
        if (_olist->string == NULL)                                 \
        {                                                           \
            if (error!=NULL)                                        \
                sprintf(error, "memory error");                     \
            while ((_olist=olist)!=NULL)                            \
            {                                                       \
                LINKEDLIST_REMOVE(olist, _olist);                   \
                free(_olist->string);                               \
                free(_olist);                                       \
            }                                                       \
            return LDAP_RET_ERR_MEMORY;                             \
        }                                                           \
        _olist->size = nlen;                                        \
    }                                                               \
    memcpy(_olist->string + _olist->len, _dst, _dlen);              \
    _olist->len = _olist->len + _dlen;                              \
    _olist->string[_olist->len] = '\0';                             \
}while(0)

#define PRINT_STRING(_STR_)                                         \
{                                                                   \
    tmp_strlen = sprintf _STR_ ;                                    \
    memcpy(&lc->bstring->berval[lc->bstring->len], string, tmp_strlen);     \
    lc->bstring->len = lc->bstring->len + tmp_strlen;                       \
}

    int i = 0, j, pp = 0, duplicate = 0;
    int optLen, attrLen, partLen, valLen, bufLen, tmpLen, tmp_strlen;
    char *p = NULL;
    char attr[LDAP_MAX_ATTR_VALUE_SIZE];
    char buff[LDAP_MAX_STRING_SIZE];
    char *string;
    olist_t *olist=NULL, *ol=NULL;

    if (lc->rber==NULL)
    {
        if (error!=NULL) strcpy(error, "buffer doesn't exist");
        return LDAP_RET_ERR_DECODE;
    }
    int len = lc->rber->len;
    char *data = lc->rber->berval;

    string = (char *) malloc(sizeof(*string)*LDAP_READ_BUFFER);
    string[0] = 0;
    ber_alloc_buffer(&(lc->bstring), BER_BUFFER);
    lc->bstring->berval[0] = 0;
    lc->bstring->len = 0;
    tmp_strlen = 0;
    if (format==LBER_DECODE_JSON)
    {
        PRINT_STRING((string, "{"));
    }
    while (len > 0)
    {
        ber_alloc_buffer(&(lc->bstring), BER_BUFFER);
        j = i;
        if (data[i++] == 0x64)//opt tag
        {
            FIND_LENGTH(data, optLen);
            len -= (optLen + i - j);
            if (data[i++] == 0x04)//object name
            {
                FIND_LENGTH(data, bufLen);
                memcpy(buff, &data[i], bufLen);
                buff[bufLen] = 0;
                i += bufLen;
                if (format==LBER_DECODE_JSON)
                {
                    char object_name[64]; object_name[0] = 0;
                    // Pre-scan objectClass
                    int scan_len = optLen - bufLen - 20; // approximate search margin
                    char scan_key[] = {0x0b, 0x6f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x31};
                    char *ptr = &(data[i]);
                    char *lptr = ptr;
                    while ((ptr=memchr(lptr, 0x0b, scan_len))!=NULL)
                    {
                        if (memcmp(ptr, scan_key, 13)==0)
                        {
                            ptr += 15;
                            scan_len = (int)*ptr;
                            (++ptr);
                            memcpy(object_name, ptr, scan_len);
                            object_name[scan_len] = 0;
                            break;
                        }
                        else
                        {
                            scan_len -= (int)(lptr-ptr);
                            lptr = ptr + 1;
                        }
                    }

                    duplicate = 0;
                    if (olist!=NULL)
                    {
                        ol = olist;
                        do
                        {
                            if (strcmp(object_name, ol->name)==0)
                            {
                                duplicate = 1;
                                break;
                            }
                            ol = ol->next;
                        } while (ol!=olist);
                    }
                    if (duplicate==0)
                    {
                        ol = (olist_t *) malloc(sizeof(*ol));
                        ol->prev = ol->next = NULL;
                        ol->name[0] = 0;
                        ol->size = 2*LDAP_MAX_STRING_SIZE;
                        ol->string = (char *) malloc(2*LDAP_MAX_STRING_SIZE);
                        ol->string[0] = 0;
                        ol->len = 0;
                        strcpy(ol->name, object_name);
                        LINKEDLIST_APPEND(olist, ol);
                    }
                    // increase buffer size
                    if (strlen(ol->string)+LDAP_MAX_STRING_SIZE > ol->size)
                    {
                        register int len = (int) strlen(ol->string) + LDAP_MAX_STRING_SIZE;
                        ADJUST_SIZE(len, LDAP_MAX_STRING_SIZE)
                        ol->string = (char *) realloc (ol->string, len);
                        if (ol->string == NULL)
                        {
                            if (error!=NULL)
                                sprintf(error, "memory error");
                            while ((ol=olist)!=NULL)
                            {
                                LINKEDLIST_REMOVE(olist, ol);
                                free(ol->string);
                                free(ol);
                            }
                            return LDAP_RET_ERR_MEMORY;
                        }
                        ol->size = len;
                    }
                    if (object_name[0]!=0)
                        tmpLen = sprintf(string, "{\"dn\":\"%s\",", buff);
                    else
                        tmpLen = sprintf(string, "{");
                    APPEND_STRING(ol, string, tmpLen);
                }
                else //LBER_DECODE_PRINTABLE_STRING
                {
                    PRINT_STRING((string, "dn: %s\n", buff));
                }
                
                if (data[i++] == 0x30)//sequence
                {
                    FIND_LENGTH(data, attrLen);
                    while (attrLen > 0)
                    {
                        j = i;
                        if (data[i++] == 0x30)//sequence
                        {
                            FIND_LENGTH(data, partLen);
                            attrLen -= (partLen + i - j);
                            while (partLen > 0)
                            {
                                j = i;
                                if (data[i++] == 0x04)//attr name
                                {
                                    FIND_LENGTH(data, bufLen);
                                    partLen -= (bufLen + i - j);
                                    memcpy(attr, &data[i], bufLen);
                                    attr[bufLen] = 0;
                                    i += bufLen;
                                    j = i;
                                    if (format==LBER_DECODE_JSON)
                                    {
                                        tmpLen = sprintf(string, "\"%s\":", attr);
                                        APPEND_STRING(ol, string, tmpLen);
                                    }
                                    if (data[i++] == 0x31)//set
                                    {
                                        int exist_flag=0;
                                        int array_flag=0;

                                        FIND_LENGTH(data, valLen);
                                        partLen -= (valLen + i - j);
                                        while (valLen > 0)
                                        {
                                            exist_flag = 1;
                                            j = i;
                                            if (data[i++] == 0x04)//attr val
                                            {
                                                FIND_LENGTH(data, bufLen);
                                                valLen -= (bufLen + i - j);
                                                memcpy(buff, &data[i], bufLen);
                                                buff[bufLen] = 0;
                                                i += bufLen;
                                                if (format==LBER_DECODE_JSON)
                                                {
                                                    if (valLen > 0)
                                                    {
                                                        if (array_flag == 0)
                                                        {
                                                            array_flag = 1;
                                                            tmpLen = sprintf(string, "[");
                                                            APPEND_STRING(ol, string, tmpLen);
                                                        }
                                                    }
                                                    tmpLen = sprintf(string, "\"%s\",", buff);
                                                    APPEND_STRING(ol, string, tmpLen);
                                                }
                                                else //LBER_DECODE_PRINTABLE_STRING
                                                {
                                                    PRINT_STRING((string, "%s: %s\n", attr, buff));
                                                }
                                            }
                                        }
                                        if (exist_flag==1)
                                        {
                                            if (format==LBER_DECODE_JSON)
                                            {
                                                if (array_flag == 1)
                                                {
                                                    ol->string[ol->len-1] = 0;
                                                    ol->len = ol->len -1;
                                                    tmpLen = sprintf(string, "],");
                                                    APPEND_STRING(ol, string, tmpLen);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if (format==LBER_DECODE_JSON)
                                            {
                                                tmpLen = sprintf(string, "\"\",");
                                                APPEND_STRING(ol, string, tmpLen);
                                            }
                                            else //LBER_DECODE_PRINTABLE_STRING
                                            {
                                                PRINT_STRING((string, "%s:\n", attr));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (format==LBER_DECODE_JSON)
                {
                    ol->string[ol->len-1] = 0;
                    ol->len = ol->len -1;
                    tmpLen = sprintf(string, "},");
                    APPEND_STRING(ol, string, tmpLen);
                }
                else //LBER_DECODE_PRINTABLE_STRING
                {
                    PRINT_STRING((string, "\n"));
                }
            }
        }
    }
    if (format==LBER_DECODE_JSON)
    {
        while ((ol=olist)!=NULL)
        {
            PRINT_STRING((string, "\"%s\":[%s", ol->name, ol->string));
            lc->bstring->berval[lc->bstring->len-1] = 0;
            --(lc->bstring->len);
            PRINT_STRING((string, "],"));
            LINKEDLIST_REMOVE(olist, ol);
            free(ol->string);
            free(ol);
        }
        lc->bstring->berval[lc->bstring->len-1] = 0;
        --(lc->bstring->len);
        PRINT_STRING((string, "}"));
    }
    lc->bstring->berval[lc->bstring->len] = 0;
    free(string);
    return LDAP_RET_OK;
}

#define FIND_LEN(_data, _len, len_size)				\
{                                                   \
    len_size = 0;									\
    pp = 0;											\
    p = &(_data[i]);								\
    if (p[0] > 0xffffff00)							\
    {												\
        len_size = p[0] - 0xffffff80;				\
        buf_len = len_size;							\
        i += buf_len;								\
        while (buf_len > 0)							\
        {											\
            p++;									\
            if ((int)p[0] > 0xffffff00)				\
                pp = pp + (int)p[0] - 0xffffff00;	\
            else									\
                pp = pp + (int)p[0];				\
                                                    \
            if (buf_len > 1)						\
                pp = pp << (1 * 8);					\
                                                    \
            buf_len--;								\
        }											\
        _len = pp;									\
    }												\
    else											\
    {												\
        _len = p[0];								\
    }												\
    i++;                                            \
}

int
ldap_decode_request(DSSESSION *sess, int decode_flag, char *error)
{
    char *ber = sess->r_buffer + sess->r_ber_body;
    int ber_len = sess->r_ber_body_length;
    
    int i = 0, j = 0, pp = 0, len = 0, const_len = 0, list_len = 0, len_size, buf_len, length = 0;
    char *p = NULL;
    LDAPMessage_t *decode_msg = NULL;
    ChangeType *change = NULL;
    BER_ATTRLIST *attr = NULL;
    BER_VALLIST *val = NULL;
    Control *control = NULL;

    if (ber[i++] == 0x30)
    {
        FIND_LENGTH(ber, len);
        if (len > ber_len)
        {
            if (error!=NULL) sprintf(error, "incorrect ldap length (%d > %d)", len, ber_len);
            return LDAP_RET_ERR_DECODE;
        }
        decode_msg = (LDAPMessage_t *) SDF_MALLOC(sizeof(LDAPMessage_t));
        if (decode_msg==NULL)
        {
            if (error!=NULL) sprintf(error, "cannot allocate memory");
            return LDAP_RET_ERR_MEMORY;
        }
        (void) memset(decode_msg, 0, sizeof(LDAPMessage_t));
        if (ber[i++] == 0x02)
        {
            len = ber[i++];
            j = i;
            i += len;
            while (len > 0)
            {
                --len;
                decode_msg->message_id += (int)(((unsigned char)ber[j]) << (8 * len));
                j++;
            }
            switch (ber[i++])
            {
                case BER_APPLICATION_PRIMITIVE | LDAP_PROTOCOL_UNBIND_REQUEST:
                    SDF_FREE(decode_msg);
                    return LDAP_RET_RECV_UNBIND;

                case BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_EXTENDED_REQUEST:
                    decode_msg->choice = LDAP_PROTOCOL_EXTENDED_REQUEST;
                    FIND_LENGTH(ber, len);
                    if ((unsigned char)ber[i++] == 0x80) // requestName
                    {
                        FIND_LENGTH(ber, len);
                        if (len > 0)
                        {
                            decode_msg->alt.extended_request.flags[REQUESTNAME] = 1;
                            memcpy(decode_msg->alt.extended_request.comps.request_name, &ber[i], len);
                            i += len;
                        }
                        decode_msg->alt.extended_request.comps.request_name[len] = 0;
                        if ((unsigned char)ber[i++] == 0x81) // requestValue
                        {
                            FIND_LENGTH(ber, len);
                            if (len > 0)
                            {
                                decode_msg->alt.extended_request.flags[REQUESTVALUE] = 1;
                                ber_alloc_buffer(&(decode_msg->alt.extended_request.comps.request_value), len);
                                memcpy(decode_msg->alt.extended_request.comps.request_value->berval, &ber[i], len);
                                decode_msg->alt.extended_request.comps.request_value->len += len;
                                i += len;
                            }
                        }
                    }
                    break;

                case BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_SEARCH_REQUEST:
                    decode_msg->choice = LDAP_PROTOCOL_SEARCH_REQUEST;
                    FIND_LENGTH(ber, len);
                    if (ber[i++] == 0x04)
                    {
                        FIND_LENGTH(ber, len);
                        if (len > 0)
                        {
                            decode_msg->alt.search_request.flags[BASEOBJECT] = 1;
                            memcpy(decode_msg->alt.search_request.comps.base_object, &ber[i], len);
                            decode_msg->alt.search_request.comps.base_object[len] = 0;
                            i += len;
                        }
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat baseDn");
                        return LDAP_RET_ERR_DECODE;
                    }

                    if (ber[i++] == 0x0a)
                    {
                        ++i;
                        decode_msg->alt.search_request.flags[SCOPE] = 1;
                        decode_msg->alt.search_request.comps.scope = ber[i++];
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat scope");
                        return LDAP_RET_ERR_DECODE;
                    }

                    if (ber[i++] == 0x0a)
                    {
                        ++i;
                        decode_msg->alt.search_request.flags[DEREFALIASES] = 1;
                        decode_msg->alt.search_request.comps.deref_aliases = ber[i++];
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat deferAliases");
                        return LDAP_RET_ERR_DECODE;
                    }

                    if (ber[i++] == 0x02)
                    {
                        ++i;
                        decode_msg->alt.search_request.flags[SIZELIMIT] = 1;
                        decode_msg->alt.search_request.comps.size_limit = ber[i++];
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat sizeLimit");
                        return LDAP_RET_ERR_DECODE;
                    }

                    if (ber[i++] == 0x02)
                    {
                        ++i;
                        decode_msg->alt.search_request.flags[TIMELIMIT] = 1;
                        decode_msg->alt.search_request.comps.time_limit = ber[i++];
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat timeLimit");
                        return LDAP_RET_ERR_DECODE;
                    }

                    if (ber[i++] == 0x01)
                    {
                        ++i;
                        decode_msg->alt.search_request.flags[TYPESONLY] = 1;
                        decode_msg->alt.search_request.comps.typesOnly = ber[i++];
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat typeOnly");
                        return LDAP_RET_ERR_DECODE;
                    }
                    
                    char ck_ber_filter  = (ber[i]&0x1F);
                    if (ck_ber_filter == FAND || ck_ber_filter == FOR || ck_ber_filter == FNOT || ck_ber_filter == FEQUALITYMATCH || 
                        ck_ber_filter == FSUBSTRINGS || ck_ber_filter == FGREATEROREQUAL || ck_ber_filter == FLESSOREQUAL || 
                        ck_ber_filter == FPRESENT || ck_ber_filter == FAPPROXMATCH || ck_ber_filter == FEXTENSIBLEMATCH)
                        {
                            i++;
    						FIND_LEN(ber, len, len_size)
                            i = (i - len_size - 2);
                            decode_msg->alt.search_request.flags[FILTER] = 1;
                            LDAP_string_to_filter((ber + i), &(decode_msg->alt.search_request.comps.filter));
                            i = (i + len + len_size + 2);
                        }

                    if (ber[i++] == 0x30) // assume attribute list
                    {
                        decode_msg->alt.search_request.flags[ATTRIBUTES] = 1;
                        FIND_LENGTH(ber, length);
                        if(length > 0)
                        {
                            while(length > 0)
                            {
                                if(ber[i] == 0x04)
                                {
                                        i++;
                                        FIND_LEN(ber, len, len_size)
                                        BER_VALLIST *attr = (BER_VALLIST *) SDF_MALLOC(sizeof(BER_VALLIST));
                                        attr->next = NULL;
                                        attr->prev = NULL;
                                        memcpy(attr->value, &ber[i], len);
                                        attr->value[len] = 0;
                                        attr->len = len;
                                        LINKEDLIST_APPEND(decode_msg->alt.search_request.comps.attributes, attr);
                                        i += len;
                                        length = (length - len - len_size - 2);
                                }
                            }
                        }
                        else
                        {
                            decode_msg->alt.search_request.comps.attributes = NULL;
                        }
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat attributes");
                        return LDAP_RET_ERR_DECODE;
                    }
                    break;
                case BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_MODIFY_REQUEST:
                    decode_msg->choice = LDAP_PROTOCOL_MODIFY_REQUEST;
                    FIND_LENGTH(ber, len);
                    if (ber[i++] == 0x04) // object
                    {
                        FIND_LENGTH(ber, len);
                        if (len > 0)
                        {
                            decode_msg->alt.modify_request.flags[OBJECT] = 1;
                            memcpy(decode_msg->alt.modify_request.comps.object, &ber[i], len);
                            decode_msg->alt.modify_request.comps.object[len] = 0;
                            i += len;
                        }
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapmodify: malformat object");
                        return LDAP_RET_ERR_DECODE;
                    }
                    if ((unsigned char)ber[i++] == 0x30) // sequence
                    {
                        FIND_LENGTH(ber, len);
                        const_len = len;
                        decode_msg->alt.modify_request.flags[CHANGETYPE] = 1;
                        decode_msg->alt.modify_request.comps.changes = NULL;
                        while (const_len > 0)
                        {
                            if ((unsigned char)ber[i++] == 0x30)
                            {
                                change = (ChangeType *) SDF_MALLOC(sizeof(ChangeType));
                                j = i;
                                FIND_LENGTH(ber, len);
                                const_len -= (1 + (i - j) + len);
                                if ((unsigned char)ber[i++] == 0x0a)
                                {
                                    FIND_LENGTH(ber, len);
                                    change->flags[OPERATION] = 1;
                                    change->comps.operation = ber[i++];
                                }
                                if ((unsigned char)ber[i++] == 0x30)
                                {
                                    FIND_LENGTH(ber, len);
                                    change->flags[MODIFICATION] = 1;
                                    attr = (BER_ATTRLIST *) SDF_MALLOC(sizeof(BER_ATTRLIST));
                                    attr->next = NULL;
                                    attr->prev = NULL;
                                   
                                    if ((unsigned char)ber[i++] == 0x04)
                                    {
                                        FIND_LENGTH(ber, len);
                                        if (len > 0)
                                        {
                                            memcpy(attr->name, &ber[i], len);
                                            attr->name[len] = 0;
                                            attr->len = len;
                                            i += len;
                                        }
                                    }
                                    if ((unsigned char)ber[i++] == 0x31)
                                    {
                                        FIND_LENGTH(ber, len);
                                        list_len = len;
                                        while (list_len > 0)
                                        {
                                            if ((unsigned char)ber[i++] == 0x04)
                                            {
                                                j = i;
                                                FIND_LENGTH(ber, len);
                                                list_len -= (1 + (i - j) + len);
                                                val = (BER_VALLIST *) SDF_MALLOC(sizeof(BER_VALLIST));
                                                val->next = NULL;
                                                val->prev = NULL;
                                                if (len > 0)
                                                {
                                                    memcpy(val->value, &ber[i], len);
                                                    val->value[len] = 0;
                                                    val->len = len;
                                                    i += len;
                                                }
                                                else
                                                {
                                                    val->value[0] = 0;
                                                    val->len = 0;
                                                }
                                                LINKEDLIST_APPEND(attr->vals, val);
                                            }
                                        }
                                    }
                                    LINKEDLIST_APPEND(change->comps.modification, attr);
                                }
                                LINKEDLIST_APPEND(decode_msg->alt.modify_request.comps.changes, change);
                            }
                        }
                    }
                    break;

                case BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_ADD_REQUEST:
                    decode_msg->choice = LDAP_PROTOCOL_ADD_REQUEST;
                    FIND_LENGTH(ber, len);
                    if (ber[i++] == 0x04) // entry
                    {
                        FIND_LENGTH(ber, len);
                        if (len > 0)
                        {
                            decode_msg->alt.add_request.flags[ADD_ENTRY] = 1;
                            memcpy(decode_msg->alt.add_request.comps.entry, &ber[i], len);
                            decode_msg->alt.add_request.comps.entry[len] = 0;
                            i += len;
                        }
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapadd: malformat object");
                        return LDAP_RET_ERR_DECODE;
                    }
                    if ((unsigned char)ber[i++] == 0x30) // sequence
                    {
                        FIND_LENGTH(ber, len);
                        const_len = len;
                        decode_msg->alt.add_request.flags[ADD_ATTRIBUTES] = 1;
                        decode_msg->alt.add_request.comps.attributes = NULL;
                        while (const_len > 0)
                        {
                            if ((unsigned char)ber[i++] == 0x30)
                            {
                                j = i;
                                FIND_LENGTH(ber, len);
                                const_len -= (1 + (i - j) + len);
                                attr = (BER_ATTRLIST *) SDF_MALLOC(sizeof(BER_ATTRLIST));
                                attr->next = NULL;
                                attr->prev = NULL;
                                if ((unsigned char)ber[i++] == 0x04)
                                {
                                    FIND_LENGTH(ber, len);
                                    if (len > 0)
                                    {
                                        memcpy(attr->name, &ber[i], len);
                                        attr->name[len] = 0;
                                        attr->len = len;
                                        i += len;
                                    }
                                }
                                if ((unsigned char)ber[i++] == 0x31)
                                {
                                    FIND_LENGTH(ber, len);
                                    list_len = len;
                                    while (list_len > 0)
                                    {
                                        if ((unsigned char)ber[i++] == 0x04)
                                        {
                                            j = i;
                                            FIND_LENGTH(ber, len);
                                            list_len -= (1 + (i - j) + len);
                                            val = (BER_VALLIST *) SDF_MALLOC(sizeof(BER_VALLIST));
                                            val->next = NULL;
                                            val->prev = NULL;
                                            if (len > 0)
                                            {
                                                memcpy(val->value, &ber[i], len);
                                                val->value[len] = 0;
                                                val->len = len;
                                                i += len;
                                            }
                                            else
                                            {
                                                val->value[0] = 0;
                                                val->len = 0;
                                            }
                                            LINKEDLIST_APPEND(attr->vals, val);
                                        }
                                    }
                                }
                                LINKEDLIST_APPEND(decode_msg->alt.add_request.comps.attributes, attr);
                            }
                        }
                    }
                    break;
                case BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_MODIFYDN_REQUEST:
                    decode_msg->choice = LDAP_PROTOCOL_MODIFYDN_REQUEST;
                    FIND_LENGTH(ber, len);
                    if (ber[i++] == 0x04) // entry
                    {
                        FIND_LENGTH(ber, len);
                        if (len > 0)
                        {
                            memcpy(decode_msg->alt.mod_dn_request.comps.entry, &ber[i], len);
                            decode_msg->alt.mod_dn_request.comps.entry[len] = 0;
                            i += len;
                        }
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapadd: malformat object");
                        return LDAP_RET_ERR_DECODE;
                    }
                    if (ber[i++] == 0x04) // newrdn
                    {
                        FIND_LENGTH(ber, len);
                        if (len > 0)
                        {
                            memcpy(decode_msg->alt.mod_dn_request.comps.new_rdn, &ber[i], len);
                            decode_msg->alt.mod_dn_request.comps.new_rdn[len] = 0;
                            i += len;
                        }
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapadd: malformat object");
                        return LDAP_RET_ERR_DECODE;
                    }
                    if (ber[i++] == 0x01) // delleteoldrdn
                    {
                        ++i;
                        decode_msg->alt.mod_dn_request.comps.delete_old_rdn = ber[i++];
                    }
                    else
                    {
                        if (error!=NULL) sprintf(error, "ldapsearch: malformat typeOnly");
                        return LDAP_RET_ERR_DECODE;
                    }
                    break;
                case BER_APPLICATION_PRIMITIVE | LDAP_PROTOCOL_DELETE_REQUEST:
                    decode_msg->choice = LDAP_PROTOCOL_DELETE_REQUEST;
                    FIND_LENGTH(ber, len);
                    if (len > 0)
                    {
                        memcpy(decode_msg->alt.delete_request.ldap_dn, &ber[i], len);
                        decode_msg->alt.delete_request.ldap_dn[len] = 0;
                        i += len;
                    }
                    break;
            }
        }
        // Controls
        if ((unsigned char)ber[i++] == 0xa0)
        {
            FIND_LENGTH(ber, len);
            const_len = len;
            while (const_len > 0)
            {
                if ((unsigned char)ber[i++] == 0x30)
                {
                    j = i;
                    FIND_LENGTH(ber, len);
                    const_len -= 1 + (j - i) + len;
                    control = (Control *) SDF_MALLOC(sizeof(Control));
                    control->flags[CRITICALITY] = 0;
                    control->comps.criticality = 0;
                    if ((unsigned char)ber[i++] == 0x04)
                    {
                        FIND_LENGTH(ber, len);
                        control->flags[CONTROLTYPE] = 1;
                        if (len > 0)
                        {
                            memcpy(control->comps.control_type, &ber[i], len);
                            control->comps.control_type[len] = 0;
                            i += len;
                        }
                        if ((unsigned char)ber[i++] == 0x04)
                        {
                            FIND_LENGTH(ber, len);
                            control->flags[CONTROLVALUE] = 1;
                            if (len > 0)
                            {
                                memcpy(control->comps.control_value.value, &ber[i], len);
                                control->comps.control_value.value[len] = 0;
                                control->comps.control_value.len = len;
                                i += len;
                            }
                            else
                            {
                                control->comps.control_value.value[0] = 0;
                                control->comps.control_value.len = 0;
                            }
                        }
                    }
                    LINKEDLIST_APPEND(decode_msg->controls, control);
                }
            }
        }

        LINKEDLIST_APPEND(sess->rdata, decode_msg);
        return LDAP_RET_DATA_AVAILABLE;
    }
    if (error!=NULL) sprintf(error, "unknown error");
    return LDAP_RET_ERR_DECODE;
}

int
ldap_decode_response(LDAP_CLNT *lc, int decode_flag, char *error)
{
    char *ber = lc->r_buffer + lc->r_ber_body;
    int ber_len = lc->r_ber_body_length;

    int i = 0, j = 0, pp = 0, len = 0;
    int optIndex;
    char *p = NULL, resp_present = 0;
    char oid[LDAP_MAX_OID_SIZE], err_msg[1024];


    if (ber[i++] == 0x30)
    {
        FIND_LENGTH(ber, len);
        if (ber[i++] == 0x02)
        {
            len = ber[i++];
            j = i;
            i += len;
            lc->tid = 0;
            while (len > 0)
            {
                --len;
                lc->tid += (int)(((unsigned char)ber[j]) << (8 * len));
                j++;
            }
            optIndex = i;
            switch (ber[i++])
            {
                case 0x78://extended
                    if ((lc->ktid!=0)&&(lc->ktid==lc->tid))
                    {
                        lc->ktid = 0;
                        return LDAP_RET_KEEPALIVE;
                    }
                    FIND_LENGTH(ber, len);
                    len = ber_len - optIndex;
                    ber_alloc_buffer(&(lc->rber), len);
                    memcpy(lc->rber->berval + lc->rber->len, &ber[optIndex], len);
                    lc->rber->len += len;
                    if (ber[i++] == 0x0a)
                    {
                        i++;
                        lc->result_code = ber[i++];
                        i += 2;
                        if (ber[i++] == 0x04)
                        {
                            FIND_LENGTH(ber, len);
                            if (len > 0)
                            {
                                memcpy(err_msg, &ber[i], len);
                                i += len;
                            }
                            err_msg[len] = 0;
                            if ((unsigned char)ber[i++] == 0x8a)
                            {
                                FIND_LENGTH(ber, len);
                                memcpy(oid, &ber[i], len);
                                oid[len] = 0;
                                i += len;
                                if ((unsigned char)ber[i++] == 0x8b)
                                {
                                    FIND_LENGTH(ber, len);
                                    if (ber[i] == 0x30)
                                    {
                                        ++i;
                                        FIND_LENGTH(ber, len);
                                        resp_present = 1;
                                    }
                                    else if ((unsigned char)ber[i] == 0xe1)
                                    {
                                        ++i;
                                        FIND_LENGTH(ber, len);
                                        if ((unsigned char)ber[i] == 0x80)
                                        {
                                            ++i;
                                            FIND_LENGTH(ber, len);
                                        }
                                        if ((unsigned char)ber[i] == 0x81)
                                        {
                                            ++i;
                                            FIND_LENGTH(ber, len);
                                            memcpy(oid, &ber[i], len);
                                            oid[len] = 0;
                                            i += len;
                                        }
                                        if ((unsigned char)ber[i] == 0x83)
                                        {
                                            ++i;
                                            FIND_LENGTH(ber, len);
                                            if (ber[i++] == 0x30)
                                            {
                                                FIND_LENGTH(ber, len);
                                                resp_present = 1;
                                            }
                                        }
                                    }
                                }

                                if (resp_present)
                                {
                                    // Do something.
                                }

                            }
                            else
                            {
                                err_msg[0] = 0;

                            }
                        }
                    }
                    break;

                case 0x64://search entry
                    if ((lc->ktid!=0)&&(lc->ktid==lc->tid))
                    {
                        return LDAP_RET_WAIT_MORE_DATA;
                    }
                    FIND_LENGTH(ber, len);//for trace only
                    len = ber_len - optIndex;
                    ber_alloc_buffer(&(lc->rber), len);
                    memcpy(lc->rber->berval + lc->rber->len, &ber[optIndex], len);
                    lc->rber->len += len;
                    return LDAP_RET_WAIT_MORE_DATA;

                case 0x65://search done
                    if ((lc->ktid!=0)&&(lc->ktid==lc->tid))
                    {
                        lc->ktid = 0;
                        return LDAP_RET_KEEPALIVE;
                    }
                    FIND_LENGTH(ber, len);
                    if (ber[i++] == 0x0a)
                    {
                        i++;//len
                        lc->result_code = ber[i++];
                        if (ber[i++] == 0x04)
                        {
                            FIND_LENGTH(ber, len);
                            if (len > 0)
                            {
                                memcpy(lc->matchedDN, &ber[i], len);
                                i += len;
                            }
                            lc->matchedDN[len] = 0;
                            if (ber[i++] == 0x04)
                            {
                                FIND_LENGTH(ber, len);
                                if (len > 0)
                                {
                                    memcpy(lc->diagnosticMessage, &ber[i], len);
                                    i += len;
                                }
                                if ((decode_flag==1)&&(lc->rber != NULL))
                                {
                                    ber_attr_to_string(lc, LBER_DECODE_PRINTABLE_STRING, error);
                                    free(lc->rber);
                                    lc->rber = NULL;
                                }
                            }
                        }
                    }
                    break;

                case 0x67: //modify result
                case 0x69: //add result;
                case 0x6b: //delete result
                case 0x6d: //modifydn result
                    FIND_LENGTH(ber, len);
                    if (ber[i++] == 0x0a)
                    {
                        i++;//len
                        lc->result_code = ber[i++];
                        if (ber[i++] == 0x04)
                        {
                            FIND_LENGTH(ber, len)
                            if (len > 0)
                            {
                                memcpy(lc->matchedDN, &ber[i], len);
                                i += len;
                            }
                            lc->matchedDN[len] = 0;
                            if (ber[i++] == 0x04)
                            {
                                FIND_LENGTH(ber, len)
                                if (len > 0)
                                {
                                    memcpy(lc->diagnosticMessage, &ber[i], len);
                                    i += len;
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }
    return LDAP_RET_DATA_AVAILABLE;
}

#define CHECK_MATCH_RULE(_data)					\
{                                               \
    pch = (char*)strstr(_data, ":=");           \
}

#define CHECK_MATCH_RULE_ID(_data_rule) 						\
{                                                               \
    pch = (char*)memchr(_data_rule, '.', strlen(_data_rule));   \
}

#define CHECK_MATCH_VALUE(_data_value)							\
{                                                               \
    pch = (char*)memchr(_data_value, '=', strlen(_data_value)); \
}

typedef struct _matching_rule_
{
    char rule_id[100];
    char match_type[10];
    char match_value[255];
    char dn_attr[3];
}MATCHDATA;

static int
matching_rule(char *data, char *b_filt, int *len_filt)
{
    char *buffer = (char*)calloc(255, sizeof(char));
    char *b_filt_temp = (char*)calloc(255, sizeof(char));
    int count_b_filt_temp = 0;
    int filt_len;
    int len;
    char *pch;
    filt_len = *len_filt;
    MATCHDATA match_rule;

    memset(&match_rule, 0, sizeof(match_rule)); // sizeof(MATCHDATA)

    buffer = data;
    buffer = strtok(buffer, ":");
    while (buffer != NULL)
    {
        CHECK_MATCH_RULE_ID(buffer)
        if (pch != NULL)
        {
            memcpy(match_rule.rule_id, buffer, strlen(buffer));
        }
        else if (strcmp(buffer, "dn") == 0)
        {
            memcpy(match_rule.dn_attr, buffer, strlen(buffer));
        }
        else
        {
            CHECK_MATCH_VALUE(buffer)
            if (pch != NULL)
            {
                memcpy(match_rule.match_value, pch, strlen(pch));
            }
            else
            {
                memcpy(match_rule.match_type, buffer, strlen(buffer));
            }
        }
        buffer = strtok(NULL, ":");
    }

    if (strlen(match_rule.rule_id) != 0)
    {
        b_filt_temp[count_b_filt_temp++] = 0x81;
        len = (int) strlen(match_rule.rule_id);
        b_filt_temp[count_b_filt_temp++] = len;
        memcpy(&b_filt_temp[count_b_filt_temp], match_rule.rule_id, len);
        count_b_filt_temp += len;
    }
    if (strlen(match_rule.match_type) !=0)
    {
        b_filt_temp[count_b_filt_temp++] = 0x82;
        len = (int) strlen(match_rule.match_type);
        b_filt_temp[count_b_filt_temp++] = len;
        memcpy(&b_filt_temp[count_b_filt_temp], match_rule.match_type, len);
        count_b_filt_temp += len;
    }
    if (strlen(match_rule.match_value) != 0)
    {
        b_filt_temp[count_b_filt_temp++] = 0x83;
        len = (int) strlen(match_rule.match_value);
        b_filt_temp[count_b_filt_temp++] = len;
        memcpy(&b_filt_temp[count_b_filt_temp], match_rule.match_value, len);
        count_b_filt_temp += len;
    }
    if (strlen(match_rule.dn_attr) != 0)
    {
        b_filt_temp[count_b_filt_temp++] = 0x84;
        len = (int) strlen(match_rule.dn_attr);
        b_filt_temp[count_b_filt_temp++] = len - 1;
        b_filt_temp[count_b_filt_temp++] = 0xFF;
        
    }
    b_filt[filt_len++] = 0xa9;
    len = (int) strlen(b_filt_temp);
    b_filt[filt_len++] = len;
    memcpy(&b_filt[filt_len], b_filt_temp, len);
    filt_len += len;

    *len_filt = filt_len;
    free(b_filt_temp);
    free(buffer);
    return 0;
}

static int
check_operation(char oper, int val_oper, char *b_filt, char *expression, int *len_filt)
{
    char *end_p, *filt_p;
    int len = (int)strlen(expression);
    int filt_len = *len_filt;
    int r, type_filter = 0;

    end_p = filt_p = expression;

    while ((*end_p++ != oper) && ((end_p - filt_p) < len));
    if (*end_p == '=')
    {

        b_filt[filt_len++] = val_oper;
        INSERT_LENGTH(b_filt, filt_len, (len + 2))
        b_filt[filt_len++] = 0x04;
        r = (int)(end_p - filt_p - 1);
        b_filt[filt_len++] = r;
        memcpy(&b_filt[filt_len], filt_p, r);
        filt_len += r;
        filt_p = end_p;
        while (*end_p++ != 0);
        b_filt[filt_len++] = 0x04;
        ++filt_p;
        r = (int)(end_p - filt_p - 1);
        b_filt[filt_len++] = r;

        memcpy(&b_filt[filt_len], filt_p, r);
        filt_len += r;
        type_filter = 1;
    }
    *len_filt = filt_len;
    return type_filter;
}

static int
any_substring(char *filt_p, char *b_filt, int *filt_len, int count_sub, int *edit_len)
{
    char *sub_p;
    int len_filt = *filt_len;
    int index_sub = 0, index_of_temp_sub = 0;
    int len = 0;
    char *temp_sub = (char*)calloc(255,sizeof(char));
    sub_p  = filt_p;
    int loop_num = 0;
    while ((sub_p = strtok(sub_p, "*")) != NULL)
    {
        if (index_sub == count_sub - 1)
        {
            //final
            ++loop_num;
            temp_sub[index_of_temp_sub++] = 0x82;
        }
        else
        {
            //any
            ++loop_num;
            temp_sub[index_of_temp_sub++] = 0x81;
        }
        len = (int)strlen(sub_p);
        temp_sub[index_of_temp_sub++] = len ;
        memcpy(&temp_sub[index_of_temp_sub], sub_p, len);
        index_of_temp_sub += len;
        ++index_sub;
        sub_p = NULL;
    }
    b_filt[len_filt++] = strlen(temp_sub);
    memcpy(&b_filt[len_filt], temp_sub, strlen(temp_sub));
    len_filt += strlen(temp_sub);
    if (loop_num > 0)
    {
        b_filt[*edit_len] = (len_filt - 2);
    }

    free(temp_sub);
    *filt_len = len_filt;
    return 0;
}

static void
create_ber_filter(char *expression, char *b_filt, int *len_filt)
{
    char *filt_p, *end_p, *sub_p;
    char *filt_p_s, *end_p_s, *sub_p_s;
    int r, len, filt_len = *len_filt;
    char def_obj[16] = "objectClass";
    int count_sub = 0;
    int len_sub;
    int edit_len;
    char *pch;
    len = (int)strlen(expression);
    end_p = filt_p = expression;

    CHECK_MATCH_RULE(expression);
    if (pch != NULL)
    {
        //case matching rule
        //input sn:dn:2.4.6.8.10:=test
        matching_rule(expression, b_filt, &filt_len);
    }
    else
    {
        if ((check_operation('<', 0xa6, b_filt, expression, &filt_len) != 1) &&
            (check_operation('>', 0xa5, b_filt, expression, &filt_len) != 1) &&
            (check_operation('~', 0xa8, b_filt, expression, &filt_len) != 1))
        {
            len = (int)strlen(expression);
            end_p = filt_p = expression;
            while ((*end_p++ != '=')&&((end_p - filt_p) < len));

            if ((*end_p == '*') && (end_p == len+filt_p - 1))
            {
                //case subscriptionState=*
                b_filt[filt_len++] = 0x87;
                b_filt[filt_len++] = end_p - filt_p - 1;
                memcpy(&b_filt[filt_len], filt_p, b_filt[filt_len - 1]);
                filt_len += b_filt[filt_len - 1];
            }
            else
            {
                if ((end_p-filt_p) == len) // no '=' sign, imply to objectClass=xxx
                {
                    b_filt[filt_len++] = 0xa3; //b_filt = 0xa3
                    r = (int)strlen(def_obj);
                    INSERT_LENGTH(b_filt, filt_len, (r + len + 4))
                    b_filt[filt_len++] = 0x04;
                    b_filt[filt_len++] = r;
                    memcpy(&b_filt[filt_len], def_obj, r);
                    filt_len += r;
                    b_filt[filt_len++] = 0x04;
                    r = (int)(end_p - filt_p);
                    b_filt[filt_len++] = r;
                    memcpy(&b_filt[filt_len], filt_p, r);
                    filt_len += r;
                }
                else
                {
                    //check substring
                    sub_p = end_p;
                    while ((sub_p-filt_p) < len)
                    {
                        if ((*sub_p) == '*')
                        {
                            ++count_sub;
                        }
                        ++sub_p;
                    }
                    filt_p_s = filt_p;
                    end_p_s = end_p;
                    sub_p_s = end_p_s;
                    end_p_s = len + filt_p_s - 1;
                    if (count_sub > 0)
                    {
                        b_filt[filt_len++] = 0xa4; //b_filt = 0xa4
                        if (count_sub == 1)
                        {
                            if ((*end_p_s == '*') || (*sub_p_s == '*'))
                            {
                                //initial or final
                                INSERT_LENGTH(b_filt, filt_len, (len + 4))
                            }
                            else
                            {
                                //intial to final
                                INSERT_LENGTH(b_filt, filt_len, (len + 6))
                            }
                        }
                        else
                        {
                            //count_sub> 1
                            //type any
                            INSERT_LENGTH(b_filt, filt_len, (len + (count_sub * 2) + 2 - 1))
                            edit_len = filt_len - 1; //only any to final
                        }
                    }
                    else
                    {
                        b_filt[filt_len++] = 0xa3; //b_filt = 0xa3
                        INSERT_LENGTH(b_filt, filt_len, (len + 3)) //b_filt = 0xa315
                    }
                    b_filt[filt_len++] = 0x04;
                    r = (int)(end_p - filt_p - 1);
                    b_filt[filt_len++] = r;
                    memcpy(&b_filt[filt_len], filt_p, r);
                    filt_len += r;
                    sub_p = filt_p = end_p;
                    len_sub = (int)strlen(filt_p);
                    if (count_sub > 0 )
                    {
                        if (count_sub == 1)
                        {
                            sub_p = filt_p;
                            if (*sub_p_s == '*')
                            {
                                // case *66876781014
                                b_filt[filt_len++] = 0x30;
                                b_filt[filt_len++] = len_sub + 2 - 1;
                                b_filt[filt_len++] = 0x82;
                                b_filt[filt_len++] = len_sub - 1;
                                filt_p = filt_p + 1;
                                memcpy(&b_filt[filt_len], filt_p, len_sub - 1);
                                filt_len += (len_sub - 1);
                            }
                            else if (*end_p_s == '*')
                            {
                                // case 66876781014*
                                b_filt[filt_len++] = 0x30;
                                b_filt[filt_len++] = len_sub + 2 - 1;
                                b_filt[filt_len++] = 0x80;
                                b_filt[filt_len++] = len_sub - 1;
                                memcpy(&b_filt[filt_len], filt_p, len_sub - 1);
                                filt_len += (len_sub - 1);
                            }
                            else
                            {
                                // case 6687*6781014
                                b_filt[filt_len++] = 0x30;
                                b_filt[filt_len++] = len_sub + 4 - 1;
                                b_filt[filt_len++] = 0x80;
                                while ((*sub_p++ != '*') &&(sub_p - filt_p < len_sub));
                                r = (int)(sub_p - filt_p - 1);
                                b_filt[filt_len++] = r;
                                
                                memcpy(&b_filt[filt_len], filt_p, r);
                                filt_len += r;
                                b_filt[filt_len++] = 0x82;
                                
                                filt_p = sub_p;
                                while (*end_p++ != 0);
                                b_filt[filt_len++] = (end_p - filt_p - 1);
                                memcpy(&b_filt[filt_len], filt_p, end_p - filt_p - 1);
                                filt_len += (end_p - filt_p - 1);
                            }
                        }else 
                        {
                            //count_sub> 1
                            //*6687*6781014 
                            //case edit_len support *66876781014*
                            b_filt[filt_len++] = 0x30;
                            any_substring(filt_p, b_filt, &filt_len, count_sub, &edit_len);
                        }
                    }
                    else 
                    {
                        //no type substring 
                        filt_p = end_p;
                        while (*end_p++ != 0);
                        b_filt[filt_len] = 0x04;// b_filt = 0xa3 15 0x04 06 6d 73 69 73 64 6e(msisdn) 04 
                        ++filt_len;
                        r = (int)(end_p - filt_p - 1);
                        b_filt[filt_len++] = r; //copy ob(len) append b_filt
                        memcpy(&b_filt[filt_len], filt_p, r); // copy  36 36 38 37 36 37 38 31 30 31 34 append b_filt
                        filt_len += r; 
                    }
                }
            }
        }
    }
    
    *len_filt = filt_len;
}

static void
process_filter(char *expression, char *b_filt, int *filt_len)
{
    char *filt_p, *end_p, buff[128], b_buff[1024];
    int r, object_len, buff_len = 0;
    int count;
    object_len = (int) strlen(expression);
    filt_p = expression;
    if ((*filt_p == '&')||(*filt_p == '|')||(*filt_p == '!')) // compound expression
    {
        if (*filt_p == '&') b_filt[(*filt_len)++] = 0xa0;
        else if (*filt_p == '|') b_filt[(*filt_len)++] = 0xa1;
        else if (*filt_p == '!') b_filt[(*filt_len)++] = 0xa2;
        ++filt_p;
        --object_len;
        while (object_len>0)
        {
            ++filt_p; --object_len; // assume "("
            end_p = filt_p;
            count = 0;
            while (end_p - filt_p -1 < object_len)
            {
                if (*end_p == '(') ++count;
                else if (*end_p == ')' && count > 0) --count;
                else if (*end_p == ')') { ++end_p; break; }
                ++end_p;
            }
            r = (int)(end_p - filt_p - 1);
            memcpy(buff, filt_p, r);
            buff[r] = 0;
            process_filter(buff, b_buff, &buff_len);
            object_len -= (end_p - filt_p);
            filt_p = end_p;
        }
        INSERT_LENGTH(b_filt, (*filt_len), buff_len)
        memcpy(&b_filt[*filt_len], b_buff, buff_len);
        *filt_len += buff_len;
    }
    else // single expression (support equality and presence)
    {
        memcpy(buff, filt_p, object_len);
        buff[object_len] = 0;
        create_ber_filter(buff, b_filt, filt_len);
    }
}

int
LDAP_gen_search_request(LDAP_t *dc, BER **ber, char *dn, char *scope, char *object_input, char *attrs, int alen, int deref_value, Control *control, char *error)
{
    int r, sum = 0, tid;
    int dn_len = 0;
    char b_dn[LDAP_MAX_DN_SIZE];
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];

    int search_len = 0, filt_len = 0, attr_len = 0, ctrl_len = 0, all_ctrl_len = 0;
    char b_search[6], s[3], deref[3], size[3], time[3], type[3], b_filt[1024], b_attr[6], b_control[6];
    char *filt_p;
    int object_len = (int)strlen(object_input);
    char *object;
    Control *ctrl = NULL, *bc = NULL;
    int bsize;

    object = (char *) calloc(object_len, sizeof(char));

    sprintf(object,"%s",object_input);

    sum = alen;

    b_attr[attr_len++] = 0x30;
    INSERT_LENGTH(b_attr, attr_len, sum);
    sum += attr_len;

    if (object[0] == 0 || strcmp(object, "*") == 0 || strcmp(object, "(*)") == 0)
    {
        b_filt[filt_len++] = 0x87;
        b_filt[filt_len++] = strlen("objectClass");
        memcpy(&b_filt[2], "objectClass", b_filt[1]);
        filt_len += b_filt[1];
    }
    else
    {
        // parsing search filter regular syntax https://www.centos.org/docs/5/html/CDS/ag/8.0/Finding_Directory_Entries-LDAP_Search_Filters.html
        filt_p = object;
        object_len = (int)strlen(object);
        if (*filt_p == '(')
        {
            ++filt_p;
            --object_len;
            if (object[object_len] == ')')
            {
                object[object_len] = 0;
                --object_len;
            }
        }
        process_filter(filt_p, b_filt, &filt_len);
    }
    sum += filt_len;

    type[0] = 0x01;
    type[1] = 0x01;
    type[2] = 0x00;
    sum += 3;

    time[0] = 0x02;
    time[1] = 0x01;
    time[2] = 0x00;
    sum += 3;

    size[0] = 0x02;
    size[1] = 0x01;
    size[2] = 0x00;
    sum += 3;

    deref[0] = 0x0a;
    deref[1] = 0x01;
    deref[2] = (deref_value & 0xff);
    sum += 3;

    s[0] = 0x0a;
    s[1] = 0x01;
    if (strcmp(scope, "base") == 0)
    {
        s[2] = 0x00;
    }
    else if (strcmp(scope, "one") == 0)
    {
        s[2] = 0x01;
    }
    else if (strcmp(scope, "sub") == 0)
    {
        s[2] = 0x02;
    }
    else
    {
        s[2] = 0x00;
    }
    sum += 3;

    CREATE_DN

    b_search[search_len++] = 0x63;
    INSERT_LENGTH(b_search, search_len, sum)
    sum += search_len;

    tid = tid_increment(dc);
    msg_len = ber_tid_create(tid, b_msg);
    sum += msg_len;

    if (control != NULL)
    {
        bc = ctrl = control;
        all_ctrl_len = 0;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                all_ctrl_len += bsize + 2;
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);

        if (all_ctrl_len > 0)
        {
            b_control[ctrl_len++] = 0xa0;
            INSERT_LENGTH(b_control, ctrl_len, all_ctrl_len);
            sum += (ctrl_len + all_ctrl_len);
        }
    }

    CREATE_LDAP

    COPY_TO_BER(b_search, search_len);
    COPY_DN
    memcpy((*ber)->berval + (*ber)->len, s, 3);
    (*ber)->len += 3;
    memcpy((*ber)->berval + (*ber)->len, deref, 3);
    (*ber)->len += 3;
    memcpy((*ber)->berval + (*ber)->len, size, 3);
    (*ber)->len += 3;
    memcpy((*ber)->berval + (*ber)->len, time, 3);
    (*ber)->len += 3;
    memcpy((*ber)->berval + (*ber)->len, type, 3);
    (*ber)->len += 3;
    memcpy((*ber)->berval + (*ber)->len, b_filt, filt_len);
    (*ber)->len += filt_len;
    memcpy((*ber)->berval + (*ber)->len, b_attr, attr_len);
    (*ber)->len += attr_len;

    if (attrs != NULL)
    {
        memcpy((*ber)->berval + (*ber)->len, attrs, alen);
        (*ber)->len += alen;
    }

    if ((control != NULL) && (all_ctrl_len > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, b_control, ctrl_len);
        (*ber)->len += ctrl_len;
        bc = ctrl = control;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                (*ber)->berval[(*ber)->len] = 0x30;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = bsize;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = 0x04;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = (int)strlen(ctrl->comps.control_type);
                ++((*ber)->len);
                memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_type, (int)strlen(ctrl->comps.control_type));
                (*ber)->len += (int)strlen(ctrl->comps.control_type);

                if (ctrl->comps.criticality != -1)
                {
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    if (ctrl->comps.criticality == 1)
                    {
                        (*ber)->berval[(*ber)->len] = 0xff;
                        ++((*ber)->len);
                    }
                    else
                    {
                        (*ber)->berval[(*ber)->len] = 0x00;
                        ++((*ber)->len);
                    }
                }

                if (ctrl->comps.control_value.value[0] != 0)
                {
                    (*ber)->berval[(*ber)->len] = 0x04;
                    ++((*ber)->len);

                    (*ber)->berval[(*ber)->len] = ctrl->comps.control_value.len;
                    ++((*ber)->len);
                    memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_value.value, ctrl->comps.control_value.len);
                    (*ber)->len += ctrl->comps.control_value.len;
                }
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);
    }

    free(object);
    return tid;
}

int
LDAP_gen_search_entry (DS_t *ds, BER **ber, int mid, char *object, char *attrs, int alen, char *error)
{
    int r, sum = 0;
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int olen = 0;
    int ext_len = 0, object_len = 0, attr_len = 0;
    char b_ext[6], b_object[6], b_attr[6];

    sum = alen;

    b_attr[attr_len++] = 0x30;
    INSERT_LENGTH(b_attr, attr_len, sum);
    sum += attr_len;
    
    b_object[object_len++] = 0x04;
    if ((object != NULL) && ((olen=(int)strlen(object)) > 0))
    {
        INSERT_LENGTH(b_object, object_len, olen);
    }
    else
    {
        olen = 0;
        INSERT_LENGTH(b_object, object_len, olen);
    }
    sum += object_len + olen;

    b_ext[ext_len++] = BER_APPLICATION_CONSTRUCTED |LDAP_PROTOCOL_SEARCH_ENTRY;
    INSERT_LENGTH(b_ext, ext_len, sum);
    sum += ext_len;

    msg_len = ber_tid_create(mid, b_msg);
    sum += msg_len;

    CREATE_LDAP

    COPY_TO_BER(b_ext, ext_len);
    memcpy((*ber)->berval + (*ber)->len, b_object, object_len);
    (*ber)->len += object_len;
    if ((object != NULL) && (olen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, object, olen);
        (*ber)->len += olen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_attr, attr_len);
    (*ber)->len += attr_len;
    if ((attrs != NULL) && (alen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, attrs, alen);
        (*ber)->len += alen;
    }
    
    return LDAP_RET_OK;
}

int
_gen_ldap_result (DS_t *ds, BER **ber, int mid, int protocol, int result, char *mdn, char *emsg, char *referal, char *error)
{
    int r, sum = 0;
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int erlen = 0, mlen = 0;
    int ext_len = 0, result_len = 0, matchDN_len = 0, errorMsg_len = 0;
    char b_ext[6], b_result[6], b_matchDN[128], b_errorMsg[128];

    b_errorMsg[errorMsg_len++] = 0x04;                      // error message
    if ((emsg != NULL) && ((erlen=(int)strlen(emsg)) > 0))
    {
        INSERT_LENGTH(b_errorMsg, errorMsg_len, erlen);
    }
    else
    {
        erlen = 0;
        INSERT_LENGTH(b_errorMsg, errorMsg_len, erlen);
    }
    sum += errorMsg_len + erlen;

    b_matchDN[matchDN_len++] = 0x04;                        // matchDN
    if ((mdn != NULL) && ((mlen=(int)strlen(mdn)) > 0))
    {
        INSERT_LENGTH(b_matchDN, matchDN_len, mlen);
    }
    else
    {
        mlen = 0;
        INSERT_LENGTH(b_matchDN, matchDN_len, mlen);
    }
    sum += matchDN_len + mlen;

    b_result[result_len++] = 0x0a;                          // resultCode
    INSERT_LENGTH(b_result, result_len, 1);
    b_result[result_len++] = (unsigned char) result;
    sum += result_len;

    b_ext[ext_len++] = protocol;
    INSERT_LENGTH(b_ext, ext_len, sum);
    sum += ext_len;

    msg_len = ber_tid_create(mid, b_msg);
    sum += msg_len;

    CREATE_LDAP

    COPY_TO_BER(b_ext, ext_len);
    memcpy((*ber)->berval + (*ber)->len, b_result, result_len);
    (*ber)->len += result_len;
    memcpy((*ber)->berval + (*ber)->len, b_matchDN, matchDN_len);
    (*ber)->len += matchDN_len;
    if ((mdn != NULL) && (mlen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, mdn, mlen);
        (*ber)->len += mlen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_errorMsg, errorMsg_len);
    (*ber)->len += errorMsg_len;
    if ((emsg != NULL) && (erlen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, emsg, erlen);
        (*ber)->len += erlen;
    }
    
    return LDAP_RET_OK;
}

int
LDAP_gen_search_done (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error)
{
    return _gen_ldap_result(ds, ber, mid,
                            BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_SEARCH_DONE,
                            result, mdn, emsg, NULL, error);
}

int
LDAP_gen_add_request(LDAP_t *dc, BER **ber, char *dn, char *attrs, int alen, Control *control, char *error)
{
    int r, sum = 0, tid;
    int dn_len = 0;
    char b_dn[LDAP_MAX_DN_SIZE];
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int attr_len = 0, add_len = 0, ctrl_len = 0, all_ctrl_len = 0;
    char b_attr[6], b_add[6];
    char b_control[6];
    Control *ctrl = NULL, *bc = NULL;
    int bsize;

    sum = alen;

    b_attr[attr_len++] = 0x30;
    INSERT_LENGTH(b_attr, attr_len, alen)
    sum += attr_len;

    CREATE_DN

    b_add[add_len++] = 0x68;
    INSERT_LENGTH(b_add, add_len, sum)
    sum += add_len;

    tid = tid_increment(dc);
    msg_len = ber_tid_create(tid, b_msg);
    sum += msg_len;

    if (control != NULL)
    {
        bc = ctrl = control;
        all_ctrl_len = 0;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                all_ctrl_len += bsize + 2;
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);

        if (all_ctrl_len > 0)
        {
            b_control[ctrl_len++] = 0xa0;
            INSERT_LENGTH(b_control, ctrl_len, all_ctrl_len);
            sum += (ctrl_len + all_ctrl_len);
        }
    }

    CREATE_LDAP

    COPY_TO_BER(b_add, add_len);
    COPY_DN
    memcpy((*ber)->berval + (*ber)->len, b_attr, attr_len);
    (*ber)->len += attr_len;
    memcpy((*ber)->berval + (*ber)->len, attrs, alen);
    (*ber)->len += alen;

    if ((control != NULL) && (all_ctrl_len > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, b_control, ctrl_len);
        (*ber)->len += ctrl_len;
        bc = ctrl = control;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                (*ber)->berval[(*ber)->len] = 0x30;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = bsize;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = 0x04;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = (int)strlen(ctrl->comps.control_type);
                ++((*ber)->len);
                memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_type, (int)strlen(ctrl->comps.control_type));
                (*ber)->len += (int)strlen(ctrl->comps.control_type);

                if (ctrl->comps.criticality != -1)
                {
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    if (ctrl->comps.criticality == 1)
                    {
                        (*ber)->berval[(*ber)->len] = 0xff;
                        ++((*ber)->len);
                    }
                    else
                    {
                        (*ber)->berval[(*ber)->len] = 0x00;
                        ++((*ber)->len);
                    }
                }

                if (ctrl->comps.control_value.value[0] != 0)
                {
                    (*ber)->berval[(*ber)->len] = 0x04;
                    ++((*ber)->len);

                    (*ber)->berval[(*ber)->len] = ctrl->comps.control_value.len;
                    ++((*ber)->len);
                    memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_value.value, ctrl->comps.control_value.len);
                    (*ber)->len += ctrl->comps.control_value.len;
                }
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);
    }

    return tid;
}

int
LDAP_gen_add_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error)
{
    return _gen_ldap_result(ds, ber, mid,
                            BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_ADD_RESPONSE,
                            result, mdn, emsg, NULL, error);
}

int
LDAP_gen_modify_request(LDAP_t *dc, BER **ber, char *dn, char *attrs, int alen, Control *control, char *error)
{
    int tid;
    int r, sum = 0;
    int dn_len = 0;
    char b_dn[LDAP_MAX_DN_SIZE];
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int attr_len = 0, mod_len = 0, ctrl_len = 0, all_ctrl_len = 0;
    char b_attr[6], b_mod[6];
    char b_control[6];
    Control *ctrl = NULL, *bc = NULL;
    int bsize;

    sum = alen;

    b_attr[attr_len++] = 0x30;
    INSERT_LENGTH(b_attr, attr_len, sum);
    sum += attr_len;

    CREATE_DN

    b_mod[mod_len++] = 0x66;
    INSERT_LENGTH(b_mod, mod_len, sum);
    sum += mod_len;

    tid = tid_increment(dc);
    msg_len = ber_tid_create(tid, b_msg);
    sum += msg_len;

    if (control != NULL)
    {
        bc = ctrl = control;
        all_ctrl_len = 0;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                all_ctrl_len += bsize + 2;
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);

        if (all_ctrl_len > 0)
        {
            b_control[ctrl_len++] = 0xa0;
            INSERT_LENGTH(b_control, ctrl_len, all_ctrl_len);
            sum += (ctrl_len + all_ctrl_len);
        }
    }

    CREATE_LDAP

    COPY_TO_BER(b_mod, mod_len);
    COPY_DN
    memcpy((*ber)->berval + (*ber)->len, b_attr, attr_len);
    (*ber)->len += attr_len;
    memcpy((*ber)->berval + (*ber)->len, attrs, alen);
    (*ber)->len += alen;

    if ((control != NULL) && (all_ctrl_len > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, b_control, ctrl_len);
        (*ber)->len += ctrl_len;
        bc = ctrl = control;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                (*ber)->berval[(*ber)->len] = 0x30;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = bsize;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = 0x04;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = (int)strlen(ctrl->comps.control_type);
                ++((*ber)->len);
                memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_type, (int)strlen(ctrl->comps.control_type));
                (*ber)->len += (int)strlen(ctrl->comps.control_type);

                if (ctrl->comps.criticality != -1)
                {
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    if (ctrl->comps.criticality == 1)
                    {
                        (*ber)->berval[(*ber)->len] = 0xff;
                        ++((*ber)->len);
                    }
                    else
                    {
                        (*ber)->berval[(*ber)->len] = 0x00;
                        ++((*ber)->len);
                    }
                }

                if (ctrl->comps.control_value.value[0] != 0)
                {
                    (*ber)->berval[(*ber)->len] = 0x04;
                    ++((*ber)->len);

                    (*ber)->berval[(*ber)->len] = ctrl->comps.control_value.len;
                    ++((*ber)->len);
                    memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_value.value, ctrl->comps.control_value.len);
                    (*ber)->len += ctrl->comps.control_value.len;
                }
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);
    }

    return tid;
}

int
LDAP_gen_modify_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error)
{
    return _gen_ldap_result(ds, ber, mid,
                            BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_MODIFY_RESPONSE,
                            result, mdn, emsg, NULL, error);
}

int
LDAP_gen_moddn_request(LDAP_t *dc, BER **ber, char *dn, char *rdn, int delete_flag, char *sup, Control *control, char *error)
{
    int tid;
    int r, sum = 0;
    int dn_len = 0, rdn_len = 0,sup_len = 0;
    char b_dn[LDAP_MAX_DN_SIZE];
    char b_rdn[LDAP_MAX_DN_SIZE];
    char b_sup[LDAP_MAX_DN_SIZE];
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int mod_len = 0, ctrl_len = 0, all_ctrl_len = 0;
    char b_mod[6];
    char b_del[4];
    char b_control[6];
    Control *ctrl = NULL, *bc = NULL;
    int bsize;

    b_dn[dn_len++] = 0x04;
    r = (int)strlen(dn);
    INSERT_LENGTH(b_dn, dn_len, r)
    memcpy(&b_dn[dn_len], dn, r);
    dn_len += r;
    sum += dn_len;

    b_rdn[rdn_len++] = 0x04;
    r = (int)strlen(rdn);
    INSERT_LENGTH(b_rdn, rdn_len, r)
    memcpy(&b_rdn[rdn_len], rdn, r);
    rdn_len += r;
    sum += rdn_len;

    b_del[0] = 1;
    b_del[1] = 1;
    b_del[2] = (delete_flag==1)?1:0;
    sum += 3;

    if (sup != NULL)
    {
        b_sup[sup_len++] = 0x04;
        r = (int)strlen(sup);
        INSERT_LENGTH(b_sup, sup_len, r)
        memcpy(&b_sup[sup_len], sup, r);
        sup_len += r;
        sum += sup_len;
    }

    b_mod[mod_len++] = 0x6C;
    INSERT_LENGTH(b_mod, mod_len, sum);
    sum += mod_len;

    tid = tid_increment(dc);
    msg_len = ber_tid_create(tid, b_msg);
    sum += msg_len;

    if (control != NULL)
    {
        bc = ctrl = control;
        all_ctrl_len = 0;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                all_ctrl_len += bsize + 2;
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);

        if (all_ctrl_len > 0)
        {
            b_control[ctrl_len++] = 0xa0;
            INSERT_LENGTH(b_control, ctrl_len, all_ctrl_len);
            sum += (ctrl_len + all_ctrl_len);
        }
    }

    CREATE_LDAP
    COPY_TO_BER(b_mod, mod_len);
    memcpy((*ber)->berval + (*ber)->len, b_dn, dn_len);
    (*ber)->len += dn_len;
    memcpy((*ber)->berval + (*ber)->len, b_rdn, rdn_len);
    (*ber)->len += rdn_len;
    memcpy((*ber)->berval + (*ber)->len, b_del, 3);
    (*ber)->len += 3;
    if (sup != NULL)
    {
        memcpy((*ber)->berval + (*ber)->len, b_sup, sup_len);
        (*ber)->len += sup_len;
    }

    if ((control != NULL) && (all_ctrl_len > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, b_control, ctrl_len);
        (*ber)->len += ctrl_len;
        bc = ctrl = control;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                (*ber)->berval[(*ber)->len] = 0x30;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = bsize;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = 0x04;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = (int)strlen(ctrl->comps.control_type);
                ++((*ber)->len);
                memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_type, (int)strlen(ctrl->comps.control_type));
                (*ber)->len += (int)strlen(ctrl->comps.control_type);

                if (ctrl->comps.criticality != -1)
                {
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    if (ctrl->comps.criticality == 1)
                    {
                        (*ber)->berval[(*ber)->len] = 0xff;
                        ++((*ber)->len);
                    }
                    else
                    {
                        (*ber)->berval[(*ber)->len] = 0x00;
                        ++((*ber)->len);
                    }
                }

                if (ctrl->comps.control_value.value[0] != 0)
                {
                    (*ber)->berval[(*ber)->len] = 0x04;
                    ++((*ber)->len);

                    (*ber)->berval[(*ber)->len] = ctrl->comps.control_value.len;
                    ++((*ber)->len);
                    memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_value.value, ctrl->comps.control_value.len);
                    (*ber)->len += ctrl->comps.control_value.len;
                }
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);
    }

    return tid;
}

int
LDAP_gen_moddn_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error)
{
    return _gen_ldap_result(ds, ber, mid,
                            BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_MODIFYDN_RESPONSE,
                            result, mdn, emsg, NULL, error);
}

int LDAP_gen_delete_request (LDAP_t *dc, BER **ber, char *dn, Control *control, char *error)
{
    int tid;
    int r;
    int sum = 0;
    int msg_len = 0;
    char b_msg[6];
    int del_len = 0, ldap_len = 0, ctrl_len = 0, all_ctrl_len = 0;
    char b_del[LDAP_MAX_DN_SIZE], b_ldap[6];
    char b_control[6];
    Control *ctrl = NULL, *bc = NULL;
    int bsize;

    r = (int)strlen(dn);
    b_del[del_len++] = 0x4a;
    INSERT_LENGTH(b_del, del_len, r);
    memcpy(&b_del[del_len], dn, r);
    del_len += r;
    sum += del_len;

    tid = tid_increment(dc);
    msg_len = ber_tid_create(tid, b_msg);
    sum += msg_len;

    if (control != NULL)
    {
        bc = ctrl = control;
        all_ctrl_len = 0;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                all_ctrl_len += bsize + 2;
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);

        if (all_ctrl_len > 0)
        {
            b_control[ctrl_len++] = 0xa0;
            INSERT_LENGTH(b_control, ctrl_len, all_ctrl_len);
            sum += (ctrl_len + all_ctrl_len);
        }
    }

    CREATE_LDAP

    COPY_TO_BER(b_del, del_len);

    if ((control != NULL) && (all_ctrl_len > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, b_control, ctrl_len);
        (*ber)->len += ctrl_len;
        bc = ctrl = control;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                (*ber)->berval[(*ber)->len] = 0x30;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = bsize;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = 0x04;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = (int)strlen(ctrl->comps.control_type);
                ++((*ber)->len);
                memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_type, (int)strlen(ctrl->comps.control_type));
                (*ber)->len += (int)strlen(ctrl->comps.control_type);

                if (ctrl->comps.criticality != -1)
                {
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    if (ctrl->comps.criticality == 1)
                    {
                        (*ber)->berval[(*ber)->len] = 0xff;
                        ++((*ber)->len);
                    }
                    else
                    {
                        (*ber)->berval[(*ber)->len] = 0x00;
                        ++((*ber)->len);
                    }
                }

                if (ctrl->comps.control_value.value[0] != 0)
                {
                    (*ber)->berval[(*ber)->len] = 0x04;
                    ++((*ber)->len);

                    (*ber)->berval[(*ber)->len] = ctrl->comps.control_value.len;
                    ++((*ber)->len);
                    memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_value.value, ctrl->comps.control_value.len);
                    (*ber)->len += ctrl->comps.control_value.len;
                }
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);
    }

    return tid;
}

int
LDAP_gen_delete_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *error)
{
    return _gen_ldap_result(ds, ber, mid,
                            BER_APPLICATION_CONSTRUCTED | LDAP_PROTOCOL_DELETE_RESPONSE,
                            result, mdn, emsg, NULL, error);
}

int
LDAP_gen_extended_common_request (LDAP_t *dc, BER **ber, char *oid, char *ext_attr, int elen, Control *control, char *error)
{
    int tid;
    int r, sum = 0;
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int olen;
    int ext_len = 0, oid_len = 0, attrall_len = 0, ctrl_len = 0, all_ctrl_len = 0;
    char b_ext[6], b_oid[6], b_attrall[6];
    char b_control[6];
    Control *ctrl = NULL, *bc = NULL;
    int bsize;

    sum = elen;

    b_attrall[attrall_len++] = 0x81;                //request value
    INSERT_LENGTH(b_attrall, attrall_len, sum);     //insert request value
    sum += attrall_len;

    olen = (int)strlen(oid);
    b_oid[oid_len++] = 0x80;                        // oid name
    INSERT_LENGTH(b_oid, oid_len, olen);            // insert len oid
    sum += oid_len + olen;

    b_ext[ext_len++] = 0x77;                        // protocal opt := oid_len + request_value
    INSERT_LENGTH(b_ext, ext_len, sum);             // insert len protocal opt
    sum += ext_len;

    tid = tid_increment(dc);
    msg_len = ber_tid_create(tid, b_msg);
    sum += msg_len;

    if (control != NULL)
    {
        bc = ctrl = control;
        all_ctrl_len = 0;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                all_ctrl_len += bsize + 2;
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);

        if (all_ctrl_len > 0)
        {
            b_control[ctrl_len++] = 0xa0;
            INSERT_LENGTH(b_control, ctrl_len, all_ctrl_len);
            sum += (ctrl_len + all_ctrl_len);
        }
    }

    CREATE_LDAP

    COPY_TO_BER(b_ext, ext_len);
    memcpy((*ber)->berval + (*ber)->len, b_oid, oid_len);
    (*ber)->len += oid_len;
    memcpy((*ber)->berval + (*ber)->len, oid, olen);
    (*ber)->len += (int)strlen(oid);
    memcpy((*ber)->berval + (*ber)->len, b_attrall, attrall_len);
    (*ber)->len += attrall_len;

    if ((ext_attr != NULL) || (elen == 0))
    {
        memcpy((*ber)->berval + (*ber)->len, ext_attr, elen);
        (*ber)->len += elen;
    }

    if ((control != NULL) && (all_ctrl_len > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, b_control, ctrl_len);
        (*ber)->len += ctrl_len;
        bc = ctrl = control;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                (*ber)->berval[(*ber)->len] = 0x30;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = bsize;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = 0x04;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = (int)strlen(ctrl->comps.control_type);
                ++((*ber)->len);
                memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_type, (int)strlen(ctrl->comps.control_type));
                (*ber)->len += (int)strlen(ctrl->comps.control_type);

                if (ctrl->comps.criticality != -1)
                {
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    if (ctrl->comps.criticality == 1)
                    {
                        (*ber)->berval[(*ber)->len] = 0xff;
                        ++((*ber)->len);
                    }
                    else
                    {
                        (*ber)->berval[(*ber)->len] = 0x00;
                        ++((*ber)->len);
                    }
                }

                if (ctrl->comps.control_value.value[0] != 0)
                {
                    (*ber)->berval[(*ber)->len] = 0x04;
                    ++((*ber)->len);

                    (*ber)->berval[(*ber)->len] = ctrl->comps.control_value.len;
                    ++((*ber)->len);
                    memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_value.value, ctrl->comps.control_value.len);
                    (*ber)->len += ctrl->comps.control_value.len;
                }
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);
    }

    return tid;
}

int
LDAP_gen_extended_common_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *oid, char *ext_attr, int elen, char *error)
{
    int r, sum = 0;
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int olen = 0, erlen = 0, mlen = 0;
    int ext_len = 0, result_len = 0, matchDN_len = 0, errorMsg_len = 0, oid_len = 0, attrall_len = 0;
    char b_ext[6], b_result[6], b_matchDN[128], b_errorMsg[128], b_oid[6], b_attrall[6];

    sum = elen;

    b_attrall[attrall_len++] = 0x8b;                        //response value
    INSERT_LENGTH(b_attrall, attrall_len, sum);             //insert response value
    sum += attrall_len;

    b_oid[oid_len++] = 0x8a;                                // oid name
    if ((oid != NULL) && ((olen=(int)strlen(oid)) > 0))
    {
        INSERT_LENGTH(b_oid, oid_len, olen);
    }
    else
    {
        olen = 0;
        INSERT_LENGTH(b_oid, oid_len, olen);
    }
    sum += oid_len + olen;

    b_errorMsg[errorMsg_len++] = 0x04;                      // error message
    if ((emsg != NULL) && ((erlen=(int)strlen(emsg)) > 0))
    {
        INSERT_LENGTH(b_errorMsg, errorMsg_len, erlen);
    }
    else
    {
        erlen = 0;
        INSERT_LENGTH(b_errorMsg, errorMsg_len, erlen);
    }
    sum += errorMsg_len + erlen;

    b_matchDN[matchDN_len++] = 0x04;                        // matchDN
    if ((mdn != NULL) && ((mlen=(int)strlen(mdn)) > 0))
    {
        INSERT_LENGTH(b_matchDN, matchDN_len, mlen);
    }
    else
    {
        mlen = 0;
        INSERT_LENGTH(b_matchDN, matchDN_len, mlen);
    }
    sum += matchDN_len + mlen;

    b_result[result_len++] = 0x0a;                          // resultCode
    INSERT_LENGTH(b_result, result_len, 1);
    b_result[result_len++] = (unsigned char) result;
    sum += result_len;

    b_ext[ext_len++] = 0x78;                                // protocal opt := oid_len + response_value
    INSERT_LENGTH(b_ext, ext_len, sum);                     // insert len protocol opt
    sum += ext_len;

    msg_len = ber_tid_create(mid, b_msg);
    sum += msg_len;

    CREATE_LDAP

    COPY_TO_BER(b_ext, ext_len);
    memcpy((*ber)->berval + (*ber)->len, b_result, result_len);
    (*ber)->len += result_len;
    memcpy((*ber)->berval + (*ber)->len, b_matchDN, matchDN_len);
    (*ber)->len += matchDN_len;
    if ((mdn != NULL) && (mlen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, mdn, mlen);
        (*ber)->len += mlen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_errorMsg, errorMsg_len);
    (*ber)->len += errorMsg_len;
    if ((emsg != NULL) && (erlen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, emsg, erlen);
        (*ber)->len += erlen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_oid, oid_len);
    (*ber)->len += oid_len;
    if ((oid != NULL) && (olen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, oid, olen);
        (*ber)->len += olen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_attrall, attrall_len);
    (*ber)->len += attrall_len;
    if ((ext_attr != NULL) && (elen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, ext_attr, elen);
        (*ber)->len += elen;
    }
    
    return LDAP_RET_OK;
}

int
LDAP_gen_extended_request (LDAP_t *dc, BER **ber, char *oid, char *ext_attr, int elen, Control *control, char *error)
{
    int tid;
    int r, sum = 0;
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int olen;
    int ext_len = 0, oid_len = 0, attrall_len = 0, attr_len = 0, ctrl_len = 0, all_ctrl_len = 0;
    char b_ext[6], b_oid[6], b_attrall[6], b_attr[6];
    char b_control[6];
    Control *ctrl = NULL, *bc = NULL;
    int bsize;

    sum = elen;

    if ((ext_attr != NULL) || (elen == 0))
    {
        b_attr[attr_len++] = 0x30;                  //ldap message
        INSERT_LENGTH(b_attr, attr_len, sum);       //size of ldap message
        sum += attr_len;
    }

    b_attrall[attrall_len++] = 0x81;                //request value
    INSERT_LENGTH(b_attrall, attrall_len, sum);     //insert request value
    sum += attrall_len;

    olen = (int)strlen(oid);
    b_oid[oid_len++] = 0x80;                        // oid name
    INSERT_LENGTH(b_oid, oid_len, olen);            // insert len oid
    sum += oid_len + olen;

    b_ext[ext_len++] = 0x77;                        // protocal opt := oid_len + request_value
    INSERT_LENGTH(b_ext, ext_len, sum);             // insert len protocal opt
    sum += ext_len;

    tid = tid_increment(dc);
    msg_len = ber_tid_create(tid, b_msg);
    sum += msg_len;

    if (control != NULL)
    {
        bc = ctrl = control;
        all_ctrl_len = 0;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                all_ctrl_len += bsize + 2;
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);

        if (all_ctrl_len > 0)
        {
            b_control[ctrl_len++] = 0xa0;
            INSERT_LENGTH(b_control, ctrl_len, all_ctrl_len);
            sum += (ctrl_len + all_ctrl_len);
        }
    }

    CREATE_LDAP

    COPY_TO_BER(b_ext, ext_len);
    memcpy((*ber)->berval + (*ber)->len, b_oid, oid_len);
    (*ber)->len += oid_len;
    memcpy((*ber)->berval + (*ber)->len, oid, olen);
    (*ber)->len += (int)strlen(oid);
    memcpy((*ber)->berval + (*ber)->len, b_attrall, attrall_len);
    (*ber)->len += attrall_len;
    memcpy((*ber)->berval + (*ber)->len, b_attr, attr_len);
    (*ber)->len += attr_len;

    if ((ext_attr != NULL) || (elen == 0))
    {
        memcpy((*ber)->berval + (*ber)->len, ext_attr, elen);
        (*ber)->len += elen;
    }

    if ((control != NULL) && (all_ctrl_len > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, b_control, ctrl_len);
        (*ber)->len += ctrl_len;
        bc = ctrl = control;
        do
        {
            if (ctrl->comps.control_type[0] != 0)
            {
                if (ctrl->comps.criticality != -1 && ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3 + (2 + ctrl->comps.control_value.len);
                else if (ctrl->comps.criticality != -1)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + 3;
                else if (ctrl->comps.control_value.value[0] != 0)
                    bsize = (2 + (int)strlen(ctrl->comps.control_type)) + (2 + ctrl->comps.control_value.len);
                else
                    bsize = (2 + (int)strlen(ctrl->comps.control_type));

                (*ber)->berval[(*ber)->len] = 0x30;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = bsize;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = 0x04;
                ++((*ber)->len);
                (*ber)->berval[(*ber)->len] = (int)strlen(ctrl->comps.control_type);
                ++((*ber)->len);
                memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_type, (int)strlen(ctrl->comps.control_type));
                (*ber)->len += (int)strlen(ctrl->comps.control_type);

                if (ctrl->comps.criticality != -1)
                {
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    (*ber)->berval[(*ber)->len] = 0x01;
                    ++((*ber)->len);
                    if (ctrl->comps.criticality == 1)
                    {
                        (*ber)->berval[(*ber)->len] = 0xff;
                        ++((*ber)->len);
                    }
                    else
                    {
                        (*ber)->berval[(*ber)->len] = 0x00;
                        ++((*ber)->len);
                    }
                }

                if (ctrl->comps.control_value.value[0] != 0)
                {
                    (*ber)->berval[(*ber)->len] = 0x04;
                    ++((*ber)->len);

                    (*ber)->berval[(*ber)->len] = ctrl->comps.control_value.len;
                    ++((*ber)->len);
                    memcpy((*ber)->berval + (*ber)->len, ctrl->comps.control_value.value, ctrl->comps.control_value.len);
                    (*ber)->len += ctrl->comps.control_value.len;
                }
            }
            ctrl = ctrl->next;
        } while (ctrl != bc);
    }

    return tid;
}

int
LDAP_gen_extended_response (DS_t *ds, BER **ber, int mid, int result, char *mdn, char *emsg, char *oid, char *ext_attr, int elen, char *error)
{
    int r, sum = 0;
    int msg_len = 0;
    char b_msg[6];
    int ldap_len = 0;
    char b_ldap[6];
    int olen = 0, erlen = 0, mlen = 0;
    int ext_len = 0, result_len = 0, matchDN_len = 0, errorMsg_len = 0, oid_len = 0, attrall_len = 0, attr_len = 0;
    char b_ext[6], b_result[6], b_matchDN[128], b_errorMsg[128], b_oid[6], b_attrall[6], b_attr[6];

    sum = elen;

    if ((ext_attr != NULL) && (elen > 0))
    {
        b_attr[attr_len++] = 0x30;                          //ldap message
        INSERT_LENGTH(b_attr, attr_len, sum);               //size of ldap message
        sum += attr_len;
    }

    b_attrall[attrall_len++] = 0x8b;                        //response value
    INSERT_LENGTH(b_attrall, attrall_len, sum);             //insert response value
    sum += attrall_len;

    b_oid[oid_len++] = 0x8a;                                // oid name
    if ((oid != NULL) && ((olen=(int)strlen(oid)) > 0))
    {
        INSERT_LENGTH(b_oid, oid_len, olen);
    }
    else
    {
        olen = 0;
        INSERT_LENGTH(b_oid, oid_len, olen);
    }
    sum += oid_len + olen;

    b_errorMsg[errorMsg_len++] = 0x04;                      // error message
    if ((emsg != NULL) && ((erlen=(int)strlen(emsg)) > 0))
    {
        INSERT_LENGTH(b_errorMsg, errorMsg_len, erlen);
    }
    else
    {
        erlen = 0;
        INSERT_LENGTH(b_errorMsg, errorMsg_len, erlen);
    }
    sum += errorMsg_len + erlen;

    b_matchDN[matchDN_len++] = 0x04;                        // matchDN
    if ((mdn != NULL) && ((mlen=(int)strlen(mdn)) > 0))
    {
        INSERT_LENGTH(b_matchDN, matchDN_len, mlen);
    }
    else
    {
        mlen = 0;
        INSERT_LENGTH(b_matchDN, matchDN_len, mlen);
    }
    sum += matchDN_len + mlen;

    b_result[result_len++] = 0x0a;                          // resultCode
    INSERT_LENGTH(b_result, result_len, 1);
    b_result[result_len++] = (unsigned char) result;
    sum += result_len;

    b_ext[ext_len++] = 0x78;                                // protocal opt := oid_len + response_value
    INSERT_LENGTH(b_ext, ext_len, sum);                     // insert len protocol opt
    sum += ext_len;

    msg_len = ber_tid_create(mid, b_msg);
    sum += msg_len;

    CREATE_LDAP

    COPY_TO_BER(b_ext, ext_len);
    memcpy((*ber)->berval + (*ber)->len, b_result, result_len);
    (*ber)->len += result_len;
    memcpy((*ber)->berval + (*ber)->len, b_matchDN, matchDN_len);
    (*ber)->len += matchDN_len;
    if ((mdn != NULL) && (mlen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, mdn, mlen);
        (*ber)->len += mlen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_errorMsg, errorMsg_len);
    (*ber)->len += errorMsg_len;
    if ((emsg != NULL) && (erlen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, emsg, erlen);
        (*ber)->len += erlen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_oid, oid_len);
    (*ber)->len += oid_len;
    if ((oid != NULL) && (olen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, oid, olen);
        (*ber)->len += olen;
    }
    memcpy((*ber)->berval + (*ber)->len, b_attrall, attrall_len);
    (*ber)->len += attrall_len;
    memcpy((*ber)->berval + (*ber)->len, b_attr, attr_len);
    (*ber)->len += attr_len;
    if ((ext_attr != NULL) && (elen > 0))
    {
        memcpy((*ber)->berval + (*ber)->len, ext_attr, elen);
        (*ber)->len += elen;
    }
    
    return LDAP_RET_OK;
}


int
LDAP_gen_ber_string(char attrs[][128], BER **ber, char *error)
{
    int i=0, len=0;
    while (attrs[i][0]!=0)
    {
        len = (int) strlen (attrs[i]);
        ber_alloc_buffer(ber, len+2);
        (*ber)->berval[(*ber)->len] = 0x04;
        ++((*ber)->len);
        (*ber)->berval[(*ber)->len] = len;
        ++((*ber)->len);
        memcpy((*ber)->berval + (*ber)->len, attrs[i], len);
        (*ber)->len += len;
        ++(i);
    }
    return LDAP_RET_OK;
}


int
LDAP_gen_ber_attrlist(BER_ATTRLIST *attrlist, BER **ber, int free_flag, char *error)
{
    BER_ATTRLIST *al=NULL, *aa=NULL;
    BER_VALLIST *vl=NULL, *vv=NULL;
    int alen, vlen, sum;
    char name[LDAP_MAX_ATTR_NAME_SIZE+2];
    char *buff;
    int r=0;

    if (attrlist==NULL) return LDAP_RET_ERR_ENCODE;
    buff = (char *) malloc(sizeof(*buff)*BER_BUFFER);
    if (buff==NULL) return LDAP_RET_ERR_MEMORY;
    ber_alloc_buffer(ber, BER_BUFFER);
    al = aa = attrlist;
    do
    {
        sum = 0;
        alen = 0;
        name[alen++] = 0x04;
        INSERT_LENGTH(name, alen, al->len)
        memcpy(name + alen, al->name, al->len);
        alen += al->len;
        sum += alen;

        vlen = 0;
        vl = vv = al->vals;
        do
        {
            buff[vlen++] = 0x04;
            INSERT_LENGTH(buff, vlen, vl->len);
            memcpy(buff + vlen, vl->value, vl->len);
            vlen += vl->len;
            if (free_flag==1)
            {
                LINKEDLIST_REMOVE(al->vals, vl);
                free(vl);
                vl = al->vals;
            }
            else
            {
                vl = vl->next;
            }
        } while ((vl!=vv)&&(vl!=NULL));
        sum += vlen;
        if (vlen < 128)
        {
            sum += 2; // tag + length(1)
        }
        else
        {
            while ((vlen>>(r*8))>0) ++(r);
            sum += (2 + r); // tag + 8x + length octet(r)
        }

        (*ber)->berval[(*ber)->len] = 0x30;
        ++((*ber)->len);
        INSERT_LENGTH((*ber)->berval, (*ber)->len, sum)

        memcpy((*ber)->berval + (*ber)->len, name, alen);
        (*ber)->len += alen;

        (*ber)->berval[(*ber)->len] = 0x31;
        ++((*ber)->len);
        INSERT_LENGTH((*ber)->berval, (*ber)->len, vlen)

        memcpy((*ber)->berval + (*ber)->len, buff, vlen);
        (*ber)->len += vlen;

        if (free_flag==1)
        {
            LINKEDLIST_REMOVE(attrlist, al);
            free(al);
            al = attrlist;
        }
        else
        {
            al = al->next;
        }
    } while ((al!=aa)&&(al!=NULL));

    free(buff);
    return LDAP_RET_OK;
}

int
LDAP_gen_ber_modlist(BER_MODLIST *modlist, BER **ber, int free_flag, char *error)
{
    BER_MODLIST *ml, *mm;
    BER_VALLIST *vl, *vv;
    char opt[3] = {0x0a, 1, 0};
    char name[LDAP_MAX_ATTR_NAME_SIZE+2];
    char *buff;
    int mlen, vlen, tlen;
    int r=0, sum;

    if (modlist==NULL) return LDAP_RET_ERR_ENCODE;
    buff = (char *) malloc(sizeof(*buff)*BER_BUFFER);
    if (buff==NULL) return LDAP_RET_ERR_MEMORY;
    ber_alloc_buffer(ber, BER_BUFFER);
    ml = mm = modlist;
    do
    {
        opt[2] = ml->operation;
        sum = 0;
        mlen = 0;
        name[mlen++] = 0x04;                            //LDAPString
        INSERT_LENGTH(name, mlen, ml->len);             //AttributeDescription
        memcpy(name + mlen, ml->name, ml->len);
        mlen += ml->len;                                //AttributedDescription len(Type)
        sum += mlen;                                    //sum = sum + attr_len(TVL)

        vlen = 0;
        vl = vv = ml->vals;
        if(vl != NULL){
            do
            {
                buff[vlen++] = 0x04;                        //LDAPString
                INSERT_LENGTH(buff, vlen, vl->len);         //value attr
                memcpy(buff + vlen, vl->value, vl->len);
                vlen += vl->len;                            //vlen = vlen + Val_attr_len(TVL)
                if (free_flag==1)
                {
                    LINKEDLIST_REMOVE(ml->vals, vl);
                    free(vl);
                    vl = ml->vals;
                }
                else
                {
                    vl = vl->next;
                }
            } while ((vl!=vv)&&(vl!=NULL));                 //value more than one.
        }
        sum += vlen;                                    //sum = sum + vlen

        if (vlen < 128)
        {
            tlen = 2;                                   // mod Vals tag + length(1)
        }
        else
        {
            while ((vlen>>(r*8))>0) ++(r);
            tlen = (2 + r);                             // mod Vals tag + 8r + length octet(r)
        }

        sum += tlen;                                    //sum = sum + tlen(length of vals)

        if (sum < 128)
        {
            sum += 2;                                   // Change tag + length(1)
        }
        else
        {
            while ((sum>>(r*8))>0) ++(r);
            sum += (2 + r);                             // Change tag + 8r + length octet(r)
        }
        sum += 3;                                       // operation ENUMERATED length

        (*ber)->berval[(*ber)->len] = 0x30;             //Change sequence
        ++((*ber)->len);
        INSERT_LENGTH((*ber)->berval, (*ber)->len, sum)

        memcpy((*ber)->berval + (*ber)->len, opt, 3);
        (*ber)->len += 3;

        tlen = tlen + vlen + mlen;
        (*ber)->berval[(*ber)->len] = 0x30;             //Partial Attribute
        ++((*ber)->len);
        INSERT_LENGTH((*ber)->berval, (*ber)->len, tlen)

        memcpy((*ber)->berval + (*ber)->len, name, mlen);
        (*ber)->len += mlen;

        (*ber)->berval[(*ber)->len] = 0x31;             //Vals SET
        ++((*ber)->len);
        INSERT_LENGTH((*ber)->berval, (*ber)->len, vlen)

        memcpy((*ber)->berval + (*ber)->len, buff, vlen);
        (*ber)->len += vlen;

        if (free_flag==1)
        {
            LINKEDLIST_REMOVE(modlist, ml);
            free(ml);
            ml = modlist;
        }
        else
        {
            ml = ml->next;
        }
    } while ((ml!=mm)&&(ml!=NULL));
    
    free(buff);
    return LDAP_RET_OK;
}

void
LDAP_ber_free(BER *ber)
{
    if (ber != NULL) free(ber);
}

int LDAP_filter_to_string(SearchFilter *filter, char *expression)
{
    switch(filter->choice)
    {
        case FAND:
        {
            strcat(expression, "(&");
            while(filter->alt.and.array_p != NULL)
            {
                char expression_a[256]; expression_a[0] = 0;
                SearchFilter *list_and = filter->alt.and.array_p;
                LDAP_filter_to_string(list_and, expression_a);
                strcat(expression, expression_a);
                LINKEDLIST_REMOVE(filter->alt.and.array_p, list_and);
            }
            strcat(expression, ")");
            SDF_FREE(filter->alt.and.array_p);
        }
        break;
        case FOR:
        {
            strcat(expression, "(|");
            while(filter->alt.or.array_p != NULL)
            {
                char expression_a[256]; expression_a[0] = 0;
                SearchFilter *list_or = filter->alt.or.array_p;
                LDAP_filter_to_string(list_or, expression_a);
                strcat(expression, expression_a);
                LINKEDLIST_REMOVE(filter->alt.or.array_p, list_or);
            }
            strcat(expression, ")");
            SDF_FREE(filter->alt.or.array_p);
        }
        break;
        case FNOT:
        {
            char expression_a[256]; expression_a[0] = 0;
            SearchFilter *list_not = filter->alt.not;
            LDAP_filter_to_string(list_not, expression_a);
            sprintf(expression,"(!%s)", expression_a);
            LINKEDLIST_REMOVE(filter->alt.not, list_not);
            SDF_FREE(filter->alt.not);
        }
        break;
        case FEQUALITYMATCH:
        {
            sprintf(expression,"(%.*s=%.*s)", filter->alt.equalityMatch.len, filter->alt.equalityMatch.name, filter->alt.equalityMatch.vals->len, filter->alt.equalityMatch.vals->value);
            SDF_FREE(filter->alt.equalityMatch.vals);
        }
        break;
        case FSUBSTRINGS:
        {
            char substrings_value_a[512]; substrings_value_a[0] = 0;
            char *ck_str_p = substrings_value_a;
            if(filter->alt.substrings.flag[SUBSTRING_VALUE] == 1 && filter->alt.substrings.flag[SUBSTRING_TYPE] == 1)
            {
                while(filter->alt.substrings.comps.substrings.array_p != NULL)
                {
                    Substrings *list_substrings = filter->alt.substrings.comps.substrings.array_p;
                    if(list_substrings->choice == SUBSTRING_INITIAL)
                    {
                        sprintf(substrings_value_a, "%.*s*", list_substrings->alt.initial.len, list_substrings->alt.initial.value);
                        ck_str_p += list_substrings->alt.initial.len;
                        LINKEDLIST_REMOVE(filter->alt.substrings.comps.substrings.array_p,list_substrings);
                    }
                    else if(list_substrings->choice == SUBSTRING_ANY)
                    {
                        int substrings_any_len = list_substrings->alt.any.len;
                        char *substrings_any_a; substrings_any_a[0] = 0;
                        substrings_any_a = list_substrings->alt.any.value;
                        if(*ck_str_p == 0x2a)
                        {
                            strncat(substrings_value_a, substrings_any_a, substrings_any_len);
                            strcat(substrings_value_a, "*");
                        }
                        else
                        {
                            strcat(substrings_value_a, "*");
                            strncat(substrings_value_a, substrings_any_a, substrings_any_len);
                            strcat(substrings_value_a, "*");
                        }
                        ck_str_p = (ck_str_p + substrings_any_len + 1) ;
                        LINKEDLIST_REMOVE(filter->alt.substrings.comps.substrings.array_p,list_substrings);
                    }
                    else if(list_substrings->choice == SUBSTRING_FINAL)
                    {
                        int substrings_final_len = list_substrings->alt.final.len;
                        char *substrings_final_a; substrings_final_a[0] = 0;
                        substrings_final_a = list_substrings->alt.final.value;
                        if(*ck_str_p == 0x2a)
                        {
                            strncat(substrings_value_a, substrings_final_a, substrings_final_len);
                        }
                        else
                        {
                            strcat(substrings_value_a, "*");
                            strncat(substrings_value_a, substrings_final_a, substrings_final_len);
                        }
                        LINKEDLIST_REMOVE(filter->alt.substrings.comps.substrings.array_p,list_substrings);
                    }
                    SDF_FREE(list_substrings);
                }
            }

            sprintf(expression,"(%.*s=%s)", filter->alt.substrings.comps.type.len, filter->alt.substrings.comps.type.value, substrings_value_a);
        }
        break;
        case FGREATEROREQUAL:
        {
            sprintf(expression,"(%.*s>=%.*s)", filter->alt.greaterOrEqual.len, filter->alt.greaterOrEqual.name, filter->alt.greaterOrEqual.vals->len, filter->alt.greaterOrEqual.vals->value);
            SDF_FREE(filter->alt.greaterOrEqual.vals);
        }
        break;
        case FLESSOREQUAL:
        {
            sprintf(expression,"(%.*s<=%.*s)", filter->alt.lessOrEqual.len, filter->alt.lessOrEqual.name, filter->alt.lessOrEqual.vals->len, filter->alt.lessOrEqual.vals->value);
            SDF_FREE(filter->alt.lessOrEqual.vals);
        }
        break;
        case FPRESENT:
        {
            sprintf(expression,"(%.*s=*)", filter->alt.present.len, filter->alt.present.value);
        }
        break;
        case FAPPROXMATCH:
        {
            sprintf(expression,"(%.*s~=%.*s)", filter->alt.approxMatch.len, filter->alt.approxMatch.name, filter->alt.approxMatch.vals->len, filter->alt.approxMatch.vals->value);
            SDF_FREE(filter->alt.approxMatch.vals);
        }
        break;
        case FEXTENSIBLEMATCH:
        {
            strcat(expression,"(");
            
            if(filter->alt.extensibleMatch.flag[ASSERT_TYPE] == 1)
            {    
                strncat(expression, filter->alt.extensibleMatch.comps.type.value, filter->alt.extensibleMatch.comps.type.len);
            }
            if(filter->alt.extensibleMatch.flag[ASSERT_TYPE] == 1 && filter->alt.extensibleMatch.comps.dn_attributes == 1)
            {
                strcat(expression,":dn");
            }
            if(filter->alt.extensibleMatch.flag[ASSERT_MATCHINGRULE] == 1)
            {
                strcat(expression,":");
                strncat(expression, filter->alt.extensibleMatch.comps.matching_rule.value, filter->alt.extensibleMatch.comps.matching_rule.len);
            }
            if(filter->alt.extensibleMatch.flag[ASSERT_MATCHVALUE] == 1)
            {
                strcat(expression,":=");
                strncat(expression, filter->alt.extensibleMatch.comps.match_value.value, filter->alt.extensibleMatch.comps.match_value.len);
            }
            strcat(expression,")");
        }
        break;
    }
    return 1;
}

int LDAP_string_to_filter(char *expression, SearchFilter *filter)
{
    int i = 0,pp = 0,len_size, buf_len;
    int length = 0, len = 0;
    char* p = NULL,*_p = NULL;

    switch ((unsigned char)expression[0]&0x1F)
    {
        case FAND:
        {
            filter->choice = FAND;
            i++;
            FIND_LENGTH(expression, length)
            char and_temp_a[512];
            and_temp_a[0] = 0;

            while(i<=length) //!--Loop for temp in case And between (1..Max) by length
            {
                SearchFilter *filter_and = (SearchFilter *) SDF_MALLOC(sizeof(SearchFilter));
                if (filter_and == NULL) return LDAP_RET_ERR_MEMORY;
                (void) memset(filter_and, 0, sizeof(SearchFilter));
                len_size = 0;
                _p = &expression[i];
                i++;
                FIND_LEN(expression, len, len_size)
                memcpy(and_temp_a, _p, (len + len_size + 2));
                and_temp_a[len + len_size + 2] = 0;
                LDAP_string_to_filter(and_temp_a, filter_and);
                LINKEDLIST_APPEND(filter->alt.and.array_p, filter_and);
                i += len;
            }
        }
            break;
        case FOR:
        {
            filter->choice = FOR;
            i++;
            FIND_LEN(expression, length, len_size)
            char or_temp_a[512];
            or_temp_a[0] = 0;

            while(i<=length) //!--Loop for temp in case Or between (1..Max) by length
            {
                SearchFilter *filter_or = (SearchFilter *) SDF_MALLOC(sizeof(SearchFilter));
                if (filter_or==NULL) return LDAP_RET_ERR_MEMORY;
                (void) memset(filter_or, 0, sizeof(SearchFilter));
                len_size = 0;
                _p = &expression[i];
                i++;
                FIND_LEN(expression, len ,len_size)
                memcpy(or_temp_a, _p, (len + len_size + 2));
                or_temp_a[len + len_size + 2] = 0;
                LDAP_string_to_filter(or_temp_a, filter_or);
                LINKEDLIST_APPEND(filter->alt.or.array_p, filter_or);
                i += len;
            }
        }
            break;
        case FNOT:
        {
            filter->choice = FNOT;
            i++;
            FIND_LENGTH(expression, length)
            char not_temp_a[512];
            not_temp_a[0] = 0;
            SearchFilter *filter_not = (SearchFilter *) SDF_MALLOC(sizeof(SearchFilter));
            if (filter_not==NULL) return LDAP_RET_ERR_MEMORY;
            (void) memset(filter_not, 0, sizeof(SearchFilter));
            memcpy(not_temp_a, (expression + i), length);
            not_temp_a[length] = 0;
            LDAP_string_to_filter(not_temp_a, filter_not);
            LINKEDLIST_APPEND(filter->alt.not, filter_not);
            i += length;
        }
            break;
        case FEQUALITYMATCH:
        {
            filter->choice = FEQUALITYMATCH;
            i++;
            FIND_LENGTH(expression, length)

            i++;
            FIND_LENGTH(expression, length)
            memcpy(filter->alt.equalityMatch.name, (expression + i), length);
            filter->alt.equalityMatch.name[length] = 0;
            filter->alt.equalityMatch.len = length;
            i += length;

            i++;
            FIND_LENGTH(expression, length)
            filter->alt.equalityMatch.vals = (BER_VALLIST *) SDF_MALLOC(sizeof(BER_VALLIST));
            memcpy(filter->alt.equalityMatch.vals->value, (expression + i), length);
            filter->alt.equalityMatch.vals->value[length] = 0;
            filter->alt.equalityMatch.vals->len = length;
        }
            break;
        case FSUBSTRINGS:
        {
            filter->choice = FSUBSTRINGS;
            i++;
            FIND_LENGTH(expression, length)

            if((unsigned char)expression[i] == 0x04)
            {
                i++;
                FIND_LENGTH(expression, len)
                filter->alt.substrings.flag[SUBSTRING_TYPE] = 1;
                memcpy(filter->alt.substrings.comps.type.value, (expression + i), len);
                filter->alt.substrings.comps.type.value[len] = 0;
                filter->alt.substrings.comps.type.len = len;
                i += len;

                if((unsigned char)expression[i] == 0x30)
                {
                    i++;
                    FIND_LENGTH(expression, len)
                    filter->alt.substrings.flag[SUBSTRING_VALUE] = 1;

                    while(i <= length) //!--Loop for temp in case Any(0x81) between (1..Max) by length
                    {
                        Substrings *list_substrings = (Substrings *) SDF_MALLOC(sizeof(Substrings));
                        if((unsigned char)expression[i] == 0x80)
                        {
                            i++;
                            FIND_LENGTH(expression, len) 
                            list_substrings->choice = SUBSTRING_INITIAL;
                            memcpy(list_substrings->alt.initial.value, (expression + i), len);
                            list_substrings->alt.initial.value[len] = 0;
                            list_substrings->alt.initial.len = len;
                            LINKEDLIST_APPEND(filter->alt.substrings.comps.substrings.array_p, list_substrings);
                            i += len;
                        }
                        else if((unsigned char)expression[i] == 0x81)
                        {
                            i++;
                            FIND_LENGTH(expression, len)
                            list_substrings->choice = SUBSTRING_ANY;
                            memcpy(list_substrings->alt.any.value, (expression + i), len);
                            list_substrings->alt.any.value[len] = 0;
                            list_substrings->alt.any.len = len;
                            LINKEDLIST_APPEND(filter->alt.substrings.comps.substrings.array_p, list_substrings);
                            i += len;
                        }
                        else if((unsigned char)expression[i] == 0x82)
                        {
                            i++;
                            FIND_LENGTH(expression, len)
                            list_substrings->choice = SUBSTRING_FINAL;
                            memcpy(list_substrings->alt.final.value, (expression + i), len);
                            list_substrings->alt.final.value[len] = 0;
                            list_substrings->alt.final.len = len;
                            LINKEDLIST_APPEND(filter->alt.substrings.comps.substrings.array_p, list_substrings);
                            i += len;
                        }
                    }
                }
            }
        }
            break;
        case FGREATEROREQUAL:
        {
            filter->choice = FGREATEROREQUAL;
            i++;
            FIND_LENGTH(expression, length)

            i++;
            FIND_LENGTH(expression, length)
            memcpy(filter->alt.greaterOrEqual.name, (expression + i), length);
            filter->alt.greaterOrEqual.name[length] = 0;
            filter->alt.greaterOrEqual.len = length;
            i += length;

            i++;
            FIND_LENGTH(expression, length)
            filter->alt.greaterOrEqual.vals = (BER_VALLIST *) SDF_MALLOC(sizeof(BER_VALLIST));
            memcpy(filter->alt.greaterOrEqual.vals->value, (expression + i), length);
            filter->alt.greaterOrEqual.vals->value[length] = 0;
            filter->alt.greaterOrEqual.vals->len = length;
        }
            break;
        case FLESSOREQUAL:
        {
            filter->choice = FLESSOREQUAL;
            i++;
            FIND_LENGTH(expression, length)

            i++;
            FIND_LENGTH(expression, length)
            memcpy(filter->alt.lessOrEqual.name, (expression + i), length);
            filter->alt.lessOrEqual.name[length] = 0;
            filter->alt.lessOrEqual.len = length;
            i += length;

            i++;
            FIND_LENGTH(expression, length)
            filter->alt.lessOrEqual.vals = (BER_VALLIST *) SDF_MALLOC(sizeof(BER_VALLIST));
            memcpy(filter->alt.lessOrEqual.vals->value, (expression + i), length);
            filter->alt.lessOrEqual.vals->value[length] = 0;
            filter->alt.lessOrEqual.vals->len = length;
        }
            break;
        case FAPPROXMATCH:
        {
            filter->choice = FAPPROXMATCH;
            i++;
            FIND_LENGTH(expression, length)

            i++;
            FIND_LENGTH(expression, length)
            memcpy(filter->alt.approxMatch.name, (expression + i), length);
            filter->alt.approxMatch.name[length] = 0;
            filter->alt.approxMatch.len = length;
            i += length;

            i++;
            FIND_LENGTH(expression, length)
            filter->alt.approxMatch.vals = (BER_VALLIST *) SDF_MALLOC(sizeof(BER_VALLIST));
            memcpy(filter->alt.approxMatch.vals->value, (expression + i), length);
            filter->alt.approxMatch.vals->value[length] = 0;
            filter->alt.approxMatch.vals->len = length;
        }
            break;
        case FPRESENT:
        {
            filter->choice = FPRESENT;
            i++;
            FIND_LENGTH(expression, length)

            memcpy(filter->alt.present.value, (expression + i), length);       
            filter->alt.present.value[length] = 0;
            filter->alt.present.len = length;
            i += length;
        }
            break;
        case FEXTENSIBLEMATCH:
        {
            filter->choice = FEXTENSIBLEMATCH;
            i++;
            FIND_LENGTH(expression, length)
            
            if((unsigned char)expression[i] == 0x81)
            {
                i++;
                FIND_LENGTH(expression, length)
                filter->alt.extensibleMatch.flag[ASSERT_MATCHINGRULE] = 1;
                memcpy(filter->alt.extensibleMatch.comps.matching_rule.value, (expression + i), length);
                filter->alt.extensibleMatch.comps.matching_rule.value[length] = 0;
                filter->alt.extensibleMatch.comps.matching_rule.len = length;
                i+=length;
            }
            if((unsigned char)expression[i] == 0x82)
            {
                i++;
                FIND_LENGTH(expression, length)
                filter->alt.extensibleMatch.flag[ASSERT_TYPE] = 1;
                memcpy(filter->alt.extensibleMatch.comps.type.value, (expression + i), length);
                filter->alt.extensibleMatch.comps.type.value[length] = 0;
                filter->alt.extensibleMatch.comps.type.len = length;
                i+=length;
            }
            if((unsigned char)expression[i] == 0x83)
            {
                i++;
                FIND_LENGTH(expression, length)
                filter->alt.extensibleMatch.flag[ASSERT_MATCHVALUE] = 1;
                memcpy(filter->alt.extensibleMatch.comps.match_value.value, (expression + i), length);
                filter->alt.extensibleMatch.comps.match_value.value[length] = 0;
                filter->alt.extensibleMatch.comps.match_value.len = length;
                i+=length;
            }
            if((unsigned char)expression[i] == 0x84)
            {
                i++;
                FIND_LENGTH(expression, length)
                filter->alt.extensibleMatch.flag[ASSERT_DNATTRIBUTES] = 1;
                filter->alt.extensibleMatch.comps.dn_attributes = 1;
                i+=length;
            }
        }
            break;
        default: break;  
    }
    
    return 1;
}

int
LBER_attr_decode(LDAP_CLNT *lc, int format, char *error)
{
    if (lc==NULL) return LDAP_RET_ERR_PARAMETER;

    ber_attr_to_string(lc, format, error);
    free(lc->rber);
    lc->rber = NULL;

    return LDAP_RET_OK;
}

int
LBER_object_decode(LDAP_CLNT *lc, LDAP_OBJECT **obj, char *error){


    if (lc==NULL) return LDAP_RET_ERR_PARAMETER;
    if (ber_attr_to_obj(lc,obj, error) == LDAP_RET_ERR_DECODE) return LDAP_RET_ERR_DECODE;
    free(lc->rber);
    lc->rber = NULL;

    return LDAP_RET_OK;
}

int
LBER_attrlist_add(BER_ATTRLIST **attrlist, char name[LDAP_MAX_ATTR_NAME_SIZE], BER_VALLIST *vallist, char *error)
{
    if (attrlist==NULL) return LDAP_RET_ERR_ENCODE;
    BER_ATTRLIST *attr=NULL;

    attr = (BER_ATTRLIST *) malloc (sizeof(*attr));
    attr->len = (int) strlen(name);
    if (attr->len >= LDAP_MAX_ATTR_NAME_SIZE) attr->len = LDAP_MAX_ATTR_NAME_SIZE - 1;
    strncpy(attr->name, name, LDAP_MAX_ATTR_NAME_SIZE-1);
    attr->vals = vallist;
    LINKEDLIST_APPEND((*attrlist), attr);

    return LDAP_RET_OK;
}

int
LBER_modlist_add(BER_MODLIST **modlist, int operation, char name[LDAP_MAX_ATTR_NAME_SIZE], BER_VALLIST *vallist, char *error)
{
    if (modlist==NULL) return LDAP_RET_ERR_ENCODE;
    BER_MODLIST *mod=NULL;

    mod = (BER_MODLIST *) malloc (sizeof(*mod));
    mod->operation = operation;
    mod->len = (int) strlen(name);
    if (mod->len >= LDAP_MAX_ATTR_NAME_SIZE) mod->len = LDAP_MAX_ATTR_NAME_SIZE - 1;
    strncpy(mod->name, name, LDAP_MAX_ATTR_NAME_SIZE-1);
    mod->vals = vallist;
    LINKEDLIST_APPEND((*modlist), mod);

    return LDAP_RET_OK;
}

int
LBER_valuelist_add(BER_VALLIST **vallist, char value[LDAP_MAX_ATTR_VALUE_SIZE], char *error)
{
    if (vallist==NULL) return LDAP_RET_ERR_ENCODE;
    BER_VALLIST *val=NULL;

    val = (BER_VALLIST *) malloc (sizeof(*val));
    val->len = (int) strlen(value);
    if (val->len >= LDAP_MAX_ATTR_VALUE_SIZE) val->len = LDAP_MAX_ATTR_VALUE_SIZE - 1;
    strncpy(val->value, value, LDAP_MAX_ATTR_VALUE_SIZE-1);
    LINKEDLIST_APPEND((*vallist), val);

    return LDAP_RET_OK;
}

