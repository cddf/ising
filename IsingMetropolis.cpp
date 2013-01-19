#include "IsingMetropolis.h"
#include <omp.h>

#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "common.h"

using namespace std;

// ----------  template function --------
double isingLoop(MetropolisStrategy* ms, int running, const double beta)
{

  //srand(1); // init seed, 1 for determinism             
  //int running = 1e3;
  int measure = running/10; // Messung alle x flips
  int i = 0;
  bool random = true;
  double M;
  //omp_set_num_threads(0);

  //#pragma omp parallel for
  for (int n = 0; n <= running; n++)
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
    //running--;
    // 3. Do Measure
    // ...
    // die Messungen aufzeichenen und am ende ergebnis zurückliefern, daher ist void wohl nicht die beste wahl....
    ms->addProbability(i);

//    if (n % measure == 0)
//    {
//      M = ms->measure();
//      
//      //cout << "  Magnetisierung: " << M << "\n";
//    }

  }
  M = ms->measure();
  return M;
}

