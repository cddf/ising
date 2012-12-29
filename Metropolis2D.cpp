
#include "Metropolis2D.h"

#include "common.h"

#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

Metropolis2D::Metropolis2D(int sizeX,int sizeY, double j, double b)
{
    _sizeX = sizeX;
	_sizeY = sizeY;
	int size = sizeX*sizeY;
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
	  if(i % _sizeX == _sizeX - 1)
		  cout << "\n";
    }
    cout << "\n";
}

double Metropolis2D::calculate_dE(int i) const
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


  int x = i % _sizeX;
  int y = i / _sizeX;
  // n.n. approx:
  // TODO: find n.n.
  int nl, nr, nu, nd;
  // zyklische Randbedingungen
  nl = y * _sizeX +  (x + 1)          % _sizeX;
  nr = y * _sizeX +  (x - 1 + _sizeX) % _sizeX; 
  
  nu = x + _sizeX * ((y + 1)          % _sizeY);
  nd = x + _sizeX * ((y - 1 + _sizeY) % _sizeY);

  
  double nn = _spins[nl] + _spins[nr] 
		+ _spins[nu] + _spins[nd];
  double dJ = _j * dS_i * nn;
  double dB = _b * dS_i; 


  return dJ - dB;
}

void Metropolis2D::flipSpin(int i)
{
  _spins[i] *= -1;
  // FIXME remove
  //cout << "Spins: ";
  //for (int i = 0; i < _sizeX*_sizeY; i++)
  //{
  //  cout << _spins[i] << " ";
  //}
  //cout << "\n";
}

void Metropolis2D::addProbability(int i)
{
  _NWs[i]++;
  _Ws[i] = _Ws[i] + _spins[i] / _NWs[i];

}

double Metropolis2D::measure() const
{
  // M = 1 / (N_i N_s) sum_i,s W_si * S_i
  // N_s = sum_s W_si
  
  double M = 0;

  cout << "Wahrscheinlichkeiten der Spins:  ";
  for (int i = 0; i < _sizeX*_sizeY; i++)
  {
    M += _Ws[i];
    cout << _Ws[i] << " ";
	if(i % _sizeX == _sizeX - 1)
		  cout << "\n";
  }
  cout << "\n";

  return M;
}








