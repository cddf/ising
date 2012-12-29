#pragma once
#ifndef _SPINARRAY_H
#define _SPINARRAY_H


class SpinArray
{
private:
	char* _array;
	int _length;
public:
	//SpinArray(void) {}
		
	SpinArray(int length);
	
	~SpinArray(void)
	{
		delete [] _array;
	}

	int GetSpin(int i) const;

	void SetSpin(int i, int val);

	void Flip(int i);
};

#endif