#include "WriteImage.h"
#include <iostream> 
#include <fstream> 

using namespace std;

void writePgm(int xsize, int ysize, double* data, char* path)
{
  double MaxValue = -1;
  double MinValue = 1;

  // find max and min
  /*
  for(int i=0 ; i < xsize * ysize; i++ )
  {
    if( data[i] < MinValue )
    {
      MinValue = data[i]; 
    }
    if( data[i] > MaxValue )
    {
      MaxValue = data[i];
    }
  }
  */

  int pixel[xsize*ysize];
  for(int i=0; i<xsize*ysize; i++)
  {
    pixel[i] = 127.0 * (data[i] / MaxValue - MinValue);
  }

  ofstream out(path); 
  if(!out) 
  {
    cout << "#Cannot open file.\n"; 
  } 
  else
  {
    out << "P2" << endl
      << xsize << endl
      << ysize << endl
      << 255 << endl;

    for(int i=0; i<xsize*ysize; i++)
    {
      out << pixel[i] << " ";
      if(i%17==0 && i>0)
      {
        out << endl;
      }
    }
    cout << "#" << path << " written.\n";
    out.close(); 
  }

}
