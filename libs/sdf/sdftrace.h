
/*

   sftrace.h

   Revision history

   1.0.0   Initialize.

*/

#ifndef _SFTRACE_H_
#define _SFTRACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SDFTRACE_MAX_NUMBER_LIST			10

enum SDFTRACE_RET
{
   SDFTRACE_RET_ERR_OPTION,
   SDFTRACE_RET_SUCCESS = 0,
};

enum SDFTRACE_OPTION
{
   SDFTRACE_OPTION_NONE = 0,
   SDFTRACE_OPTION_SET,
   SDFTRACE_OPTION_REMOVE,
   SDFTRACE_OPTION_RESET
};

enum SDFTRACE_TYPE
{
   SDFTRACE_NONE = -1,
   SDFTRACE_END_USER_E164 = 0,
   SDFTRACE_END_USER_IMSI,
   SDFTRACE_END_USER_SIP_URI,
   SDFTRACE_END_USER_NAI
};

enum SDFTRACE_DIRECTION
{
   SDFTRACE_INPUT = 0,
   SDFTRACE_OUTPUT
};

void SDF_trace_init (char *name, char *dir);
int SDF_trace_set_option (int option, int type, char *number, char *err);
void SDF_trace_message (int direction, const char *msg);


#ifdef __cplusplus
}
#endif

#endif

