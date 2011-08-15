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

#include "c++/SiltGraph.h"
#include "c++/SiltCppError.h"

using namespace silt;

int main(int argc, char** argv){
  myassert(argc == 2, "Wrong number of arguments.");
  char* outputDir = argv[1];

  ClusteredHistogram h;
  h.setTitle ("Graph Title");
  h.setUseTimestamp (true);
  h.setDisplayGrid (true);
  h.setXaxisTitle("XAXIS");
  h.setYaxisTitle("YAXIS");
  h.setYrange(0, 5);

  h.nameCluster(0, "c1");
  h.nameCluster(2, "c3");
  h.nameCluster(1, "c2");

  h.nameDataSeries(4, "ds5");
  h.nameDataSeries(3, "ds4");
  h.nameDataSeries(2, "ds3");
  h.nameDataSeries(1, "ds2");
  h.nameDataSeries(0, "ds1");

  h.add(2, 0, 1);
  h.add(2, 1, 2);
  h.add(2, 2, 1);
  h.add(2, 3, 2);
  h.add(2, 4, 1);

  h.add(0, 0, 1);
  h.add(0, 1, 2);
  h.add(0, 2, 3);
  h.add(0, 3, 4);
  h.add(0, 4, 5);

  h.add(1, 0, 5);
  h.add(1, 1, 4);
  h.add(1, 2, 3);
  h.add(1, 3, 2);
  h.add(1, 4, 1);


  myassert(h.generate(Histogram::SGT_PNG, std::string(outputDir) + "/mygraph"),
	   "Failed to create graph.");

  return 0;
}
