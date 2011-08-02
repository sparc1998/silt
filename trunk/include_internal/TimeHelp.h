#ifndef _TIMEHELP_H_
#define _TIMEHELP_H_

#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <string.h>

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

static inline enum TimeUnits str2timeUnits(const char* s){
  if(strcmp(s, UNDEFINED_TU_STR) == 0){ return UNDEFINED_TIME_UNITS; }
  else if(strcmp(s, "") == 0){          return UNDEFINED_TIME_UNITS; }
  else if(strcmp(s, CYCLES_STR) == 0){  return CYCLES; }
  else if(strcmp(s, USECS_STR) == 0){   return USECS; }
  else if(strcmp(s, NSECS_STR) == 0){   return NSECS; }
  else if(strcmp(s, SECS_STR) == 0){    return SECS; }
  else{ myerror("Illegal time units string."); }
}

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

/**
 *  Converts a struct timeval to SimpleTime
 */
static inline uint64_t structTimeval2usecs(struct timeval t){
  return (uint64_t)t.tv_sec * (uint64_t)USECS_IN_1_SEC + (uint64_t)t.tv_usec;
}

/**
 *  Converts a struct timespec to uint64_t.
 */
static inline uint64_t structTimespec2nsecs(struct timespec t){
  return (uint64_t)t.tv_sec * (uint64_t)NSECS_IN_1_SEC + (uint64_t)t.tv_nsec;
}

#endif // _TIMEHELP_H_
