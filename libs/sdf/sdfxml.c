
/*
   Copyright (C) 2009, Program Thai Co., Ltd.

   sfxml.c

   Revision history

   1.0.0   Initialize.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdfxml.h"


#define IS_SPACE(_c)   ( (_c) == ' ' || (_c) == '\n' || (_c) == '\t' || (_c) == '\r' )


void
SDF_xml_decode_data (char *s, int len, char *t)
{
#define _DECODE_CONST_STR(_str,_c)                \
{                                                 \
   if (memcmp (s, (_str), sizeof(_str)-1) == 0)   \
   {                                              \
      *t++ = (_c);                                \
      s += sizeof(_str)-1;                        \
      len -= sizeof(_str)-1;                      \
      continue;                                   \
   }                                              \
}
   while (len > 0)
   {
      if (*s == '&')
      {
         _DECODE_CONST_STR("&lt;",'<')
         _DECODE_CONST_STR("&gt;",'>')
         _DECODE_CONST_STR("&amp;",'&')
         _DECODE_CONST_STR("&apos;",'\'')
         _DECODE_CONST_STR("&quot;",'"')
      }
      *t++ = *s++;
      --len;
   }
   *t = '\0';
}


int
SDF_xml_encode_data (char *s, int len, char *t)
{
#define _COPY_CONST_STR(_str)                     \
{                                                 \
   (void) memcpy (t, (_str), sizeof(_str)-1);     \
   t += sizeof(_str)-1;                           \
}
#define _CHECK_ALREADY_ENCODED(_str)              \
{                                                 \
   if (memcmp (s, (_str), sizeof(_str)-1) == 0)   \
   {                                              \
      *t++ = *s;                                  \
      break;                                      \
   }                                              \
}
   char *tt = t;

   while (len > 0)
   {
      switch (*s)
      {
         case '<' :
            _COPY_CONST_STR("&lt;")
            break;
         case '>' :
            _COPY_CONST_STR("&gt;")
            break;
         case '&' :
			_CHECK_ALREADY_ENCODED("&lt;")
			_CHECK_ALREADY_ENCODED("&gt;")
			_CHECK_ALREADY_ENCODED("&amp;")
			_CHECK_ALREADY_ENCODED("&apos;")
			_CHECK_ALREADY_ENCODED("&quot;")
            _COPY_CONST_STR("&amp;")
            break;
         case '\'' :
            _COPY_CONST_STR("&apos;")
            break;
         case '"' :
            _COPY_CONST_STR("&quot;")
            break;
         default :
            *t++ = *s;
            break;
      }
      ++s;
      --len;
   }
   *t = '\0';
   return (int)(t-tt);
}

static int
sdf_xml_find_start_element (char *data, int *curr, int len, SDF_XML *xml, char *err)
{
   char *p;
   int i, ii, tag_close=0;
#define IS_CONST_STR(_curr,_str)   ( (len-(_curr)) >= (sizeof(_str)-1) && \
                                     memcmp (&data[(_curr)], (_str), sizeof(_str)-1) == 0 )
#define IS_BEGIN_ELEMENT1(_curr)	( (len-(_curr)) >= 2 && data[(_curr)] == '<' && data[(_curr)+1] != '/')
#define IS_BEGIN_ELEMENT2(_curr)	( (len-(_curr)) >= 2 && data[(_curr)] == '<' && data[(_curr)+1] == '/')
#define IS_END_ELEMENT1(_curr)		( data[(_curr)] == '>' )
#define IS_END_ELEMENT2(_curr)		( (len-(_curr)) >= 2 && data[(_curr)] == '/' && data[(_curr)+1] == '>')

   i = *curr;

   /* find first '<' */
   for (;;)
   {
      p = memchr (&data[i], '<', len-i);
      if (p == NULL)
         return SDF_XML_RET_EOF;
      i = p-data;
      if (!IS_CONST_STR(i,"<!--"))
         break;
      /* <!-- comment --> */
      for (;;)
      {
         p = memchr (&data[i], '>', len-i);
         if (p == NULL)
         {
            (void) sprintf (err, "Found start comment without ending");
            return SDF_XML_RET_ERROR;
         }
         i = p-data;
         ii = i-2;
         if (IS_CONST_STR(ii,"-->"))
         {
            ++i;
            break;
         }
		 ++i;
      }
   }
   
   if (IS_BEGIN_ELEMENT2(i)) tag_close = 1;

   /* element */
   ++i;
   xml->element = &data[i];
   while (i < len && !IS_SPACE(data[i]) && !IS_END_ELEMENT1(i) && !IS_END_ELEMENT2(i))
      ++i;
   if (i >= len)
   {
      (void) sprintf (err, "Invalid element field");
      return SDF_XML_RET_ERROR;
   }
   xml->element_len = &data[i] - xml->element;
   if (xml->element_len <= 0)
   {
      (void) sprintf (err, "Invalid element structure");
      return SDF_XML_RET_ERROR;
   }

   /* attribute */
   while (i < len && IS_SPACE(data[i]))
      ++i;
   xml->attr = &data[i];
   ii = 0;
   while (i < len)
   {
      if (IS_END_ELEMENT1(i))
      {
         ii = 1;
         xml->element_close = 0;
         break;
      }
      if (IS_END_ELEMENT2(i))
      {
         xml->element_close = 1;
         xml->data_len = 0;
         ii = 2;
         break;
      }
      ++i;
   }
   if (i >= len)
   {
      (void) sprintf (err, "Invalid attribute field");
      return SDF_XML_RET_ERROR;
   }
   xml->attr_len = &data[i] - xml->attr;
   i += ii;

   /* content */
   if ((tag_close == 0)&&(xml->element_close == 0))
   {
	   xml->content = &data[i];
	   while (i < len)
	   {
		  if (IS_BEGIN_ELEMENT1(i))
		  {
			 // found nested loop imply no data
		     xml->content = NULL;
		     xml->content_len = 0;
			 break;
		  }
		  if (IS_BEGIN_ELEMENT2(i))
		  {
			 xml->content_len = &data[i] - xml->content;
			 break;
		  }

		  ++i;
	   }
	   if (i >= len)
	   {
		  (void) sprintf (err, "Invalid content field");
		  return SDF_XML_RET_ERROR;
	   }
   }
   else
   {
	   xml->content = NULL;
	   xml->content_len = 0;
   }
   
   xml->data = &data[i];

   *curr = i;
   return SDF_XML_RET_OK;
}


static int
sdf_xml_find_end_element (char *data, int *curr, int len, SDF_XML *xml, char *err)
{
   int i, r;
   SDF_XML x;

   i = *curr;
   for (;;)
   {
      r = sdf_xml_find_start_element (data, &i, len, &x, err);
      if (r < 0)
         return r;
      if (r)
      {
         (void) sprintf (err, "Found element without ending");
         return SDF_XML_RET_ERROR;
      }
      if (x.element_close)
         continue;
      if (x.element[0] == '/')
      {
         if ((x.element_len-1) == xml->element_len && memcmp (x.element+1, xml->element, xml->element_len) == 0)
            break;
         (void) sprintf (err, "Found extra element ending");
         return SDF_XML_RET_ERROR;
      }
      r = sdf_xml_find_end_element (data, &i, len, &x, err);
      if (r < 0)
         return r;
   }
   xml->data_len = (x.element-1) - xml->data;
   *curr = i;
   return SDF_XML_RET_OK;
}

int
SDF_xml_find_start_element (char *data, int *curr, int len, SDF_XML *xml, char *err)
{
   int r;

   for (;;)
   {
      r = sdf_xml_find_start_element (data, curr, len, xml, err);
      if (r != 0)
         return r;
      if (xml->element_len == (sizeof("?xml")-1) && memcmp (xml->element, "?xml", sizeof("?xml")-1) == 0)
         continue;
      if (xml->element_len == (sizeof("!DOCTYPE")-1) && memcmp (xml->element, "!DOCTYPE", sizeof("!DOCTYPE")-1) == 0)
         continue;
      if (xml->element_close)
         return 0;
      break;
   }
   return r;
}

int
SDF_xml_find_element (char *data, int *curr, int len, SDF_XML *xml, char *err)
{
   int r;

   for (;;)
   {
      r = sdf_xml_find_start_element (data, curr, len, xml, err);
      if (r != 0)
         return r;
      if (xml->element_len == (sizeof("?xml")-1) && memcmp (xml->element, "?xml", sizeof("?xml")-1) == 0)
         continue;
      if (xml->element_len == (sizeof("!DOCTYPE")-1) && memcmp (xml->element, "!DOCTYPE", sizeof("!DOCTYPE")-1) == 0)
         continue;
      if (xml->element_close)
         return 0;
      break;
   }
   return sdf_xml_find_end_element (data, curr, len, xml, err);
}

static int
sdf_xml_find_end_element_by_name (char *name, char *data, int *curr, int len, SDF_XML *xml, char *err)
{
   char *p;
   int i, tag_close=0;
#define IS_CONST_STR1(_curr,_str)   ( (len-(_curr)) >= (strlen(_str)-1) && \
                                     memcmp (&data[(_curr)], (_str), strlen(_str)) == 0 )
   char close_tag[50];
   i = *curr;

   /* find end '/>' or nested end '</tag>' */
   for (;;)
   {
      p = memchr (&data[i], '>', len-i);
      if (p == NULL)
	  {
		(void) sprintf (err, "Can not find ending tag");
	    return SDF_XML_RET_ERROR;
	  }
      i = p-data;
      if (tag_close == 0 && IS_CONST_STR1(i-1,"/>"))
	  {
	     i += strlen("/>");
         break;
	  }
	  tag_close = 1;
	  memset(close_tag, 0, sizeof(close_tag));
	  sprintf(close_tag, "</%s>", name);
	  p = memchr (&data[i], '<', len-i);
      if (p == NULL)
	  {
		 (void) sprintf (err, "Can not find nested ending tag");
		 return SDF_XML_RET_ERROR;
	  }
      i = p-data;
      if (IS_CONST_STR1(i,close_tag))
	  {
	     i += strlen(close_tag);
         break;
	  }
	  ++i;
   }
   
   *curr = i;
   return SDF_XML_RET_OK;
}

int
SDF_xml_find_element_by_name (char *name, char *data, int *curr, int len, SDF_XML *xml, char *err)
{
   int r;

   for (;;)
   {
      r = sdf_xml_find_start_element (data, curr, len, xml, err);
      if (r != 0)
         return r;
      if (xml->element_len == (sizeof("?xml")-1) && memcmp (xml->element, "?xml", sizeof("?xml")-1) == 0)
         continue;
	  if (strncmp(xml->element, name, strlen(name))==0)
		 break;
   }

   *curr = (xml->attr - data) + xml->attr_len - 1; // adjust data for check ending symbol
   return sdf_xml_find_end_element_by_name (name, data, curr, len, xml, err);
}

int
SDF_xml_find_attribute (char *data, int *curr, int len, char **name, int *nlen, char **value, int *vlen, char *err)
{
   int i, q;

   i = *curr;

   while (i < len && IS_SPACE(data[i]))
      ++i;
   if (i >= len)
      return SDF_XML_RET_EOF;

   *name = &data[i];
   while (i < len && !IS_SPACE(data[i]) && data[i] != '=')
      ++i;
   *nlen = &data[i] - *name;
   while (i < len && IS_SPACE(data[i]))
      ++i;
   if (i >= len || *nlen <= 0 || data[i] != '=')
   {
      (void) sprintf (err, "Invalid attribute");
      return SDF_XML_RET_ERROR;
   }

   ++i;   /* '=' */

   while (i < len && IS_SPACE(data[i]))
      ++i;

   if (i >= len || !(data[i] == '\'' || data[i] == '"'))
   {
      (void) sprintf (err, "Invalid attribute");
      return SDF_XML_RET_ERROR;
   }
   q = data[i];
   ++i;
   *value = &data[i];
   while (i < len && data[i] != q)
      ++i;
   if (i >= len)
   {
      (void) sprintf (err, "Invalid attribute");
      return SDF_XML_RET_ERROR;
   }
   *vlen = &data[i] - *value;
   ++i;
   if (i < len && !IS_SPACE(data[i]))
   {
      (void) sprintf (err, "Invalid attribute");
      return SDF_XML_RET_ERROR;
   }
   *curr = i;
   return SDF_XML_RET_OK;
}


