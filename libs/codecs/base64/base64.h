
/*
   Copyright (C) 2008-2009, TORO, MIMO Tech Co., Ltd.

   base64.h

   Revision history

   1.0.0   Initialize.

*/

#ifndef _BASE64_H_
#define _BASE64_H_ 1

#ifdef __cplusplus
extern "C" {
#endif


#define BASE64_ENCODE_SIZE(_len)   ( (((_len)+(3-1)) / 3) * 4 )
#define BASE64_DECODE_SIZE(_len)   ( (((_len)+(4-1)) / 4) * 3 )


void base64_encode (char **in, int *ilen, char **out, int *olen);
int base64_decode (char **in, int *ilen, char **out, int *olen);


#ifdef __cplusplus
}
#endif

#endif

