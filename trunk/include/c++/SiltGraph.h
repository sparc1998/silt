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

#ifndef _SILTGRAPH_H_
#define _SILTGRAPH_H_

#include <vector>
#include <string>
#include <iostream>

namespace silt{
  class SiltGraph{
  public:
    enum OutputType{
      SGT_START,
      SGT_GNUPLOT = SGT_START,
      SGT_PNG,
      SGT_END = SGT_PNG
    };
    struct DoublePair{
      DoublePair(double val1, double val2) : v1(val1), v2(val2) {}
      double v1;
      double v2;
    };

    SiltGraph();
    virtual ~SiltGraph() {}
    virtual bool generate(enum OutputType ot, std::string baseFilename);

    void setTitle(std::string t){ _title = t; }
    std::string getTitle(void){ return _title; }
    void setUseTimestamp(bool val){ _timestamp = val; }
    bool getUseTimestamp(void){ return _timestamp; }
    void setDisplayGrid(bool val){ _grid = val; }
    bool getDisplayGrid(void){ return _grid; }
    void setIncludeLegend(bool val);
    bool getIncludeLegend(void);
    void setXaxisTitle(std::string t){ _xaxisTitle = t; }
    std::string getXaxisTitle(void){ return _xaxisTitle; }
    void setYaxisTitle(std::string t){ _yaxisTitle = t; }
    std::string getYaxisTitle(void){ return _yaxisTitle; }
    void setXrange(double min, double max);
    DoublePair getXrange(void);
    void setXticInterval(double interval);
    double getXticInterval(void);
    void setYrange(double min, double max);
    DoublePair getYrange(void);
    void setYticInterval(double interval);
    double getYticInterval(void);

    virtual std::string graphType(void) = 0;
    virtual bool isLegendAvailable(void) = 0;
    virtual bool isXAxisNumeric(void) = 0;
    virtual bool isYAxisNumeric(void) = 0;

  protected:
    std::string _title;
    bool _timestamp;
    bool _grid;
    bool _includeLegend;
    std::string _xaxisTitle;
    std::string _yaxisTitle;
    double _xmin;
    double _xmax;
    double _xInterval;
    double _ymin;
    double _ymax;
    double _yInterval;

    std::string getOutputLine(enum OutputType ot, std::string baseFilename);
    std::string getTerminalLine(enum OutputType ot);
    virtual bool outputGplotDirectives(std::ostream& o,
				       std::string baseFilename) = 0;
    virtual bool outputDataFiles(enum OutputType ot,
				 std::string baseFilename) = 0;
  };

  class LineGraph : public SiltGraph{
  public:
    typedef std::vector<DoublePair> DataVector;
    typedef DataVector::iterator DataVectorIt;
    struct DataSeries{
      std::string name;
      DataVector data;
      std::string str(void);
    };
    typedef std::vector<DataSeries*> DataCollection;
    typedef DataCollection::iterator DataCollectionIt;

    LineGraph();
    virtual ~LineGraph();

    virtual inline std::string graphType(void){ return "line graph"; }
    virtual inline bool isLegendAvailable(void){ return true; }
    virtual inline bool isXAxisNumeric(void){ return true; }
    virtual inline bool isYAxisNumeric(void){ return true; }

    void nameDataSeries(unsigned dataSeriesIndex, std::string name);
    void add(unsigned dataSeriesIndex, double x, double y);
    void add(unsigned dataSeriesIndex, DoublePair& v);

  protected:
    DataCollection _data;

    virtual bool outputGplotDirectives(std::ostream& o,
				       std::string baseFilename);
    virtual bool outputDataFiles(enum OutputType ot,
				 std::string baseFilename);
    DataSeries* getDataSeries(unsigned dataSeriesIndex);
  };
}

#endif // _SILTGRAPH_H_
