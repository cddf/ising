#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

// compile with g++ Ising.cpp


/**
 * Returns a pseudo random number in [0,1)
 */ 
double drand()
{
  return (double)rand() / ((double)RAND_MAX + 1); 
}



// ----------  interface for strategie pattern ------------------
class MetropolisStrategy
{
  public:
  /**
   * Calculates the Energiedifference between flipped (Spin i) and actual state
   */     
  virtual double calculate_dE(int i) const = 0;

  /**
   * Flips the Spin i
   */     
  virtual void flipSpin(int i) = 0;

  /**
   * Aktualisiere Wahrscheinlichkeiten von Spin i mit Wert spin
   */     
  virtual void addProbability(int i) = 0;

  /**
   * Measures the state  
   */  
  virtual double measure() const = 0;

  /**
   * Retruns the number of spins
   */     
  virtual int spinNumber() const = 0;
}; 


// ----------  Sample for 1D an n.n. ------------------
class MetropolisSample : public virtual MetropolisStrategy
{
  private:
  int* _spins;
  int _size;
  double _j;
  double _b;
  double* _Ws; // Einzelne Wahrscheinlichkeiten
  int* _NWs; // Einzelne Summen der Wahrscheinlichkeiten

  public:

  MetropolisSample(int size, double j, double b)
  {
    _size = size;
    _spins = new int[size];
    _j = j;
    _b = b;
    _Ws = new double[size];
    _NWs = new int[size];


    // do random initialization with +1 or -1
    // or maybe add method to Strategie for init with seed parameter
    cout << "Initialisierung: ";
    for(int i = 0; i < size; i++)
    {
      int n;
      if (drand() <= 0.5)
      {
        n = -1;
      }
      else
      {
        n = 1;
      }
      _spins[i] = n;
      cout << " " << _spins[i];
    }
    cout << "\n";
  }
  // destrcutor
  ~MetropolisSample()
  {
    delete [] _spins;
  }

  double calculate_dE(int i) const;
  void flipSpin(int i);
  void addProbability(int i);
  double measure() const;

  int spinNumber() const
  {
    return _size;
  }

}; 

// ----- impls for sample class -----
double MetropolisSample::calculate_dE(int i) const
{
  // dE = E_new - E_old;
  // dE psi = dH psi = (H_new - H_old) psi ;
  // S' == new, S == old
  // dH = sum_j J_ij * (S'_i * S_j - S_i * S_j) - (S'_i - S_i) * B
  // dS_i = S'_i - S_i = +- 2
  // dH = sum_j J_ij * (dS_i * S_j) - (dS_i) * B
  int spin_old = _spins[i];
  int spin_new = -spin_old;

  int dS_i = spin_new - spin_old;

  // n.n. approx:
  // TODO: find n.n.
  int nl, nr;
  nl = i - 1;
  nr = i + 1;
  // zyklische Randbedingungen
  if (nl < 0)
  {
    nl = _size - 1;
  }
  if (nr >= _size)
  {
    nr = 0;
  }

  double nn = _spins[nl] + _spins[nr];
  double dJ = _j * dS_i * nn;
  double dB = _b * dS_i; 


  return dJ - dB;
}

void MetropolisSample::flipSpin(int i)
{
  _spins[i] *= -1;
  // FIXME remove
  cout << "Spins: ";
  for (int i = 0; i < _size; i++)
  {
    cout << _spins[i] << " ";
  }
  cout << "\n";
}

void MetropolisSample::addProbability(int i)
{
  _NWs[i]++;
  _Ws[i] = _Ws[i] + _spins[i] / _NWs[i];

}

double MetropolisSample::measure() const
{
  // M = 1 / (N_i N_s) sum_i,s W_si * S_i
  // N_s = sum_s W_si
  
  double M = 0;

  cout << "Wahrscheinlichkeiten der Spins:  ";
  for (int i = 0; i < _size; i++)
  {
    M += _Ws[i];
    cout << _Ws[i] << " ";
  }
  cout << "\n";

  return M;
}









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


int main (int argc, char** argv)
{
  MetropolisSample* ms = new MetropolisSample(20, 1, 2);
  isingLoop(ms);
  // TODO: print result 
  delete ms;
}
