
/*
 Copyright (C) 2014, TORO, MIMO Tech Co., Ltd.

 rauthen.h

 Revision history

 1.0.0   Initialize.

 */
#include "common.h"

#ifndef _R_AUTHEN_H_
#define _R_AUTHEN_H_

#define USER_RHOSTS_FILE                            "/.rhosts"
#define INIT_RHOST_BUFFER                           256
#define RAUTHEN_MAX_LINE_LENGTH                     64

enum RAUTHEN_RETURN
{
    RAUTHEN_FAIL = 0,
    RAUTHEN_PASS
};

typedef struct _host_equiv_
{
	struct _host_equiv_ *prev;
	struct _host_equiv_ *next;
	size_t len;
	char data[RAUTHEN_MAX_LINE_LENGTH];
} HOST_EQUIV;

int rauthen_init(HOST_EQUIV **hl, char *error);
int rauthen_verify(HOST_EQUIV *hl, unsigned int raddr , const char *rhost, char *message);
void rauthen_free(HOST_EQUIV *hl, char *error);

#endif

