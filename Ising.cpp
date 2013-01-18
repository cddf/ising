#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <omp.h>


#include "MetropolisStrategy.h"
#include "MetropolisSample.h"
#include "Metropolis2D.h"
#include "MetropolisND.h"

#include "IsingMetropolis.h"


#include "SpinArray.h"
using namespace std;



void betaSlice(MetropolisStrategy& ms1, int running, double betaMin, double betaMax, int steps)
{
  const double betaStep = (betaMax - betaMin) / steps;
  

  //cout << "Threads max = " << omp_get_max_threads() << "\n";
  
  cout << "#Beta Magnetisierung\n";
  
  if(steps < omp_get_max_threads())
    omp_set_num_threads(steps);

  //cout << "Threads max = " << omp_get_max_threads() << "\n";
  
  MetropolisStrategy& ms = ms1;
  #pragma omp parallel 
  {

    bool notmaster = true;
    
    #pragma omp master
    {
      //cout << "Threads num = " << omp_get_num_threads() << "\n";
  
      notmaster = false;
    }
    if(notmaster)
      ms = ms1.clone();
    
    #pragma omp for schedule(static) ordered
    for(int i= 0; i < steps; i++)
    {
      double beta = betaMin + betaStep * i;
      
      ms.reset();
      double M = isingLoop(&ms, running, beta);

      #pragma omp ordered 
        cout << beta << " " << M / ms.spinNumber() << "\n";

    }

  }
}




int main (int argc, char** argv)
{
  int xspins, yspins, running;
  double J, B;
  double beta = 1.23456e-3;
  running = 1e3;
  yspins = 1;
  //parse command-line
  int j = 2;
  if (strcmp(argv[1],"--help") == 0)
  {
    cout << "Parameter:\n"
      << "1d <# Spins> <J> <B> <beta> [<# Durchläufe] \n"
      << "2d <# Spins X> <# Spins Y> <J> <B> <beta>  [<# Durchläufe] \n";
    return 1;
  }
  else if (strcmp(argv[1], "1d") == 0)
  {
    xspins = atoi(argv[j++]);
    J = strtod(argv[j++], NULL);
    B = strtod(argv[j++], NULL);
    beta = strtod(argv[j++], NULL);
    if (argc == j + 1)
      running = atoi( argv[j++]);
  }
  else if (strcmp(argv[1], "2d") == 0)
  {
    xspins = atoi(argv[j++]);
    yspins = atoi(argv[j++]);
    J = strtod(argv[j++], NULL);
    B = strtod(argv[j++], NULL);
    beta = strtod(argv[j++], NULL);
    if (argc == j + 1)
      running = atoi( argv[j++]);
  }
  else
  {
    cout << "Falsche Parameter\n"
      << argv[0] << argv[1] << argv[2];
    return 1;
  }
  cout << "xspins = " << xspins << "\n"
    <<"yspins = " << yspins <<"\n"
    <<"J = " << J <<"\n"
    <<"B = " << B <<"\n"
    <<"beta = " << beta <<"\n"
    <<"running = " << running <<"\n\n";

  
  struct timeval start_time_init;
  struct timeval start_time_loop;
  struct timeval comp_time;

  gettimeofday(&start_time_init, NULL);
  srand(start_time_init.tv_usec);

  //MetropolisStrategy* ms = new Metropolis2D(xspins,yspins, J, B);
  MetropolisStrategy* ms = new MetropolisND(J, B, 2, xspins,yspins );

  gettimeofday(&start_time_loop, NULL);
  double M = 0; //isingLoop(ms, running, beta);
  betaSlice(*ms, running, beta, 20*beta, 50);

  gettimeofday(&comp_time, NULL);
  double time_init = (start_time_loop.tv_sec - start_time_init.tv_sec) + (start_time_loop.tv_usec - start_time_init.tv_usec) * 1e-6;
  double time_loop = (comp_time.tv_sec - start_time_loop.tv_sec) + (comp_time.tv_usec - start_time_loop.tv_usec) * 1e-6;
  
  
  cout << "\nMagnetisierung M = " << M / xspins / yspins << "\n";
  cout << "Berechnungszeit:\n  Init = " 
	  << time_init << " sec\n  Loop = " 
	  << time_loop << " sec\n  Ges  = "
	  << time_loop +  time_init << " sec\n";


  delete ms;
  return 0;
}
