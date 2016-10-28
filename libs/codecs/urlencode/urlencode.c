/*
    Copyright (C) 2008-2012, TORO Co., Ltd.

    urlencode.c

    Revision history
    0.0.1   intialize
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "urlencode.h"

/* Converts a hex character to its integer value */
char from_hex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char to_hex(char code) {
  	static char hex[] = "0123456789abcdef";
  	return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode_init(char *str) {
  	char *pstr = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
  	while (*pstr) {
    	if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
      		*pbuf++ = *pstr;
#ifdef SUPPORT_HTML5
    else if (*pstr == ' ') 
     	*pbuf++ = '+';
#endif
    else 
      	*pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    pstr++;
  	}
  	*pbuf = '\0';
  	return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode_init(char *str) {
  	char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
  	while (*pstr) {
    	if (*pstr == '%') {
      		if (pstr[1] && pstr[2]) {
        		*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        		pstr += 2;
      		}
#ifdef SUPPORT_HTML5
    	} else if (*pstr == '+') { 
      		*pbuf++ = ' ';
#endif
    	} else {
      		*pbuf++ = *pstr;
    	}
    	pstr++;
  	}
  	*pbuf = '\0';
 	return buf;
}

/* Free data */
void url_free(char *url){
    free(url);
}
