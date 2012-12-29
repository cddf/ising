#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include "MetropolisStrategy.h"
#include "MetropolisSample.h"
#include "Metropolis2D.h"
#include "IsingMetropolis.h"

using namespace std;






int main (int argc, char** argv)
{
  //MetropolisSample* ms = new MetropolisSample(20, 1, 2);
	MetropolisStrategy* ms = new Metropolis2D(4,5, 1, -2);
  isingLoop(ms);
  // TODO: print result 
  delete ms;
}
