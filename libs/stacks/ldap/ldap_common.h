
/*
 Copyright (C) 2016, TORO, MIMO Tech Co., Ltd.

 ldap_common.h

 Revision history
 1.0.0   Initialize.

 */
#include "common.h"

#ifndef _LDAP_COMMON_H_
#define _LDAP_COMMON_H_

enum LDAP_NUM_TYPE
{
    LDAP_NUM_ASCII = 0,
    LDAP_NUM_LHEX,
    LDAP_NUM_UHEX,
    LDAP_NUM_LBCD,
    LDAP_NUM_UBCD,

    LDAP_NUM_LAST
};

#define ADJUST_SIZE(_l,_s)                              \
{                                                       \
register int _r_ = (_l) % (_s);                         \
if (_r_ > 0)                                            \
(_l) += (_s)-_r_;                                       \
}

int timeval_subtract (struct timeval *result, struct timeval *x,struct timeval *y);
int tid_increment(LDAP_t *dc);

int ldap_send_bind_rqt(LDAP_t *dc, LDAP_CLNT *lc);
int ldap_send_bind_resp(DS_t *ds, DSSESSION *sess, int result_code);
int ldap_recv_bind_rqt(DS_t *ds, DSSESSION *sess);
int ldap_recv_bind_resp(LDAP_t *dc, LDAP_CLNT *lc);
int ldap_send_generic_resp(DS_t *ds, DSSESSION *sess, int result_code, LDAPMessage_t *msg);
int ldap_send_keepalive_rqt (LDAP_t *dc, LDAP_CLNT *lc, BER *ber, char *error);
int ldap_send_keepalive_resp (DS_t *ds, DSSESSION *sess, BER *ber, char *error);

char *state_to_string(int state);
char *error_to_string(int result);

void bin_to_hex(char *buff, char *val, int vlen, int type);
void bin_to_bcd(char *buff, char *val, int vlen, int type);

int ber_alloc_buffer(BER **ber, int len);
int ber_tid_create(long tid, char *b_msg);
int ber_attr_to_obj(LDAP_CLNT *lc, LDAP_OBJECT **object, char *error);
int ber_attr_to_string(LDAP_CLNT *lc, int format, char *error);

int ldap_decode_request(DSSESSION *sess, int decode_flag, char *error);
int ldap_decode_response(LDAP_CLNT *lc, int decode_flag, char *error);

#endif

