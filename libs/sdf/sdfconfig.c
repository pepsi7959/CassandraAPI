
/*
   Copyright (C) 2014, MIMO Tech Co., Ltd.

   sdfconfig.c

   Revision history

   1.0.0   Initialize.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "sdfconfig.h"
#include "sdflib.h"
#include "sdflog.h"
#include "sdfxml.h"


extern int errno;


SDF_CONFIG_MEM *
SDF_config_load_file (char *file)
{
   static char func[] = "SDF_config_load_file"; 
   int fd, r;
   struct flock lck;
   struct stat st;
   SDF_CONFIG_MEM *mem;

   fd = open (file, O_RDONLY);
   if (fd < 0)
   {
      SDFLOG_ERROR((B,"open [%s] return error [%s]",file,strerror (errno)))
      return NULL;
   }
   lck.l_type = F_RDLCK;
   lck.l_start = 0;
   lck.l_whence = SEEK_SET;
   lck.l_len = 0;
   if (fcntl (fd, F_SETLKW, &lck) < 0)
   {
      SDFLOG_ERROR((B,"fcntl return error [%s]",strerror (errno)))
      (void) close (fd);
      return NULL;
   }
   if (fstat (fd, &st) != 0)
   {
      SDFLOG_ERROR((B,"fstat return error [%s]",strerror (errno)))
      (void) close (fd);
      return NULL;
   }
   mem = SDF_MALLOC(sizeof(*mem)+(int)st.st_size+1);
   if (mem == NULL)
   {
      SDFLOG_ERROR((B,"Can not allocate memory size (%u)",(unsigned int)(sizeof(*mem)+st.st_size+1)))
      (void) close (fd);
      return NULL;
   }
   mem->data = (char *)mem;
   mem->data += sizeof(*mem);
   r = (int) read (fd, mem->data, st.st_size);
   if (r < 0)
   {
      SDFLOG_ERROR((B,"read return error [%s]",strerror (errno)))
      SDF_FREE(mem);
      (void) close (fd);
      return NULL;
   }
   if (r != st.st_size)
   {
      SDFLOG_ERROR((B,"read return (%d), expected (%d)",r,(int)st.st_size))
      SDF_FREE(mem);
      (void) close (fd);
      return NULL;
   }
   mem->data[r] = '\0';
   mem->len = r;
   mem->curr = 0;
   if (close (fd) != 0)
   {
      SDFLOG_ERROR((B,"close return error [%s]",strerror (errno)))
      SDF_FREE(mem);
      return NULL;
   }
   return mem;
}


void
SDF_config_free (SDF_CONFIG_MEM *mem)
{
   SDF_FREE(mem);
}


int
SDF_config_find_element (SDF_CONFIG_MEM *scope, char *element, SDF_CONFIG_MEM *omem, SDF_XML *oxml, int mandatory)
{
   static char func[] = "SDF_config_find_element";
   char err[SDF_ERR_BUFFER_SIZE]; 
   int n, r;
   SDF_XML xml;

   n = (int) strlen (element);
   for (;;)
   {
      r = SDF_xml_find_element (scope->data, &(scope->curr), scope->len, &xml, err);
      if (r < 0)
      {
         SDFLOG_ERROR((B,"SDF_xml_find_element return error [%s]",err))
         return SDF_CONFIG_RET_ERROR;
      }
      if (r)
      {
         if (mandatory)
         {
            SDFLOG_ERROR((B,"Can not find element <%s>",element))
            return SDF_CONFIG_RET_ERROR;
         }
         return SDF_CONFIG_RET_EOF;
      }
      if (n == xml.element_len && memcmp (xml.element, element, n) == 0)
         break;
   }
   if (omem != NULL)
   {
      omem->data = xml.data;
      omem->len = xml.data_len;
      omem->curr = 0;
   }
   if (oxml != NULL)
      *oxml = xml;
   return SDF_CONFIG_RET_OK;
}


int
SDF_config_find_attr (SDF_XML *xml, char *name, char *value, int vsize, int mandatory)
{
   static char func[] = "SDF_config_find_attr";
   char err[SDF_ERR_BUFFER_SIZE]; 
   int i, n, r;
   SDF_CONFIG_MEM nn, vv;

   n = (int) strlen (name);
   i = 0;
   for (;;)
   {
      r = SDF_xml_find_attribute (xml->attr, &i, xml->attr_len, &(nn.data), &(nn.len), &(vv.data), &(vv.len), err);
      if (r < 0)
      {
         SDFLOG_ERROR((B,"SDF_xml_find_attribute return error [%s]",err))
         return SDF_CONFIG_RET_ERROR;
      }
      if (r)
      {
         if (mandatory)
         {
            SDFLOG_ERROR((B,"Can not find attribute \"%s\"",name))
            return SDF_CONFIG_RET_ERROR;
         }
         return SDF_CONFIG_RET_EOF;
      }
      if (n == nn.len && memcmp (nn.data, name, n) == 0)
         break;
   }
   if (vsize <= vv.len)
   {
      SDFLOG_ERROR((B,"value buffer may not big enough"))
      return SDF_CONFIG_RET_ERROR;
   }
   SDF_xml_decode_data (vv.data, vv.len, value);
   return SDF_CONFIG_RET_OK;
}


int
SDF_config_find_element_attr (SDF_CONFIG_MEM *scope, char *element, char *name, char *value, int vsize, int mandatory)
{
   static char func[] = "SDF_config_find_element";
   char err[SDF_ERR_BUFFER_SIZE]; 
   int i, n, r;
   SDF_CONFIG_MEM nn, vv;
   SDF_XML xml;

   n = (int) strlen (element);
   for (;;)
   {
      r = SDF_xml_find_element (scope->data, &(scope->curr), scope->len, &xml, err);
      if (r < 0)
      {
         SDFLOG_ERROR((B,"SDF_xml_find_element return error [%s]",err))
         return SDF_CONFIG_RET_ERROR;
      }
      if (r)
      {
         if (mandatory)
         {
            SDFLOG_ERROR((B,"Can not find element <%s>",element))
            return SDF_CONFIG_RET_ERROR;
         }
         return SDF_CONFIG_RET_EOF;
      }
      if (n == xml.element_len && memcmp (xml.element, element, n) == 0)
         break;
   }
   n = (int) strlen (name);
   i = 0;
   for (;;)
   {
      r = SDF_xml_find_attribute (xml.attr, &i, xml.attr_len, &(nn.data), &(nn.len), &(vv.data), &(vv.len), err);
      if (r < 0)
      {
         SDFLOG_ERROR((B,"SDF_xml_find_attribute return error [%s]",err))
         return SDF_CONFIG_RET_ERROR;
      }
      if (r)
      {
         if (mandatory)
         {
            SDFLOG_ERROR((B,"Can not find attribute \"%s\" of element <%s>",name,element))
            return SDF_CONFIG_RET_ERROR;
         }
         return SDF_CONFIG_RET_EOF;
      }
      if (n == nn.len && memcmp (nn.data, name, n) == 0)
         break;
   }
   if (vsize <= vv.len)
   {
      SDFLOG_ERROR((B,"value buffer may not big enough"))
      return SDF_CONFIG_RET_ERROR;
   }
   SDF_xml_decode_data (vv.data, vv.len, value);
   return SDF_CONFIG_RET_OK;
}


