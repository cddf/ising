#pragma once
#ifndef _ISINGMETROPOLIS_H
#define _ISINGMETROPOLIS_H

#include "MetropolisStrategy.h"

double isingLoop(MetropolisStrategy* ms, int running, const double beta, double *flipRate);


#endif

