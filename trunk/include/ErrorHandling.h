#ifndef _ERRORHANDLING_H_
#define _ERRORHANDLING_H_

// Copyright (c) 2011, Ryan M. Lefever
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
