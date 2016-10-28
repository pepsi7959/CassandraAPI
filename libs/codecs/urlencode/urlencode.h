#ifndef _URLENCODE_H_
#define _URLENCODE_H_

#define url_decode_free(_url_) url_free(_url_);
#define url_encode_free(_url_) url_free(_url_);

/* Converts a hex character to its integer value */
char from_hex(char ch);

/* Converts an integer value to its hex character*/
char to_hex(char code);

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode_init(char *str);

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode_init(char *str);

/* Free data */
void url_free(char *url);
#endif
