#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include "MetropolisStrategy.h"
#include "MetropolisSample.h"
#include "Metropolis2D.h"
#include "IsingMetropolis.h"


#include "SpinArray.h"
using namespace std;






int main (int argc, char** argv)
{
  SpinArray* a = new SpinArray(2);
  srand(1);
  //MetropolisSample* ms = new MetropolisSample(20, 1, 2);
  MetropolisStrategy* ms = new Metropolis2D(4,5, 1, -2);
  double M = isingLoop(ms);
  cout << "\nMagnetisierung M = " << M << "\n";
  delete ms;
}
