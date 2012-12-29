#include "common.h"

#include <math.h>
#include <stdlib.h>


double drand()
{
  return (double)rand() / ((double)RAND_MAX + 1); 
}