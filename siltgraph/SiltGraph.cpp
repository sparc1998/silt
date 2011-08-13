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

//////////////////////
///// Silt Graph /////
//////////////////////

silt::SiltGraph::SiltGraph() :
  _title(""), _timestamp(true), _grid(false), _includeLegend(true),
  _xaxisTitle(""), _yaxisTitle(""),
  _xmin((double)NAN), _xmax((double)NAN), _xInterval((double)NAN),
  _ymin((double)NAN), _ymax((double)NAN), _yInterval((double)NAN) {}

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
  if(_timestamp){ gplotFile << "set time\n"; }
  // Gridlines
  if(_grid){ gplotFile << "set grid\n"; }
  // Legend
  if(isLegendAvailable()){
    if(_includeLegend){ gplotFile << "set key outside\n"; }
    else{ gplotFile << "set key off\n"; }
  }
  else{ gplotFile << "set key off\n"; }
  // x-axis label
  if(!_xaxisTitle.empty()){
    gplotFile << "set xlabel '" << _xaxisTitle << "'\n";
  }
  // y-axis label
  if(!_yaxisTitle.empty()){
    gplotFile << "set ylabel '" << _yaxisTitle << "'\n";
  }
  if(isXAxisNumeric()){
    // x range
    if(!isnan(_xmin) && !isnan(_xmax)){
      gplotFile << "set xrange [" << _xmin << ":" << _xmax << "]\n";
    }
    // x interval
    if(!isnan(_xInterval)){ gplotFile << "set xtics " << _xInterval << "\n"; }
  }
  if(isYAxisNumeric()){
    // y range
    if(!isnan(_ymin) && !isnan(_ymax)){
      gplotFile << "set yrange [" << _ymin << ":" << _ymax << "]\n";
    }
    // y interval
    if(!isnan(_yInterval)){ gplotFile << "set ytics " << _yInterval << "\n"; }
  }
  // Child class gnuplot directives
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

void silt::SiltGraph::setIncludeLegend(bool val){
  myassert(isLegendAvailable(),
	   "Legends are not available for " << graphType());
  _includeLegend = val;
}

bool silt::SiltGraph::getIncludeLegend(void){
  myassert(isLegendAvailable(),
	   "Legends are not available for " << graphType());
  return _includeLegend;
}

void silt::SiltGraph::setXrange(double min, double max){
  myassert(isXAxisNumeric(), "The x axis is not numeric for " << graphType());
  _xmin = min; _xmax = max;
}

silt::SiltGraph::DoublePair silt::SiltGraph::getXrange(void){
  myassert(isXAxisNumeric(), "The x axis is not numeric for " << graphType());
  return DoublePair(_xmin, _xmax);
}

void silt::SiltGraph::setXticInterval(double interval){
  myassert(isXAxisNumeric(), "The x axis is not numeric for " << graphType());
  _xInterval = interval;
}

double silt::SiltGraph::getXticInterval(void){
  myassert(isXAxisNumeric(), "The x axis is not numeric for " << graphType());
  return _xInterval;
}

void silt::SiltGraph::setYrange(double min, double max){
  myassert(isYAxisNumeric(), "The y axis is not numeric for " << graphType());
  _ymin = min; _ymax = max;
}

silt::SiltGraph::DoublePair silt::SiltGraph::getYrange(void){
  myassert(isYAxisNumeric(), "The y axis is not numeric for " << graphType());
  return DoublePair(_ymin, _ymax);
}

void silt::SiltGraph::setYticInterval(double interval){
  myassert(isYAxisNumeric(), "The y axis is not numeric for " << graphType());
 _yInterval = interval;
}

double silt::SiltGraph::getYticInterval(void){
  myassert(isYAxisNumeric(), "The y axis is not numeric for " << graphType());
 return _yInterval;
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

//////////////////////
///// Line Graph /////
//////////////////////

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
  getDataSeries(dataSeriesIndex)->data.push_back(DoublePair(x,y));
}

void silt::LineGraph::add(unsigned dataSeriesIndex, DoublePair& v){
  add(dataSeriesIndex, v.v1, v.v2);
}

bool silt::LineGraph::outputGplotDirectives(std::ostream& o,
					    std::string baseFilename){
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
	DoublePair v = *dvIt;
	// We use fp2str because it will convert NAN to a "nan" string
	dataFile << silt::fp2str<double>(v.v1) << "\t"
		 << silt::fp2str<double>(v.v2) << "\n";
      }
      // Close data file
      dataFile.close();

      ++datafileCount;
    }
  }

  return true;
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

/////////////////////
///// Histogram /////
/////////////////////

void silt::Histogram::add(std::string name, double value){
  _data.push_back(DataValue(name, value));
}

bool silt::Histogram::outputGplotDirectives(std::ostream& o,
					    std::string baseFilename){
  o << "set style data histogram\n";
  o << "set style histogram cluster gap 1\n";
  o << "set style fill solid border -1\n";
  // plot command
  o << "plot '" << baseFilename << "-0.data' using 2:xtic(1) linecolor "
    << "rgbcolor \"#66CC66\"\n";

  return true;
}

bool silt::Histogram::outputDataFiles(enum OutputType ot,
				      std::string baseFilename){
  std::string dataFilename = baseFilename + "-0.data";
  // Open data file
  std::ofstream dataFile(dataFilename.c_str(),
			 std::ios::out | std::ios::trunc);
  if(!dataFile.is_open()){
    mywarn(0, "Unable to open " << dataFilename << ".");
    return false;
  }
  DataCollectionIt dcIt, endDcIt;
  for(dcIt = _data.begin(), endDcIt = _data.end(); dcIt != endDcIt; ++dcIt){
    DataValue& dv = *dcIt;
    dataFile << "\"" << dv.name << "\"\t" << dv.value << "\n";
  }
  // Close data file
  dataFile.close();
  
  return true;
}
