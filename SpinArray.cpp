
#include "SpinArray.h"

SpinArray::SpinArray(int length)
{
	_length = 1 + ((length - 1) / (sizeof(char) * 8));
  
	_array = new char[_length];
}

int SpinArray::GetSpin(int i) const
{
	int j = i / (sizeof(char) * 8);  // char ist 8 bit lang
	i = i % (sizeof(char) * 8);
	char spins = _array[j];

	char s = spins & ( '\1' << i);

	return s == 0 ? -1 : 1;
}

void SpinArray::SetSpin(int i, int val)
{
	int j = i / (sizeof(char) * 8);  // char ist 8 bit lang
	i = i % (sizeof(char) * 8);
	char spins = _array[j];
	
	char s;
	if( val == 1) // bit == 1
	{
		s = spins | ( '\1' << i);
	}
	else // val == -1 =>  bit == 0
	{
		s = spins & (~( '\1' << i));
	}

	_array[j] = s;
	
}

void SpinArray::Flip(int i)
{
	int j = i / (sizeof(char) * 8);  // char ist 8 bit lang
	i = i % (sizeof(char) * 8);
	char spins = _array[j];
	
	_array[j] = spins ^ ('\1' << i);
}
