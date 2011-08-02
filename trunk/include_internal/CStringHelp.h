#ifndef _CSTRINGHELP_H_
#define _CSTRINGHELP_H_

#include "ErrorHandling.h"

#define UINT64_T_MAX_DIGITS 20

/**
 *  This function returns a pointer to the string that immediately
 *  follows prefix in testStr.  If testStr does not begin with prefix,
 *  then NULL is returned.
 */
static inline char* getPtr2StrAfterPrefix(char* testStr, const char* prefix){
  myassert(testStr && prefix, 0, "testStr or prefix is NULL.");
  unsigned i;
  for(i=0; testStr[i] != '\0' && prefix[i] != '\0' && testStr[i] == prefix[i];
      ++i){}
  if(prefix[i] == '\0'){
    return &(testStr[i]);
  }
  return NULL;
}

#endif // _CSTRINGHELP_H_
