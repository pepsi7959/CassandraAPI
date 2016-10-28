
/*
   Copyright (C) 2008-2009, TORO, MIMO Tech Co., Ltd.

   base64.c

   Revision history

   1.0.1   Fix bug in base64_decode() when found PAD_CHAR and output buffer's size is just enough.
   1.0.0   Initialize.

*/

#include <stdio.h>
#include <string.h>
#include "base64.h"

#define _BASE64_PAD_CHAR_         '='

#ifndef _BASE64_CHAR_63_
#define _BASE64_CHAR_63_          '+'
#define _BASE64_CHAR_63_STRING_   "+"
#endif

#ifndef _BASE64_CHAR_64_
#define _BASE64_CHAR_64_          '/'
#define _BASE64_CHAR_64_STRING_   "/"
#endif

static const char _base64_charcters_[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" _BASE64_CHAR_63_STRING_ _BASE64_CHAR_64_STRING_;

/*

   +--first octet--+-second octet--+--third octet--+
   |7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|
   +-----------+---+-------+-------+---+-----------+
   |5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|
   +--1.index--+--2.index--+--3.index--+--4.index--+

   11111111 11111111 11111111   3 bytes (8 bits)

   111111                       6 bits
         11 1111                2+4 bits
                1111 11         4+2 bits
                       111111   6 bits
*/

#define _BASE64_ENCODE_(_i,_ilen,_o)                                                                                  \
{                                                                                                                     \
   register unsigned char *_i_ = (unsigned char *)(_i);                                                               \
   register unsigned char *_o_ = (unsigned char *)(_o);                                                               \
   _o_[0] = _base64_charcters_[ _i_[0] >> 2 ];                                                                        \
   _o_[1] = _base64_charcters_[ ((_i_[0] & 0x03) << 4) | ((_i_[1] & 0xF0) >> 4)];                                     \
   _o_[2] = (_ilen <= 1)? _BASE64_PAD_CHAR_ : _base64_charcters_[ ((_i_[1] & 0x0F) << 2) | ((_i_[2] & 0xC0) >> 6)];   \
   _o_[3] = (_ilen <= 2)? _BASE64_PAD_CHAR_ : _base64_charcters_[ _i_[2] & 0x3F ];                                    \
}

#define _BASE64_DECODE_CHAR_64_(_c)  ( ((_c)==_BASE64_CHAR_64_)?   (26+26+10+1)     : 0xFF                          )
#define _BASE64_DECODE_CHAR_63_(_c)  ( ((_c)==_BASE64_CHAR_63_)?   (26+26+10)       : _BASE64_DECODE_CHAR_64_((_c)) )
#define _BASE64_DECODE_09_(_c)       ( ((_c)>='0'&&(_c)<='9')?     ((_c)-'0'+26+26) : _BASE64_DECODE_CHAR_63_((_c)) )
#define _BASE64_DECODE_az_(_c)       ( ((_c)>='a'&&(_c)<='z')?     ((_c)-'a'+26)    : _BASE64_DECODE_09_((_c))      )
#define _BASE64_DECODE_AZ_(_c)       ( ((_c)>='A'&&(_c)<='Z')?     ((_c)-'A')       : _BASE64_DECODE_az_((_c))      )
#define _BASE64_DECODE_CHAR(_c)      ( (unsigned char)_BASE64_DECODE_AZ_((_c))                                      )

#define _BASE64_DECODE_(_i,_o,_olen)                                    \
{                                                                       \
   register char *_i_ = (char *)(_i);                                   \
   register unsigned char *_o_ = (unsigned char *)(_o);                 \
   register unsigned char _h_, _l_;                                     \
   do                                                                   \
   {                                                                    \
      if ((_l_ = _BASE64_DECODE_CHAR(_i_[0])) >= 64)                    \
      {                                                                 \
         (_olen) = -1;                                                  \
         break;                                                         \
      }                                                                 \
      _h_ = (_l_ << 2);                                                 \
      if ((_l_ = _BASE64_DECODE_CHAR(_i_[1])) >= 64)                    \
      {                                                                 \
         (_olen) = -2;                                                  \
         break;                                                         \
      }                                                                 \
      _o_[0] = _h_ | (_l_ >> 4);                                        \
      _h_ = (_l_ << 4);                                                 \
      if (_i_[2] == _BASE64_PAD_CHAR_ && _i_[3] == _BASE64_PAD_CHAR_)   \
      {                                                                 \
         (_olen) = 1;                                                   \
         break;                                                         \
      }                                                                 \
      if ((_l_ = _BASE64_DECODE_CHAR(_i_[2])) >= 64)                    \
      {                                                                 \
         (_olen) = -3;                                                  \
         break;                                                         \
      }                                                                 \
      _o_[1] = _h_ | (_l_ >> 2);                                        \
      _h_ = (_l_ << 6);                                                 \
      if (_i_[3] == _BASE64_PAD_CHAR_)                                  \
      {                                                                 \
         (_olen) = 2;                                                   \
         break;                                                         \
      }                                                                 \
      if ((_l_ = _BASE64_DECODE_CHAR(_i_[3])) >= 64)                    \
      {                                                                 \
         (_olen) = -4;                                                  \
         break;                                                         \
      }                                                                 \
      _o_[2] = _h_ | _l_;                                               \
      (_olen) = 3;                                                      \
   } while (0);                                                         \
}


void
base64_encode (char **in, int *ilen, char **out, int *olen)
{
   while (*ilen > 0 && *olen >= 4)
   {
      _BASE64_ENCODE_(*in,*ilen,*out)
      if (*ilen < 3)
      {
         *in += *ilen;
         *ilen = 0;
      }
      else
      {
         *in += 3;
         *ilen -= 3;
      }
      *out += 4;
      *olen -= 4;
   }
}


int
base64_decode (char **in, int *ilen, char **out, int *olen)
{
   int n;

   while (*ilen >= 4 && *olen >= 3)
   {
      _BASE64_DECODE_(*in,*out,n)
      if (n <= 0)
         return -1;
      *out += n;
      *olen -= n;
      *in += 4;
      *ilen -= 4;
      if (n < 3)
         return 0;
   }
   if (*ilen <= 0)
      return 0;
   if (*ilen < 4)
      return -1;
   if (*olen > 0)
   {
      char *s = *in;
      char *t, temp[3];
      t = temp;
      _BASE64_DECODE_(s,t,n)
      if (n <= 0)
         return -1;
      if (n <= *olen)
      {
         (void) memcpy (*out, temp, n);
         *out += n;
         *olen -= n;
         *in += 4;
         *ilen -= 4;
      }
   }
   return 0;
}

