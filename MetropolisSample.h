#pragma once
#ifndef _METROPOLISSAMPLE_H
#define _METROPOLISSAMPLE_H

#include "MetropolisStrategy.h"

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



  MetropolisSample(int size, double j, double b);

  // destrcutor
  ~MetropolisSample()
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
    return _size;
  }

}; 


#endif