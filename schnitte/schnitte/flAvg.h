#pragma once
#include "common.h"

#ifndef FLAVG_H
#define FLAVG_H

class flAvg
{
public:
	int len = 1;
	double* val;
	double fv = 0.5;
	flAvg(int num,double filter,double init=0);
	void add(double* inp);
	~flAvg();
};
#endif // !FLAVG_H

