#ifndef _ERRORHANDLING_H_
#define _ERRORHANDLING_H_

#include <stdio.h>

#define DEBUG_MSG  "D"
#define ERROR_MSG  "Error"
#define WARN_MSG   "Warning"

#define debugmsg(printFile, printFun, flush, msg, ...)			\
  fprintf(stderr, DEBUG_MSG);						\
  if(printFile){ fprintf(stderr, " " __FILE__ ":%d", __LINE__); }	\
  if(printFun){ fprintf(stderr, " %s",  __FUNCTION__); }		\
  fprintf(stderr, ": ");						\
  fprintf(stderr, msg, ## __VA_ARGS__);					\
  fprintf(stderr, "\n");						\
  if(flush){ fflush(stderr); }

#define myerror(msg, ...)						\
  fprintf(stderr, ERROR_MSG ": ");					\
  fprintf(stderr, msg, ## __VA_ARGS__);					\
  fprintf(stderr, "\n");						\
  abort();

#define mywarn(flush, msg, ...)						\
  fprintf(stderr, WARN_MSG ": ");					\
  fprintf(stderr, msg, ## __VA_ARGS__);					\
  fprintf(stderr, "\n");						\
  if(flush){ fflush(stderr); }

#define myassert(cond, printFile, msg, ...)				\
  if(!(cond)){								\
    fprintf(stderr, "Assert failure (%s)", #cond);			\
    if(printFile){ fprintf(stderr, " %s:%d", __FILE__, __LINE__); }	\
    fprintf(stderr, ": ");						\
    fprintf(stderr, msg, ## __VA_ARGS__);				\
    fprintf(stderr, "\n");						\
    abort();								\
  }

#endif // _ERRORHANDLING_H_
