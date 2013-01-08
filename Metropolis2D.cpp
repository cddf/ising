
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
	_spins = new SpinArray(size);
    _j = j;
    _b = b;
    _Ws = new double[size];
    _NWs = new int[size];


    // do random initialization with +1 or -1
    // or maybe add method to Strategie for init with seed parameter
//    cout << "Initialisierung:\n";
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
      _spins->SetSpin(i, n);
//      cout << " " << _spins->GetSpin(i);;
//	  if(i % _sizeX == _sizeX - 1)
//		  cout << "\n";
    }
//    cout << "\n";
}

double Metropolis2D::calculate_dE(int i) const
{
  // dE = E_new - E_old;
  // dE psi = dH psi = (H_new - H_old) psi ;
  // S' == new, S == old
  // dH = sum_j J_ij * (S'_i * S_j - S_i * S_j) - (S'_i - S_i) * B
  // dS_i = S'_i - S_i = +- 2
  // dH = sum_j J_ij * (dS_i * S_j) - (dS_i) * B
  int spin_old = _spins->GetSpin(i);
  int spin_new = -spin_old;

  int dS_i = spin_new - spin_old;


  int x = i % _sizeX;
  int y = i / _sizeX;
  // n.n. approx:
  int nl, nr, nu, nd;
  double nx, ny;
  nx = ny = 0;

  // zyklische Randbedingungen
  if(_sizeY == 1)
  {
    nl = y * _sizeX +  (x + 1)          % _sizeX;
    nr = y * _sizeX +  (x - 1 + _sizeX) % _sizeX; 
    nx = _spins->GetSpin(nl) + _spins->GetSpin(nr);
  }
  if(_sizeX == 1)
  {
    nu = x + _sizeX * ((y + 1)          % _sizeY);
    nd = x + _sizeX * ((y - 1 + _sizeY) % _sizeY);
    ny = _spins->GetSpin(nu) + _spins->GetSpin(nd);
  }
  
  double nn = nx + ny;
  double dJ = _j * dS_i * nn;
  double dB = _b * dS_i; 


  return dJ - dB;
}

void Metropolis2D::flipSpin(int i)
{
  _spins->Flip(i);
}

void Metropolis2D::addProbability(int i)
{
  ++_NWs[i];
  _Ws[i] = (1.0 * _Ws[i] * _NWs[i] +  1.0 *_spins->GetSpin(i)) / (1.0 * _NWs[i] + 1.0);

}

double Metropolis2D::measure() const
{
  // M = 1 / (N_i N_s) sum_i,s W_si * S_i
  // N_s = sum_s W_si
  
  double M = 0;

//  cout << "Wahrscheinlichkeiten der Spins:\n";
  for (int i = 0; i < _sizeX*_sizeY; i++)
  {
    M += _Ws[i];
//    cout << _Ws[i] << " ";
//	if(i % _sizeX == _sizeX - 1)
//		  cout << "\n";
  }
//  cout << "\n";

  return M;
}








