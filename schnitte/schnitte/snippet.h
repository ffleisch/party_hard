#pragma once

#ifndef SNIPPET_H
#define SNIPPET_H

#include "common.h"

#include "flAvg.h"

class hsvModder;
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
	long resetnum = 0;
	//temp code für farbe
	/*
	float g1 = 100;
	float g2 = 200;
	double brigh = 128;
	double gpf = 1;//geheimer pop-faktor
	double gpf2 = 1;

	hsvModder* mod;
	*/
	void print();//infos und inhalt in konsole ausgeben
	bool add(BYTE* nData,int num);
	void addZeroes(int num);
	//num samples aus nData an Snippet anfügen

	void getInts();//wandelt Arrays von bytes zu kiss_fft_cpx
	void reset();//setzt snippet zurpck, neue datan werden am anfagng geschrieben

	void transform();//führt fft an sdata aus 



	//temp code für farbe
	//void getCol1(int* out,int* ind,int* avgs);
	//void getCol2(int* out, int* ind);
	//void getCol3(int* out,int* ind);

	//flAvg* avg;
	//flAvg* sqE;
	//flAvg* rgb;
	//flAvg* outV;


	snippet(int minSize, WAVEFORMATEXTENSIBLE* wformat);// erzeugt snippet welches minSize samples enthalten kann im geg format
	~snippet();//destuctor
};

#endif