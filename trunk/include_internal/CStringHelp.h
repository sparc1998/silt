#ifndef _CSTRINGHELP_H_
#define _CSTRINGHELP_H_

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

// DESCRIPTION: Defines useful functions to operate on C strings. No C
//              files are required to use the contents of this file.

#include "ErrorHandling.h"

#define UINT64_T_MAX_DIGITS 20

/**
 *  This function returns a pointer to the string that immediately
 *  follows prefix in teststr.  If teststr does not begin with prefix,
 *  then NULL is returned.
 */
static inline char* getPtr2StrAfterPrefix(char* teststr, const char* prefix){
  myassert(teststr && prefix, 0, "teststr or prefix is NULL.");
  unsigned i;
  for(i=0; teststr[i] != '\0' && prefix[i] != '\0' && teststr[i] == prefix[i];
      ++i){}
  if(prefix[i] == '\0'){
    return &(teststr[i]);
  }
  return NULL;
}

/**
 *  Convert a string to an unsigned 64-bit integer.
 */
static inline uint64_t strtou64(char* s){
  uint64_t v = 0;
  while(isdigit(*s)){
    v = v*10 + (*s - '0');
    ++s;
  }
  return v;
}

#endif // _CSTRINGHELP_H_
