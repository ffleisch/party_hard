#pragma once

#ifndef HSVMODDER_H
#include "common.h"
#define HSVMODDER_H

//klasse kann gegebene Farben im HSV Farbraum ändern
class hsvModder{
public:
	double pfs[3];
	double mult[3];
	double addi[3];
	void mod(int* col);
	hsvModder(double * pf, double * mul, double * additon);
private:
	void cap(int * inp);

};
void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);
void RGBtoHSV(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);

#endif
