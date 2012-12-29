#include "IsingMetropolis.h"

#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "common.h"



using namespace std;


// ----------  template function --------
void isingLoop(MetropolisStrategy* ms)
{

  //srand(1); // init seed, 1 for determinism             
  int running = 1000;
  int measure = 100; // Messung alle x flips
  const double beta = 1.23456; // beta vllt in MetropolisStrategie zutun

  while (running > 0)
  {
    // 1. Pick random element
    int i = rand() % ms->spinNumber();


    // 2. clac the Energy

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

    if (running % 10 == 0)
    {
      double M = ms->measure();
      
      cout << "Magnetisierung: " << M << "\n";
    }
  }
}

