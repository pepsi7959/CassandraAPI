
/*
   Copyright (C) 2014, MIMO Tech Co., Ltd.

   sdflog.h

   Revision history

   1.0.0   Initialize.

*/

#ifndef _SDFLOG_H_
#define _SDFLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

//!--Use SDFLOG_SPLIT
#define SDFLOG_SPLIT		1

#define SDFLOG_MSG_SIZE   (32*1024)

enum SDFLOG_LEVEL
{
   SDFLOG_LEVEL_NONE = 0,
   SDFLOG_LEVEL_FATAL = 1,
   SDFLOG_LEVEL_ERROR = 2,
   SDFLOG_LEVEL_WARN = 4,
   SDFLOG_LEVEL_INFO = 8,
   SDFLOG_LEVEL_DEBUG = 16,
   SDFLOG_LEVEL_ALL = (SDFLOG_LEVEL_DEBUG<<1)-1,
};

#define _SDFLOG_(_lev,_FORMAT)                                 		\
{                                                             		\
   if( SDF_log_level () & _lev ) {							  		\
		char *B;                                    				\
		B = SDF_log_get_buffer ();              			  	      	\
		(void) sprintf _FORMAT;                                    	\
		SDF_log_write_string (_lev, __FILE__, func, __LINE__, B);   	\
   }														  		\
}

#define SDFLOG_FATAL(_FORMAT)   _SDFLOG_(SDFLOG_LEVEL_FATAL,_FORMAT)
#define SDFLOG_ERROR(_FORMAT)   _SDFLOG_(SDFLOG_LEVEL_ERROR,_FORMAT)
#define SDFLOG_WARN(_FORMAT)    _SDFLOG_(SDFLOG_LEVEL_WARN,_FORMAT)
#define SDFLOG_INFO(_FORMAT)    _SDFLOG_(SDFLOG_LEVEL_INFO,_FORMAT)
#define SDFLOG_DEBUG(_FORMAT)   _SDFLOG_(SDFLOG_LEVEL_DEBUG,_FORMAT)

#ifdef SDFLOG_SPLIT
void SDF_log_init (char *name, char *dir ,int interval);
#else
void SDF_log_init (char *name, char *dir);
#endif
int SDF_set_level_string (char *level);
int SDF_log_level ();
char* SDF_log_get_buffer ();
void SDF_log_write_string (int level, char *file, char *func, int line, char *msg);
char *level_to_string(int level);
void get_file_name(char* buff);
char* get_log_dir(void);
void get_file_message_name(char* buff);

#ifdef __cplusplus
}
#endif

#endif

