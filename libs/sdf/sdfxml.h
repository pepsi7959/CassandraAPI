
/*
   Copyright (C) 2014, MIMO Tech Co., Ltd.

   sdfxml.h

   Revision history

   1.0.0   Initialize.

*/

#ifndef _SDFXML_H_
#define _SDFXML_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SDF_XML_NAME_SIZE					1024

enum SDF_XML_RET_CODE
{
   SDF_XML_RET_ERROR = -1000,
   SDF_XML_RET_OK = 0,
   SDF_XML_RET_EOF,
};

struct _sdf_xml_element_
{
   char *element;
   int element_len;
   int element_close;
   char *attr;
   int attr_len;
   char *content;
   int content_len;
   char *data;
   int data_len;
};
typedef struct _sdf_xml_element_ SDF_XML;


void SDF_xml_decode_data (char *s, int len, char *t);
int SDF_xml_encode_data (char *s, int len, char *t);
int SDF_xml_find_start_element (char *data, int *curr, int len, SDF_XML *xml, char *err);
int SDF_xml_find_element (char *data, int *curr, int len, SDF_XML *xml, char *err);
int SDF_xml_find_element_by_name (char *name, char *data, int *curr, int len, SDF_XML *xml, char *err);
int SDF_xml_find_attribute (char *data, int *curr, int len, char **name, int *nlen, char **value, int *vlen, char *err);



#ifdef __cplusplus
}
#endif

#endif

