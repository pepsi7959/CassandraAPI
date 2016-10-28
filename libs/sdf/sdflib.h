
/*
   Copyright (C) 2014, MIMO Tech Co., Ltd.

   sdflib.h

   Revision history
   1.0.0   Initialize.

*/

#ifndef _SDFLIB_H_
#define _SDFLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>


#define SDF_ERR_BUFFER_SIZE         1024
#define SDF_HOST_NAME_SIZE          128

#define SDF_WATCHDOG_MIN_TIME       2


enum SDF_ELEMENT_RET_CODE
{
   SDF_ELEMENT_RET_ERROR = -1000,
   SDF_ELEMENT_RET_OK = 0,
   SDF_ELEMENT_RET_MORE_DATA,
   SDF_ELEMENT_RET_NO_DATA,
};


void SDF_random (void *data, int len);


#if defined(MEM_DEBUG)
#define SDF_MALLOC(_s)       _sdf_malloc((_s),__FILE__,__LINE__)
#define SDF_REALLOC(_p,_s)   _sfd_realloc((_p),(_s),__FILE__,__LINE__)
#define SDF_FREE(_p)         { _sdf_free((_p),__FILE__,__LINE__); _p=NULL; }
void * _sdf_malloc (int size, char *file, int line);
void * _sdf_realloc (void *m, int size, char *file, int line);
void _sdf_free (void *p, char *file, int line);
void _sdf_show_mem_info ();
#define SDF_SHOW_MEM         { _sf_show_mem_info (); }
#else
#define SDF_MALLOC(_s)       _malloc((_s))
#define SDF_REALLOC(_p,_s)   realloc((_p),(_s))
#define SDF_FREE(_p)         { free((_p)); _p=NULL; }
void * _malloc (int size);
#define SDF_SHOW_MEM
#endif


#ifdef __cplusplus
}
#endif

#endif

