#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include "MetropolisStrategy.h"
#include "MetropolisSample.h"
#include "IsingMetropolis.h"

using namespace std;






int main (int argc, char** argv)
{
  MetropolisSample* ms = new MetropolisSample(20, 1, 1e-2);
  double M = isingLoop(ms);
  cout << "\nMagnetisierung M = " << M << "\n";
  delete ms;
}
