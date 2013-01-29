#pragma once
#ifndef _METROPOLISSTRATEGY_H
#define _METROPOLISSTRATEGY_H

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

  /**
   * Schreibt ein Bild mit den gemessenen Wahrscheinlichkeiten.
   */
  virtual void writeImageProbability(char* path) = 0;

  /**
   * Schreibt ein Bild mit aktuellen Spins.
   */
  virtual void writeImageSpins(char* path) = 0;

  /**
   * Setzt die Messung zurück und verteilt die Spins wieder zufaellig
   */
  virtual void reset() = 0;

  /**
   * Kopiert alle Einstellungen und gibt eine neue Strategie zurueck
   * Dabei sind alle Messwert 0 und die Spinverteilung ist zufaellig
   */
  virtual MetropolisStrategy& clone() const = 0;
}; 


#endif
