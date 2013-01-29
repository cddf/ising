#pragma once
#ifndef _METROPOLISND_H
#define _METROPOLISND_H


#include "MetropolisStrategy.h"
#include "SpinArray.h"


class MetropolisND : public MetropolisStrategy
{
  private:
  SpinArray* _spins;
  int _dim;
  int *_size;
  int _spinnumber; // cache zur Zeitersparniss
  double _j;
  double _b;
  double* _Ws; // Einzelne Wahrscheinlichkeiten
  int* _NWs; // Einzelne Summen der Wahrscheinlichkeiten

  void getCoord(int i, int coord[]) const;
  int getLin(const int coord[]) const;
  public:

  /**
   *
   * dim: Anzahl der Dimensionen
   * ...: int size in der entsprechenden Dimension
   */
  MetropolisND(double j, double b, int dim, ... );

  MetropolisND(double j, double b, int dim, int size[] );

  // destrcutor
  ~MetropolisND()
  {
   	delete _spins;
	  delete [] _size;
	  delete [] _Ws;
	  delete [] _NWs;
  }
  
  void reset();
  MetropolisStrategy& clone() const;

  double calculate_dE(int i) const;
  void flipSpin(int i);
  void addProbability(int i);
  double measure() const;
  void writeImageProbability(char* path);
  void writeImageSpins(char* path);

  int spinNumber() const
  {
	  return _spinnumber;
  }

  
};

#endif
