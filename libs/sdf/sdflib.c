
/*
   Copyright (C) 2014, MIMO Tech Co., Ltd.

   sdflib.c

   Revision history

   1.0.0   Initialize.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(WIN32)

#include <windows.h>

#else

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

#endif

#include "sdflog.h"


#if defined(MEM_DEBUG)

static unsigned long mem_alloc_size;
static unsigned long mem_alloc_count;
static unsigned long mem_free_count;

static struct _mem_debug_info_ *mem_alloc_list;

#define SDF_MALLOC_LIMIT   (8*1024*1024)

/*
#define LIST_APPEND(_first,_item)                           \
{                                                           \
   if ((_first) == NULL)                                    \
   {                                                        \
      (_first) = (_item)->prev = (_item)->next = (_item);   \
   }                                                        \
   else                                                     \
   {                                                        \
      (_item)->prev = (_first)->prev;                       \
      (_item)->next = (_first);                             \
      (_first)->prev->next = (_item);                       \
      (_first)->prev = (_item);                             \
   }                                                        \
}
#define LIST_REMOVE(_first,_item)                           \
{                                                           \
   if ((_first) == (_item))                                 \
   {                                                        \
      if ((_first)->next == (_item))                        \
         (_first) = NULL;                                   \
      else                                                  \
      {                                                     \
         (_first) = (_item)->next;                          \
         (_item)->next->prev = (_item)->prev;               \
         (_item)->prev->next = (_item)->next;               \
      }                                                     \
   }                                                        \
   else                                                     \
   {                                                        \
      (_item)->next->prev = (_item)->prev;                  \
      (_item)->prev->next = (_item)->next;                  \
   }                                                        \
   (_item)->prev = (_item)->next = NULL;                    \
}*/

struct _mem_debug_info_
{
   int size;
   char *file;
   int line;
   struct _mem_debug_info_ *prev;
   struct _mem_debug_info_ *next;
};

typedef union _align_unit_
{
   struct _mem_debug_info_ x;
   double dd;
   void *pp;
   long ll;
   char cc;
} ALIGN_UNIT;


void *
_sdf_malloc (int size, char *file, int line)
{
   static char func[] = "_sdf_malloc";
   ALIGN_UNIT *p;

   size += sizeof(ALIGN_UNIT);
   if (size > SDF_MALLOC_LIMIT)
      SDFLOG_WARN((B,"allocated size (%u) is too big from line (%d) of file [%s]",(unsigned int)size,line,file))
   p = malloc (size);
   if (p != NULL)
   {
	  (void) memset (p, 0, size);
      mem_alloc_size += size;
      ++mem_alloc_count;
      p->x.size = size;
      p->x.file = file;
      p->x.line = line;
      LINKEDLIST_APPEND(mem_alloc_list,&(p->x))
      ++p;
   }
   else
   {
      SDFLOG_ERROR((B,"Can not allocate memory size (%u) from line (%d) of file [%s]",(unsigned int)size,line,file))
      _sdf_show_mem_info ();
   }
   return p;
}


void *
_sdf_realloc (void *m, int size, char *file, int line)
{
   int n;
   void *p;

   p = _sdf_malloc (size, file, line);
   if (p != NULL)
   {
      ALIGN_UNIT *mm = (ALIGN_UNIT *)m;
      --mm;
      n = mm->x.size - sizeof(ALIGN_UNIT);
      if (n > size)
         n = size;
      (void) memcpy (p, m, n);
      _sdf_free (m, file, line);
   }
   return p;
}


void
_sdf_free (void *p, char *file, int line)
{
   static char func[] = "_sdf_free";
   ALIGN_UNIT *pp = (ALIGN_UNIT *)p;
   --pp;
   LINKEDLIST_REMOVE(mem_alloc_list,&(pp->x))
   mem_alloc_size -= pp->x.size;
   if (pp->x.size > SDF_MALLOC_LIMIT)
      SDFLOG_WARN((B,"found allocated size (%u) is too big from line (%d) of file [%s]",(unsigned int)(pp->x.size),line,file))
   else if (pp->x.size <= 0)
      SDFLOG_FATAL((B,"Invalid free size (%d) from line (%d) of file [%s]",pp->x.size,line,file))
   pp->x.size = 0;
   ++mem_free_count;
   free (pp);
}


void
_sdf_show_mem_info ()
{
   static char func[] = "_sdf_show_mem_info";
   struct _mem_debug_info_ *p, *l;

   SDFLOG_INFO((B,"allocated size (%lu), allocted (%lu), free (%lu)",mem_alloc_size,mem_alloc_count,mem_free_count))
   if ((p = mem_alloc_list) != NULL)
   {
      l = p;
      for (;;)
      {
         SDFLOG_WARN((B,"allocate memory at line (%d) of file [%s]",p->line,p->file))
         p = p->next;
         if (p == l)
            break;
      }
   }
}

#endif

void * 
_malloc (int size) 
{
	void *p=NULL; 
	p=malloc(size); 
	(void)memset(p,0,size); 
	return p;
}

void
SDF_random (void *data, int len)
{
#define _FILL(_p,_l)                    \
{                                       \
register char *__p = (char *)(_p);   \
register int __l = (int)(_l);        \
while (__l > 0)                      \
{                                    \
if (i >= len)                     \
{                                 \
i = 0;                         \
++z;                           \
}                                 \
((char *)data)[i] ^= __p[0];      \
++i;                              \
++__p;                            \
--__l;                            \
}                                    \
}
#if defined(WIN32)
    int i, z;
    struct
    {
        LARGE_INTEGER perf;
        int pad;
        SYSTEMTIME ct;
        DWORD th;
    } gt;

    (void) memset (&gt, 0, sizeof(gt));
    i = z = 0;
    for (;;)
    {
        (void) QueryPerformanceCounter (&gt.perf);
        GetSystemTime (&gt.ct);
        gt.th = GetCurrentThreadId ();
        _FILL(&gt,sizeof(gt))
        if (z != 0)
            break;
        Sleep (0);
    }
#else
    int i, r, z;
    int fd;
    struct
    {
        clock_t wt;
        struct timezone tz;
        struct timeval tv;
        int pad;
        struct tms tms;
        int pid;
    } gt;

    fd = open ("/dev/urandom", O_RDONLY);
    if (fd >= 0)
    {
        i = 0;
        while (i < len)
        {
            r = (int) read (fd, &((char *)data)[i], len-i);
            if (r <= 0)
                break;
            i += r;
        }
        (void) close (fd);
    }
    i = z = 0;
    (void) memset (&gt, 0, sizeof(gt));
    for (;;)
    {
        (void) gettimeofday (&gt.tv, &gt.tz);
        gt.wt = times (&gt.tms);
        gt.pid = getpid ();
        _FILL(&gt,sizeof(gt))
        if (z != 0)
            break;
        sleep (0);
    }
#endif
}


