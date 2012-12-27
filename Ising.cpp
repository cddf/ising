#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


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
   * Calculates the Energiedifference between fliped (Spin i) and actual state
   */     
  virtual double calculate_dE(int i) const = 0;
  /**
   * Flips the Spin i
   */     
  virtual void flipSpin(int i) = 0;
  
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
  char* _spins;
  int _size;
  double _j;
  double _b;

  public:
  
  MetropolisSample(int size, double j, double b)
  {
    _size = size;
    _spins = new char[size];
    _j = j;
    _b = b;
    
    // TODO: do random initialization with +1 or -1
    // or maybe add method to Strategie for init with seed parameter
    for(int i = 0; i < size; i++)
    {
      _spins[i] = 1;  // <- do random here 
    }
  }
  // destrcutor
  ~MetropolisSample()
  {
    delete [] _spins;
  }
      
  double calculate_dE(int i) const;
  void flipSpin(int i);
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
  char spin_old = _spins[i];
  char spin_new = -spin_old;
  
  char dS_i = spin_new - spin_old;  
  
  // n.n. approx:
  // TODO: find n.n.  
  double nn = 1;
  double dJ = _j * dS_i * nn;
  double dB = _b * dS_i; 


  return dJ - dB;
}

void MetropolisSample::flipSpin(int i)
{
  _spins[i] *= -1;  
}

double MetropolisSample::measure() const
{
  // TODO do measure ...
  return 0;  
}









// ----------  template function --------
void isingLoop(MetropolisStrategy* ms)
{

  //srand(1); // init seed, 1 for determinism             
  int running = 10;
  const double beta = 1.23456; // beta vllt in MetropolisStrategie zutun
  
  while (running > 0)
  {
    // 1. Pick random element
    int i = rand() % ms->spinNumber();
    
  
    // 2. clac the Energie
    
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
    
  }
}


int main (int argc, char** argv)
{
  MetropolisSample* ms = new MetropolisSample(10, 1, -1);
  isingLoop(ms);
  // TODO: print result 
  delete ms;
}