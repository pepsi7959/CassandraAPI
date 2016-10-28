
/*
   Copyright (C) 2014, MIMO Tech Co., Ltd.

   sdfconfig.h

   Revision history

   1.0.0   Initialize.

*/

#ifndef _SDFCONFIG_H_
#define _SDFCONFIG_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "sdflib.h"
#include "sdfxml.h"


enum SDF_CONFIG_RET_CODE
{
   SDF_CONFIG_RET_ERROR = -1000,
   SDF_CONFIG_RET_OK = 0,
   SDF_CONFIG_RET_EOF,
};


typedef struct _sdf_config_mem_
{
   char *data;
   int curr;
   int len;
} SDF_CONFIG_MEM;


SDF_CONFIG_MEM * SDF_config_load_file (char *file);
void SDF_config_free (SDF_CONFIG_MEM *mem);
int SDF_config_find_element (SDF_CONFIG_MEM *scope, char *element, SDF_CONFIG_MEM *omem, SDF_XML *oxml, int mandatory);
int SDF_config_find_attr (SDF_XML *xml, char *name, char *value, int vsize, int mandatory);
int SDF_config_find_element_attr (SDF_CONFIG_MEM *scope, char *element, char *name, char *value, int vsize, int mandatory);


#ifdef __cplusplus
}
#endif

#endif
