#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <omp.h>
#include <fstream>

#include "MetropolisStrategy.h"
#include "MetropolisSample.h"
#include "Metropolis2D.h"
#include "MetropolisND.h"

#include "IsingMetropolis.h"

#define kB  1.3806488e-23

#include "SpinArray.h"

int _xspins,
    _yspins,
    _zspins,
    _dim, //Dimension
    _running;
double _J,
       _B,
       _beta;

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

/*
 * Lese Einstellungs-Datei
 */
void readConfig(int argc, char** argv, double& betaMax, int& steps)
{
  fstream f;
  string line;
  f.open(argv[1], ios::in);
  while (!f.eof())
  {
    getline(f, line);
    unsigned eq = line.find("=");
    string variable = line.substr(0,eq);
    string wert = line.substr(eq+1);

    if(strcmp(variable.c_str(),"dim") == 0)
    {
      _dim = atoi(wert.c_str());
    }
    if(strcmp(variable.c_str(),"xspins") == 0)
    {
      _xspins = atoi(wert.c_str());
    }
    else if(strcmp(variable.c_str(),"yspins") == 0)
    {
      _yspins = atoi(wert.c_str());
    }
    else if(strcmp(variable.c_str(),"zspins") == 0)
    {
      _zspins = atoi(wert.c_str());
    }
    else if(strcmp(variable.c_str(),"running") == 0)
    {
      _running = atoi(wert.c_str());
    }
    else if(strcmp(variable.c_str(),"T") == 0)
    {
      _beta = 1.0/(kB * strtod(wert.c_str(), NULL));
    }
    else if(strcmp(variable.c_str(),"beta") == 0)
    {
      _beta = strtod(wert.c_str(), NULL);
    }
    else if(strcmp(variable.c_str(),"J") == 0)
    {
      _J = strtod(wert.c_str(), NULL);
    }
    else if(strcmp(variable.c_str(),"B") == 0)
    {
      _B = strtod(wert.c_str(), NULL);
    }
    else if(strcmp(variable.c_str(),"betaMax") == 0)
    {
      betaMax = strtod(wert.c_str(), NULL);
    }
    else if(strcmp(variable.c_str(),"TMin") == 0)
    {
      betaMax = 1.0/(kB * strtod(wert.c_str(), NULL));
    }
    else if(strcmp(variable.c_str(),"steps") == 0)
    {
      steps = atoi(wert.c_str());
    }
  }
  f.close();
}


int main (int argc, char** argv)
{
  _beta = 1.23456e-3;
  _running = 1e3;
  _yspins = 1;
  double betaMax = _beta;
  int steps = 1;
  //parse command-line
  int j = 2;
  if (argc == 1 || strcmp(argv[1],"--help") == 0)
  {
    cout << "Parameter:\n"
      << "1d <# Spins> <J> <B> <beta> [<# Durchläufe] \n"
      << "2d <# Spins X> <# Spins Y> <J> <B> <beta>  [<# Durchläufe] \n"
      << "3d <# Spins X> <# Spins Y> <# Spins Z> <J> <B> <beta> [<# Durchläufe] \n\n"
      << "./ising <config-Datei>\n"
      << "mit folgenden Variablen:\n"
      << "dim=<int>\n"
      << "xspins=<int>\n"
      << "yspins=<int>\n"
      << "running=<int>\n"
      << "beta=<double>\n"
      << "T=<double> # Die Temperatur lässt sich auch in Kelvin angeben.\n"
      << "J=<double>\n"
      << "B=<double>\n"
      << "TMin=<double>\n"
      << "betaMax=<double>\n"
      << "steps=<int>\n"
      ;
    return 1;
  }
  else if (argc == 2) // lese config Datei
  {
    readConfig(argc, argv, betaMax, steps);
  }
  else
  {
    if (strcmp(argv[1], "1d") == 0)
    {
      _dim = 1;
      _xspins = atoi(argv[j++]);
    }
    else if (strcmp(argv[1], "2d") == 0)
    {
      _dim = 2;
      _xspins = atoi(argv[j++]);
      _yspins = atoi(argv[j++]);
    }
    else if (strcmp(argv[1], "3d") == 0)
    {
      _dim = 3;
      _xspins = atoi(argv[j++]);
      _yspins = atoi(argv[j++]);
      _zspins = atoi(argv[j++]);
    }
    else
    {
      cout << "Falsche Parameter\n"
        << argv[0] << argv[1] << argv[2];
      return 1;
    }
    _J = strtod(argv[j++], NULL);
    _B = strtod(argv[j++], NULL);
    _beta = strtod(argv[j++], NULL);
    if (argc == j + 1)
      _running = atoi( argv[j++]);
  }

  cout <<"#dim = " << _dim <<"\n"
    << "#xspins = " << _xspins << "\n"
    <<"#yspins = " << _yspins <<"\n"
    <<"#zspins = " << _yspins <<"\n"
    <<"#J = " << _J <<"\n"
    <<"#B = " << _B <<"\n"
    <<"#beta = " << _beta <<"\n"
    <<"#betaMax = " << betaMax <<"\n"
    <<"#steps = " << steps <<"\n"
    <<"#running = " << _running <<"\n\n";

  
  struct timeval start_time_init;
  struct timeval start_time_loop;
  struct timeval comp_time;

  gettimeofday(&start_time_init, NULL);
  srand(start_time_init.tv_usec);

  //MetropolisStrategy* ms = new Metropolis2D(xspins,yspins, J, B);
  MetropolisStrategy* ms;
  if(_dim == 1)
  {
    ms = new MetropolisND(_J, _B, _dim, _xspins);
  }
  else if(_dim == 2)
  {
    ms = new MetropolisND(_J, _B, _dim, _xspins,_yspins );
  }
  else if(_dim == 3)
  {
    ms = new MetropolisND(_J, _B, _dim, _xspins,_yspins,_zspins );
  }

  gettimeofday(&start_time_loop, NULL);
  double M = 0; //isingLoop(ms, _running, beta);
  betaSlice(*ms, _running, _beta, betaMax, steps);

  gettimeofday(&comp_time, NULL);
  double time_init = (start_time_loop.tv_sec - start_time_init.tv_sec) + (start_time_loop.tv_usec - start_time_init.tv_usec) * 1e-6;
  double time_loop = (comp_time.tv_sec - start_time_loop.tv_sec) + (comp_time.tv_usec - start_time_loop.tv_usec) * 1e-6;
  
  
  //cout << "\n#Magnetisierung M = " << M / xspins / yspins << "\n";
  cout << "#Berechnungszeit:\n#  Init = " 
	  << time_init << " sec\n#  Loop = " 
	  << time_loop << " sec\n#  Ges  = "
	  << time_loop +  time_init << " sec\n";


  delete ms;
  return 0;
}
