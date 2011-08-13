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

  LineGraph lg;
  lg.setTitle ("Graph Title");
  lg.setIncludeLegend (true);
  lg.setAddTimestamp (true);
  lg.setDisplayGrid (true);
  lg.setXaxisTitle("XAXIS");
  lg.setYaxisTitle("YAXIS");
  lg.setXrange(0.0, 1.0);
  lg.setYrange(0, 100);
  lg.setXticInterval(.1);
  lg.setYticInterval(10);

  lg.nameDataSeries(0, "nails 1 2 3");
  lg.add(0, .1, 56);
  lg.add(0, .23, 56);
  lg.add(0, .4, 26);
  lg.add(0, .6, 23);
  lg.add(0, .8, 1);
  lg.add(0, .99, 88);
  lg.nameDataSeries(2, "screws");
  lg.add(2, .1111, 76);
  lg.add(2, .001, 88);
  lg.add(2, .6, 4);
  lg.nameDataSeries(1, "bolts");
  lg.add(1, 0.0, 0);
  lg.add(1, .2, 20);
  lg.add(1, .4, 40);
  lg.add(1, .6, 60);
  lg.add(1, .8, 80);
  lg.add(1, 1.0, 100);

  myassert(lg.generate(LineGraph::SGT_PNG, std::string(outputDir) + "/mygraph"),
	   "Failed to create graph.");

  return 0;
}
