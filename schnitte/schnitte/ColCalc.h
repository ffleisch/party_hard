#pragma once
#ifndef COLCALC_H
#define COLCALC_H
#include "snippet.h"
#include "hsvModder.h"
//brechnet farben aus frequenzen


//grundklasse
class ColCalc
{
public:
	snippet * source;
	double fSum(kiss_fft_cpx * start, float num);
	float fSumSt(kiss_fft_cpx * start, float num, float avg);
	void enhanceRGB(int * inp, double f);
	void remMin(int * inp);
	void cap(int * inp);
	ColCalc(snippet* inp);
	void CalcRGB(int* out);
	void Init();
	~ColCalc();
	int f1, f2;
};


//teilt frequenzen in drei bereiche und berechnet farben mithilfe des mittelwertes
class ColCalcBounds :public ColCalc {
public:
	void Init();
	void CalcRGB(int* out);
	double brigh = 150;
	double gpf = 1.8;
	double gpf2 = 1.8;
	double g1 = 880;
	double g2 = 1760;
	double avmul = 0.9;
	flAvg* avg;
	flAvg* outV;
	flAvg* rgb;
	ColCalcBounds(snippet* inp);


};


//berechnet farben indem hue dem zentrum folgt
class ColCalcHSV:public ColCalc{
public:
	flAvg* avgs;
	flAvg* outVal;
	flAvg* tAvg;
	flAvg* cenAvg;
	double speed=10;
	double brigh = 1;
	double valMul = 1.2;
	double* dist;
	double*diff;
	//double colCor[3] = {255,255,200};
	double rgb[3];
	void calcEuc(double* avg);

	double calcCenter(double* avg);
	void CalcRGB(int* out);

	void Init();


	ColCalcHSV(snippet* inp);
	~ColCalcHSV();
private:

};



#endif