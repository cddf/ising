#include "IsingMetropolis.h"

#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "common.h"



using namespace std;


// ----------  template function --------
double isingLoop(MetropolisStrategy* ms)
{

  //srand(1); // init seed, 1 for determinism             
  int running = 10000000;
  int measure = 1000000; // Messung alle x flips
  const double beta = 1.23456e0; // beta vllt in MetropolisStrategie zutun
  int i = 0;
  bool random = true;
  double M;

  while (running > 0)
  {
    if (random)
    {
    // 1. Pick random element
    i = rand() % ms->spinNumber();
    }
    else
    {
    // Ein Element nach dem anderen
    i++;
    if (i >= ms->spinNumber())
      i = 0;
    }

    // 2. calc the Energy

    // dE = E_new - E_old;
    double dE = ms->calculate_dE(i);

    double W = dE > 0.0 ? exp(-beta*dE) : 1.0;

    if(W >= 1.0  || drand() < W)
    {
      // accept
      ms->flipSpin(i);
    }
    else
    {
      // reject, do nothing
    }

    running--;
    // 3. Do Measure
    // ...
    // die Messungen aufzeichenen und am ende ergebnis zurückliefern, daher ist void wohl nicht die beste wahl....
    ms->addProbability(i);

    if (running % measure == 0)
    {
      M = ms->measure();
      
      cout << "  Magnetisierung: " << M << "\n";
    }
  }
  return M;
}

