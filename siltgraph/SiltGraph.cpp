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
#include "c++/CppStringHelp.h"
#include "CommonStringHelp.h"
#include "c++/SiltCppError.h"

#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace silt;

silt::SiltGraph::SiltGraph() : _title(""),
			       _addTimestamp(true),
			       _displayGrid(false) {}

/// Generates the graph in the desired output. True is returned if the
/// graph generation is successful.
bool silt::SiltGraph::generate(enum OutputType ot, std::string baseFilename){
  std::string gplotFilename = baseFilename + ".gplot";

  // Gnuplot file
  std::ofstream gplotFile(gplotFilename.c_str(),
			  std::ios::out | std::ios::trunc);
  if(!gplotFile.is_open()){
    mywarn(0, "Unable to open " << gplotFilename << ".");
    return false;
  }
  // Output file
  gplotFile << getOutputLine(ot, baseFilename);
  // Terminal type
  gplotFile << getTerminalLine(ot);
  // Title
  if(!_title.empty()){ gplotFile << "set title '" << _title << "'\n"; }
  // Data missing string
  gplotFile << "set datafile missing '" << NAN_STR << "'\n";
  // Timestamp
  if(_addTimestamp){ gplotFile << "set time\n"; }
  // Gridlines
  if(_displayGrid){ gplotFile << "set grid\n"; }
  // Child class output gnuplot directives
  if(!outputGplotDirectives(gplotFile, baseFilename)){ return false; }
  gplotFile.close();

  // Data files
  if(!outputDataFiles(ot, baseFilename)){ return false; }

  // Execute gnuplot command
  if(ot == SGT_PNG){
    std::string cmd = std::string("gnuplot ") + gplotFilename; 
    int systemRv = system(cmd.c_str());
    if(systemRv == -1){
      mywarn(0, std::string("Failed to execute: ") << cmd);
      return false;
    }
    else if(systemRv != 0){
      mywarn(0, std::string("Non-zero exit value when running: ") << cmd);
      return false;
    }
  }

  return true;
}

std::string silt::SiltGraph::getOutputLine(enum OutputType ot,
					   std::string baseFilename){
  switch(ot){
  case SGT_GNUPLOT: return "";
  case SGT_PNG: return std::string("set output '") + baseFilename + ".png'\n";
  }
  return "";
}

std::string silt::SiltGraph::getTerminalLine(enum OutputType ot){
  switch(ot){
  case SGT_GNUPLOT: return "";
  case SGT_PNG: return "set terminal png\n";
  }
  return "";
}

std::string silt::LineGraph::DataSeries::str(void){
  return std::string("name=") + name + " datasize=" +
    silt::int2str<DataVector::size_type>(data.size());
}

silt::LineGraph::LineGraph() : SiltGraph(),
			       _includeLegend(true),
			       _xaxisTitle(""),
			       _yaxisTitle(""),
			       _xmin((double)NAN),
			       _xmax((double)NAN),
			       _ymin((double)NAN),
			       _ymax((double)NAN),
			       _xInterval((double)NAN),
			       _yInterval((double)NAN),
			       _data() {}

silt::LineGraph::~LineGraph(){
  DataCollectionIt it, endit;
  for(it = _data.begin(), endit = _data.end(); it != endit; ++it){
    DataSeries* ds = *it;
    if(ds){ delete ds; }
  }
}

void silt::LineGraph::nameDataSeries(unsigned dataSeriesIndex,
				     std::string name){
  getDataSeries(dataSeriesIndex)->name = name;
}

/// Add a data value to the graph.
void silt::LineGraph::add(unsigned dataSeriesIndex, double x, double y){
  getDataSeries(dataSeriesIndex)->data.push_back(DataValue(x,y));
}

void silt::LineGraph::add(unsigned dataSeriesIndex, DataValue& v){
  add(dataSeriesIndex, v.x, v.y);
}

silt::LineGraph::DataSeries*
silt::LineGraph::getDataSeries(unsigned dataSeriesIndex){
  if(dataSeriesIndex >= _data.size()){
    _data.insert(_data.end(), (dataSeriesIndex - _data.size() + 1), NULL);
  }
  DataSeries* ds = _data[dataSeriesIndex];
  if(!ds){
    ds = new DataSeries;
    ds->name = std::string("Series ") + int2str<unsigned>(dataSeriesIndex);
    _data[dataSeriesIndex] = ds;
  }
  return ds;
}

bool silt::LineGraph::outputGplotDirectives(std::ostream& o,
					    std::string baseFilename){
  // Legend
  if(_includeLegend){ o << "set key outside\n"; }
  else{ o << "set key off\n"; }
  // x-axis label
  if(!_xaxisTitle.empty()){ o << "set xlabel '" << _xaxisTitle << "'\n"; }
  // y-axis label
  if(!_yaxisTitle.empty()){ o << "set ylabel '" << _yaxisTitle << "'\n"; }
  // x range
  if(!isnan(_xmin) && !isnan(_xmax)){
    o << "set xrange [" << _xmin << ":" << _xmax << "]\n";
  }
  // y range
  if(!isnan(_ymin) && !isnan(_ymax)){
    o << "set yrange [" << _ymin << ":" << _ymax << "]\n";
  }
  // x interval
  if(!isnan(_xInterval)){ o << "set xtics " << _xInterval << "\n"; }
  // y interval
  if(!isnan(_yInterval)){ o << "set ytics " << _yInterval << "\n"; }
  // plot command
  o << "plot";
  DataCollectionIt it, endit;
  unsigned datafileCount = 0;
  for(it = _data.begin(), endit = _data.end(); it != endit; ++it){
    DataSeries* ds = *it;
    if(ds && !ds->name.empty() && !ds->data.empty()){
      if(datafileCount != 0){ o << ","; }
      o << " '" << baseFilename << "-" << datafileCount << ".data' using "
	<< "1:2" << " title columnheader with linespoints";
      ++datafileCount;
    }
  }
  o << "\n";

  return true;
}

bool silt::LineGraph::outputDataFiles(enum OutputType ot,
				      std::string baseFilename){
  unsigned datafileCount = 0;
  DataCollectionIt dcIt, endDcIt;
  for(dcIt = _data.begin(), endDcIt = _data.end(); dcIt != endDcIt; ++dcIt){
    DataSeries* ds = *dcIt;
    if(ds && !ds->name.empty() && !ds->data.empty()){
      std::string dataFilename = baseFilename + "-" +
	silt::int2str<unsigned>(datafileCount) + ".data";
      // Open data file
      std::ofstream dataFile(dataFilename.c_str(),
			     std::ios::out | std::ios::trunc);
      if(!dataFile.is_open()){
	mywarn(0, "Unable to open " << dataFilename << ".");
	return false;
      }
      dataFile << "\"\"\t\"" << ds->name << "\"\n";
      DataVectorIt dvIt, endDvIt;
      for(dvIt = ds->data.begin(), endDvIt = ds->data.end();
	  dvIt != endDvIt; ++dvIt){
	DataValue v = *dvIt;
	// We use fp2str because it will convert NAN to a "nan" string
	dataFile << silt::fp2str<double>(v.x) << "\t"
		 << silt::fp2str<double>(v.y) << "\n";
      }
      // Close data file
      dataFile.close();

      ++datafileCount;
    }
  }

  return true;
}
