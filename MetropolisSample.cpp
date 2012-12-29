
#include "MetropolisSample.h"
#include "common.h"

#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

MetropolisSample::MetropolisSample(int size, double j, double b)
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








