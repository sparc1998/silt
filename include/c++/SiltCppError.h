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

#ifndef _SILTCPPERROR_H_
#define _SILTCPPERROR_H_

/**
 *  @file
 *
 *  This file defines command error handling macros for use with C++
 *  programs. Use of this file does not require the use of any cpp
 *  files.
 */

#include "SiltCommonError.h"

#include <iostream>

#include <stdlib.h>

#define debugmsg(printFile, printFun, flush, msg)			\
  std::cerr << DEBUG_MSG;						\
  if(printFile){ std::cerr << " " __FILE__ ":" __LINE__; }		\
  if(printFun){ std::cerr << " " __FUNCTION__; }			\
  std::cerr << ": " << msg << "\n";					\
  if(flush){ std::cerr << std::flush; }

#define myerror(msg)							\
  std::cerr << ERROR_MSG << ": " << msg << "\n";			\
  abort();

#define mywarn(flush, msg)						\
  std::cerr << WARN_MSG << ": " << msg << "\n";				\
  if(flush){ std::cerr << std::flush; }

#define myassert(cond, msg)						\
  if(!(cond)){								\
    std::cerr << "Assert failure (" << #cond << "): " << msg << "\n";	\
    abort();								\
  }

namespace silt{
  template<class T1, class T2> T2 checked_ptr_cast(T1 t){
    T2 nt = dynamic_cast<T2>(t);
    myassert(nt, "Illegal cast.");
    return nt;
  }
}

#endif // _SILTCPPERROR_H_
