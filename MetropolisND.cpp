#include "MetropolisND.h"

#include "common.h"

#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdarg.h>

#include "WriteImage.h"

using namespace std;

MetropolisND::MetropolisND(double j, double b, int dim, ...)
{
	_dim  = dim;
	_size = new int[dim];
	
	_spinnumber = 1;

    va_list params; // Zugriffshandle für Parameter
    va_start(params, dim); // Zugriff vorbereiten
    // Durchlaufe alle Parameter 
    for (int i=0; i < dim; i++)
    {
        _spinnumber *= (_size[i] = va_arg(params, int)); // hole den Parameter, berechne size
    }
    va_end(params); // Zugriff abschließen


    
	_spins = new SpinArray(_spinnumber);
    _j = j;
    _b = b;
    _Ws = new double[_spinnumber];
    _NWs = new int[_spinnumber];


    // do random initialization with +1 or -1
    for(int i = 0; i < _spinnumber; i++)
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

    }
	//for(int i=0; i < _spinnumber; i++)
	//{
	//  cout << _spins->GetSpin(i) << " ";
	//  if(i % _size[0] == _size[0] - 1)
	//	  cout << "\n";
	//}
	//cout << "\n";
}

MetropolisND::MetropolisND(double j, double b, int dim, int size[])
{
	_dim  = dim;
	_size = new int[dim];
	
	_spinnumber = 1;

    for (int i=0; i < dim; i++)
    {
        _spinnumber *= (_size[i] = size[i]);
    }


    
	_spins = new SpinArray(_spinnumber);
    _j = j;
    _b = b;
    _Ws = new double[_spinnumber];
    _NWs = new int[_spinnumber];


    // do random initialization with +1 or -1
    for(int i = 0; i < _spinnumber; i++)
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


    }
	//for(int i=0; i < _spinnumber; i++)
	//{
	//  cout << _spins->GetSpin(i) << " ";
	//  if(i % _size[0] == _size[0] - 1)
	//	  cout << "\n";
	//}
	//cout << "\n";
}

MetropolisStrategy& MetropolisND::clone() const
{
  MetropolisND* ms = new MetropolisND(_j, _b, _dim, _size);

  return *ms;
}

void MetropolisND::reset()
{
   // do random initialization with +1 or -1
	for(int i = 0; i < _spinnumber; i++)
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

	  _Ws[i] = 0;
	  _NWs[i] = 0;

	}
}

void MetropolisND::resetMeasure()
{
   // do random initialization with +1 or -1
	for(int i = 0; i < _spinnumber; i++)
	{
	  _Ws[i] = 0;
	  _NWs[i] = 0;
	}
}

void MetropolisND::getCoord(int i, int coord[]) const
{
  //int* result = new int[_dim];
  for(int j = 0; j < _dim; j++)
  {
    coord[j] = i % _size[j];
	  i /= _size[j];
  }

  //return result;
}

int MetropolisND::getLin(const int coord[]) const
{
  int i = 0;
  int size = 1;
  for(int j = 0; j < _dim; j++)
  {
    i += coord[j] * size;
	  size *= _size[j];
  }
  return i;
}

double MetropolisND::calculate_dE(int i) const
{
  // dE = E_new - E_old;
  // dE psi = dH psi = (H_new - H_old) psi ;
  // S' == new, S == old
  // dH = sum_j J_ij * (S'_i * S_j - S_i * S_j) - (S'_i - S_i) * B
  // dS_i = S'_i - S_i = +- 2
  // dH = sum_j J_ij * (dS_i * S_j) - (dS_i) * B
  //
  // => dE = J dS_i sum_j S_j + B dS_i
  // dS_i = S_old - S_new
  int spin_old = _spins->GetSpin(i);
  int spin_new = -spin_old;

  int dS_i = spin_old - spin_new;


  int coord[_dim];
  //int* coord = new int[_dim]; 
  getCoord(i, coord);
  double jn = 0;
  for(int j = 0; j < _dim; j++)
  {
    int x = coord[j];
    int size = _size[j];
    
	// n.n. approx:
    if(size != 1)
    {
	  coord[j] = (x + 1)          % size;
      int nl = getLin(coord);
	  coord[j] = (x - 1 + size)   % size;
      int nr = getLin(coord);
	  coord[j] = x; 
      
      jn += ( _spins->GetSpin(nl) + _spins->GetSpin(nr));
    }
	
  }
  //delete [] coord;
  double dJ = _j * dS_i * jn;
  double dB = _b * dS_i; 

  return dJ + dB;
}

void MetropolisND::flipSpin(int i)
{
  _spins->Flip(i);
}

void MetropolisND::addProbability(int i)
{
  ++_NWs[i];
  _Ws[i] = (1.0 * _Ws[i] * _NWs[i] +  1.0 *_spins->GetSpin(i)) / (1.0 * _NWs[i] + 1.0);

}

double MetropolisND::measure() const
{
  // M = 1 / (N_i N_s) sum_i,s W_si * S_i
  // N_s = sum_s W_si
  
  double M = 0;

  for (int i = 0; i < _spinnumber; i++)
  {
    M += _Ws[i];
  }
  return M;
}

void MetropolisND::writeImageProbability(char* path)
{
  writePgm(_size[0],_size[1],_Ws,path);
}


void MetropolisND::writeImageSpins(char* path)
{
  int coord[_dim < 2 ? 2 : _dim];
  for(int i = 0; i < _dim;i++)
    coord[i] = 0;
  
  int xdim = _size[0];
  int ydim = _dim >= 2 ? _size[1] : 1;

  double* spins;
  spins = new double[xdim * ydim];

  for(int i=0; i< xdim;i++)
  {
    coord[0] = i;
    for(int j=0; j< ydim;j++)
    {
      coord[1] = j;
      int index = getLin(coord);
      spins[i * ydim + j] = _spins->GetSpin(index);
    }
  }
  
  writePgm(xdim,ydim,spins,path);
  delete[] spins;
}
