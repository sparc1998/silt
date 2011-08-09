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

#ifndef _TIMEHELP_H_
#define _TIMEHELP_H_

/**
 *  @file
 *
 *  This file defines common functions to help with time based
 *  functions. Use of the functions in this file does not require the
 *  use of an c files.
 */

#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// Constants
#define USECS_IN_1_SEC                            1000000
#define NSECS_IN_1_SEC                            1000000000
#define MSECS_IN_1_SEC                            1000
#define NSECS_IN_1_MSEC                           1000000
#define USECS_IN_1_MSEC                           1000
#define HZ_IN_1_MHZ                               1000000
#define UNDEFINED_TU_STR                          "undefined"
#define CYCLES_STR                                "cycles"
#define USECS_STR                                 "usecs"
#define NSECS_STR                                 "nsecs"
#define SECS_STR                                  "secs"

enum TimeUnits{
  UNDEFINED_TIME_UNITS = 0,
  CYCLES,
  USECS,
  NSECS,
  SECS
};

/// Convert time units to a string.
static inline const char* timeUnits2str(enum TimeUnits tu){
  switch(tu){
  case UNDEFINED_TIME_UNITS: return UNDEFINED_TU_STR; break;
  case CYCLES:               return CYCLES_STR;       break;
  case USECS:                return USECS_STR;        break;
  case NSECS:                return NSECS_STR;        break;
  case SECS:                 return SECS_STR;         break;
  default: myerror("Illegal time unit.");
  }
}

/// Convert a string to time units.
static inline enum TimeUnits str2timeUnits(const char* s){
  if(strcmp(s, UNDEFINED_TU_STR) == 0){ return UNDEFINED_TIME_UNITS; }
  else if(strcmp(s, "") == 0){          return UNDEFINED_TIME_UNITS; }
  else if(strcmp(s, CYCLES_STR) == 0){  return CYCLES; }
  else if(strcmp(s, USECS_STR) == 0){   return USECS; }
  else if(strcmp(s, NSECS_STR) == 0){   return NSECS; }
  else if(strcmp(s, SECS_STR) == 0){    return SECS; }
  else{ myerror("Illegal time units string."); }
}

/// Converts a time value to msecs.
static inline double time2msecs(uint64_t value, enum TimeUnits tu,
				float cpuMhz){
  switch(tu){
  case CYCLES:
    return (((double)value)*((double)MSECS_IN_1_SEC))/
      (((double)cpuMhz)*((double)(HZ_IN_1_MHZ)));
    break;
  case USECS:
    return ((double)value)/((double)USECS_IN_1_MSEC);
    break;
  case NSECS:
    return ((double)value)/((double)NSECS_IN_1_MSEC);
    break;
  case SECS:
    return ((double)value)*((double)MSECS_IN_1_SEC);
    break;
  default:
    myerror("Illegal time unit.");
  }
}

/// Converts a struct timeval to usecs.
static inline uint64_t structTimeval2usecs(struct timeval t){
  return (uint64_t)t.tv_sec * (uint64_t)USECS_IN_1_SEC + (uint64_t)t.tv_usec;
}

/// Converts a struct timespec to nsecs.
static inline uint64_t structTimespec2nsecs(struct timespec t){
  return (uint64_t)t.tv_sec * (uint64_t)NSECS_IN_1_SEC + (uint64_t)t.tv_nsec;
}

#endif // _TIMEHELP_H_
