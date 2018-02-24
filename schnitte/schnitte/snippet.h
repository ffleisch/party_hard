#pragma once

#ifndef COMMON_INC
#include "common.h"
#endif // !COMMON_INC

#define SNIPPET_INC
//vlt rgb config mit kanälen
class snippet
{
public:
	BYTE* data;
	BYTE* end;
	kiss_fft_cpx** sdata;
	WAVEFORMATEXTENSIBLE* format;
	kiss_fft_cpx** freq;
	int samples = 0;
	int bytenum=0;
	
	void print();
	bool add(BYTE* nData,int num);

	void getInts();
	void reset();

	void transform();
	void getCol1(int* out,int* ind);
	void getCol2(int* out,int* ind);
	float havg[3];
	float havg2[3];
	float rgb[3];


	snippet(int minSize, WAVEFORMATEXTENSIBLE* wformat);
	~snippet();
};

