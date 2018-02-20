#pragma once

#ifndef COMMON_INC
#include "common.h"
#endif // !COMMON_INC

#define SNIPPET_INC

class snippet
{
public:
	BYTE* data;
	BYTE* end;
	kiss_fft_cpx* idata;
	WAVEFORMATEXTENSIBLE* format;
	int size=0;
	int fnum = 0;
	
	void print();
	bool add(BYTE* nData,int num);

	void getInts();
	void reset();

	snippet(int minSize, WAVEFORMATEXTENSIBLE* wformat);
	~snippet();
};

