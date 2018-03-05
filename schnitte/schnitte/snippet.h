#pragma once

#include "common.h"

#include "flAvg.h"

#define SNIPPET_INC
class hsvModder;
//vlt rgb config mit kan�len
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
	//temp code f�r farbe
	/*
	float g1 = 100;
	float g2 = 200;
	double brigh = 128;
	double gpf = 1;//geheimer pop-faktor
	double gpf2 = 1;

	hsvModder* mod;
	*/
	void print();//infos und inhalt in konsole ausgeben
	bool add(BYTE* nData,int num);//num samples aus nData an Snippet anf�gen

	void getInts();//wandelt Arrays von bytes zu kiss_fft_cpx
	void reset();//setzt snippet zurpck, neue datan werden am anfagng geschrieben

	void transform();//f�hrt fft an sdata aus 



	//temp code f�r farbe
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
/*
class hsvModder{
public:
	double pfs[3];
	double mult[3];
	double addi[3];
	void mod(int* col);
	hsvModder(double * pf, double * mul, double * additon);

};*/