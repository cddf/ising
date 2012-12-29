#pragma once
#ifndef _METROPOLIS2D_H
#define _METROPOLIS2D_H

#include "MetropolisStrategy.h"

class Metropolis2D : public virtual MetropolisStrategy
{
  private:
  int* _spins;
  int _sizeX;
  int _sizeY;
  double _j;
  double _b;
  double* _Ws; // Einzelne Wahrscheinlichkeiten
  int* _NWs; // Einzelne Summen der Wahrscheinlichkeiten

  public:

  Metropolis2D(int sizeX, int sizeY, double j, double b);

  // destrcutor
  ~Metropolis2D()
  {
    delete [] _spins;
	delete [] _Ws;
	delete [] _NWs;
  }

  double calculate_dE(int i) const;
  void flipSpin(int i);
  void addProbability(int i);
  double measure() const;

  int spinNumber() const
  {
	  return _sizeX*_sizeY;
  }

  

};

#endif