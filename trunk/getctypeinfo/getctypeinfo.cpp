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

/**
 *  @file
 *
 *  This program computes the size, min, and max of several commonly
 *  used C scalar types.
 */

#define COL_1_LEN  20
#define COL_2_LEN  4
#define COL_3_LEN  20
#define COL_4_LEN  20

#include "c++/CppStringHelp.h"

#include <iostream>

#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <sys/types.h>

/// Get the max value for the signed integral type.
template <class Type> Type getSignedIntegralTypeMax(){
  size_t bits = sizeof(Type)*8 - 2;
  Type half;
  for(half = 2; bits > 1; --bits){ half = half*2; }
  return (half-(Type)1) + half;
}

/// Get the min value for the signed integral type.
template <class Type> Type getSignedIntegralTypeMin(){
  size_t bits = sizeof(Type)*8 - 2;
  Type half;
  for(half = 2; bits > 1; --bits){ half = half*2; }
  return 0-half-half;
}

/// Get the max value for the unsigned integral type. 
template <class Type> Type getUnsignedIntegralTypeMax(){
  size_t bits = sizeof(Type)*8 - 1;
  Type half;
  for(half = 2; bits > 1; --bits){ half = half*2; }
  return (half-(Type)1) + half;
}

/// Get the min value for the unsigned integral type.
template <class Type> Type getUnsignedIntegralTypeMin(){ return 0; }

/// Print information for an integral type.
template <class Type> void printIntegralType(std::string typeName,
					     bool isSigned){
  size_t size;
  std::cout << typeName;
  size = typeName.size();
  if(size < COL_1_LEN){ std::cout << std::string(COL_1_LEN - size, ' '); }
  std::cout << " ";
 
  std::string str = silt::int2str<size_t>(sizeof(Type)*8);
  size = str.size();
  if(size < COL_2_LEN){ std::cout << std::string(COL_2_LEN - size, ' '); }
  std::cout << str << " ";

  str = silt::int2str<Type>(isSigned ? getSignedIntegralTypeMin<Type>() :
			    getUnsignedIntegralTypeMin<Type>());
  size = str.size();
  if(size < COL_3_LEN){ std::cout << std::string(COL_3_LEN - size, ' '); }
  std::cout << str << " ";

  str = silt::int2str<Type>(isSigned ? getSignedIntegralTypeMax<Type>() :
			    getUnsignedIntegralTypeMax<Type>());
  size = str.size();
  if(size < COL_4_LEN){ std::cout << std::string(COL_4_LEN - size, ' '); }
  std::cout << str << "\n";
}

/// Print information for the char type.
void printCharType(std::string typeName, bool isSigned){
  size_t size;
  std::cout << typeName;
  size = typeName.size();
  if(size < COL_1_LEN){ std::cout << std::string(COL_1_LEN - size, ' '); }
  std::cout << " ";

  std::string str = silt::int2str<size_t>(sizeof(char)*8);
  size = str.size();
  if(size < COL_2_LEN){ std::cout << std::string(COL_2_LEN - size, ' '); }
  std::cout << str << " ";

  str = silt::int2str<int>
    (isSigned ? (int)getSignedIntegralTypeMin<char>() :
     (int)getUnsignedIntegralTypeMin<unsigned char>());
  size = str.size();
  if(size < COL_3_LEN){ std::cout << std::string(COL_3_LEN - size, ' '); }
  std::cout << str << " ";

  str = silt::int2str<int>
    (isSigned ? (int)getSignedIntegralTypeMax<char>() :
     (int)getUnsignedIntegralTypeMax<unsigned char>());
  size = str.size();
  if(size < COL_4_LEN){ std::cout << std::string(COL_4_LEN - size, ' '); }
  std::cout << str << "\n";
}

/// Print information for the void* type.
void printVoidPtrType(){
  int numOfHex = sizeof(void*)*2;
  size_t size;

  std::cout << "void*";
  size = strlen("void*");
  if(size < COL_1_LEN){ std::cout << std::string(COL_1_LEN - size, ' '); }
  std::cout << " ";

  std::string str = silt::int2str<size_t>(sizeof(void*)*8);
  size = str.size();
  if(size < COL_2_LEN){ std::cout << std::string(COL_2_LEN - size, ' '); }
  std::cout << str << " ";

  str = "0x";
  for(int i=0; i < numOfHex; ++i){ str += "0"; }
  size = str.size();
  if(size < COL_3_LEN){ std::cout << std::string(COL_3_LEN - size, ' '); }
  std::cout << str << " ";

  str = "0x";
  for(int i=0; i < numOfHex; ++i){ str += "f"; }
  size = str.size();
  if(size < COL_4_LEN){ std::cout << std::string(COL_4_LEN - size, ' '); }
  std::cout << str << "\n";
}

/// Print information for the float type.
template <class Type> void printFloatingPointType(std::string typeName,
						  Type min, Type max){
  std::string str = typeName;
  std::cout << str;
  size_t size = str.size();
  if(size < COL_1_LEN){ std::cout << std::string(COL_1_LEN - size, ' '); }
  std::cout << " ";

  str = silt::int2str<size_t>(sizeof(Type)*8);
  size = str.size();
  if(size < COL_2_LEN){ std::cout << std::string(COL_2_LEN - size, ' '); }
  std::cout << str << " ";

  str = silt::fp2str<Type>(min);
  size = str.size();
  if(size < COL_3_LEN){ std::cout << std::string(COL_3_LEN - size, ' '); }
  std::cout << str << " ";

  str = silt::fp2str<Type>(max);
  size = str.size();
  if(size < COL_4_LEN){ std::cout << std::string(COL_4_LEN - size, ' '); }
  std::cout << str << "\n";
}

/// Print out information for several types.
int main(int argc, char** argv){
  std::string str = "type";
  std::cout << str << std::string(COL_1_LEN - str.size() + 1, ' ');
  str = "bits";
  std::cout << str << std::string(COL_2_LEN - str.size() + 1, ' ');
  str = "min";
  std::cout << str << std::string(COL_3_LEN - str.size() + 1, ' ');
  str = "max";
  std::cout << str << "\n";

  printCharType("char", true);
  printCharType("unsigned char", false);
  printIntegralType<short>("short", true);
  printIntegralType<unsigned short>("unsigned short", false);
  printIntegralType<int>("int", true);
  printIntegralType<unsigned>("unsigned", false);
  printIntegralType<long>("long", true);
  printIntegralType<unsigned long>("unsigned long", false);
  printIntegralType<long long>("long long", true);
  printIntegralType<unsigned long long>("unsigned long long", false);
  printFloatingPointType<float>("float", FLT_MIN, FLT_MAX);
  printFloatingPointType<double>("double", DBL_MIN, DBL_MAX);
  printFloatingPointType<long double>("long double", LDBL_MIN, LDBL_MAX);
  printVoidPtrType();
  printIntegralType<size_t>("size_t", false);
  printIntegralType<ssize_t>("ssize_t", true);
  printIntegralType<ptrdiff_t>("ptrdiff_t", true);

  return 0;
}
