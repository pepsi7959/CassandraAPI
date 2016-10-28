#include <stdio.h>
#include <stdlib.h>
#include "urlencode.h"

int main(int argc, char *argv[]){
    char data[] = "http://a/b/c /g;x?y#s";
    char *url;
    char *encode;
    char *decode;

    url = data;

    printf("url : %s\n",url);

    encode = url_encode_init(url);
    printf("after encode : %s\n",encode);

    decode = url_decode_init(encode);
    printf("after decode : %s\n", decode);

    url_decode_free(decode);
    url_encode_free(encode);
return 0;
}
