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

#ifndef _CPPSTRINGHELP_H_
#define _CPPSTRINGHELP_H_

/**
 *  @file
 *
 *  This file provides help generating and manipulating c++ strings.
 */

#include "c++/CppErrorHandling.h"
#include "CommonStringHelp.h"

#include <string>
#include <sstream>
#include <iostream>
#include <math.h>

namespace silt{
  /// Converts a bool to a string of "1" or "0".
  static inline std::string zeroOrOne(bool expr){ return (expr ? "1" : "0"); }


  /// Converts a string to an integer.
  template<class Type> Type str2int(std::string s){
    Type v = 0;
    std::istringstream iss(s);
    iss >> v;
    myassert(iss, "Invalid string to int conversion for: " << s);
    return v;
  }

  /// Converts a string to a floating point type.
  template<class Type> Type str2fp(std::string s){
    if(s == NAN_STR){ return (Type)NAN; }
    Type v = 0;
    std::istringstream iss(s);
    iss >> v;
    myassert(iss, "Invalid string to fp conversion for: " << s);
    myassert(!isnan(v), "String converts to a nan.");
    return v;
  }

  /// Converts an integer to a string
  template<class Type> std::string int2str(Type n){
    std::ostringstream oss;
    oss << n;
    return oss.str();
  }

  /// Converts a floating point value to a string.
  template<class Type> std::string fp2str(Type n){
    if(isnan(n)){ return NAN_STR; }
    std::ostringstream oss;
    oss << n;
    return oss.str();
  }

  /// Returns true if the prefix of str2test matches prefix2match
  static inline bool doesStrsPrefixMatch(std::string str2test,
					 std::string prefix2match){
    return str2test.compare(0, prefix2match.size(), prefix2match) == 0;
  }

  /**
   *  If prefix2match is a prefix of str2test, then the remainder of
   *  str2test (after the prefix) is returned.
   */
  static inline std::string getRemainderAfterPrefix(std::string str2test,
						    std::string prefix2match){
    if(!doesStrsPrefixMatch(str2test, prefix2match)){ return ""; }
    return str2test.substr(prefix2match.length());
  }

  /// Removes the whitespace ('\t', ' ', '\n') from the beginning and
  /// the end of s.
  static inline std::string trimWhitespace(std::string s){
    std::string::size_type i;
    for(i = 0; i < s.size() &&
	  (s[i] == ' ' || s[i] == '\n' || s[i] == '\t'); ++i){}
    s.erase(0, i);
    std::string::size_type indexPlus1;
    for(indexPlus1 = s.size(); indexPlus1 > 0 &&
	  (s[indexPlus1-1] == ' ' || s[indexPlus1-1] == '\n' ||
	   s[indexPlus1-1] == '\t'); --indexPlus1){}
    s.erase(indexPlus1);
    return s;
  }

  /// Removes all whitespace ('\t', ' ', '\n') from s.
  static inline std::string removeAllWhitespace(std::string s){
    std::string rv = "";
    std::string::size_type i;
    for(i = 0; i < s.size(); ++i){
      if(s[i] != ' ' && s[i] != '\t' && s[i] != '\n'){ rv = rv + s[i]; }
    }
    return rv;
  }
}

#endif // _CPPSTRINGHELP_H_
