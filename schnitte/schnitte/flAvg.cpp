#include "flAvg.h"



flAvg::flAvg(int num, double filter,double init)
{

	len = num;
	fv = filter;
	val = new double[len];
	for (int i = 0;i < len;i++) {
		val[i] = init;
	}
}

void flAvg::add(double * inp)
{
	for (int i = 0;i < len;i++) {
		val[i] = inp[i]*fv+val[i]*(1-fv);
		if (val[i] != val[i]) {
			val[i] = 0;
		}
	}
}

flAvg::~flAvg()
{
	delete val;
}
