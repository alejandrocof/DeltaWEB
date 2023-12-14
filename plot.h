#ifndef PLOT_H
#define PLOT_H

#include "gnuplot-iostream.h"
#include "data2d.h"

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Plot
{
public:
  string title;
  string xlabel;
  string ylabel;
  string name;
  string units;
  bool logscalexy;
  bool logscalex;
  bool logscaley;
  bool multipleDeployments;
  double ymin,ymax;
  double xmin,xmax;
  double dmax;
  int mxtics=-1,mytics=-1;

  void push_dat( Data2D &d );
  void push_dat( Data2D &d1, Data2D &d2);
  void draw(string);
    Plot();
private:
  Gnuplot gp;
  std::vector< Data2D > _dat;
  std::vector< Data2D > _dat2;
  bool dobleData=false;
};



#endif // PLOT_H
