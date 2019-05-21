#include "ColCalc.h"

double ColCalc::fSum(kiss_fft_cpx* start, float num) {
	double s = 0;
	for (int i = 0;i < num;i++) {
		s += sqrt(pow(start[i].r, 2) + pow(start[i].i, 2));
	}
	return(s);
}

float ColCalc::fSumSt(kiss_fft_cpx* start, float num, float avg) {
	float s = 0;
	for (int i = 0;i < num;i++) {
		s += pow(sqrt(pow(start[i].r, 2) + pow(start[i].i, 2)) - avg, 2);
	}
	return(s);
}
void ColCalc::enhanceRGB(int* inp, double f) {
	for (int i = 0;i < 3;i++) {
		inp[i] = pow(inp[i] / 255.f, f) * 255;
	}
}
void ColCalc::remMin(int* inp) {
	for (int i = 0;i < 3;i++) {
		if (min(inp[0], inp[1], inp[2]) == inp[i]) {
			inp[i] *= 0.3;
		}
	}
}

void ColCalc::cap(int* inp) {
	for (int i = 0;i < 3;i++) {
		if (inp[i] > 255)inp[i] = 255;
		if (inp[i] < 0)inp[i] = 0;
	}
};



ColCalc::ColCalc(snippet * inp)
{
	source = inp;

}

void ColCalc::CalcRGB(int * out)
{
	out[0] = 255;
	out[1] = 128;
	out[2] = 0;
}

void ColCalc::Init()
{
}

ColCalc::~ColCalc()
{
}

ColCalcBounds::ColCalcBounds(snippet* inp) :ColCalc(inp) {
	source = inp;
	Init();
}
void ColCalcBounds::Init()
{
	f1 = (g1 / source->format->Format.nSamplesPerSec)*source->samples;
	f2 = (g2 / source->format->Format.nSamplesPerSec)*source->samples;
	avg = new flAvg(3,0.005);
	rgb = new flAvg(3, 0.08);
	outV = new flAvg(3, 0.5);
	avg->ignZero=false;
	
	avg->val[0]=5;
	avg->val[1] =5;
	avg->val[2] = 1.5;
}

void ColCalcBounds::CalcRGB(int * out)
{
	double zwcol = pow(brigh, 1 / gpf);
	double c[3] = { zwcol,zwcol,zwcol };


	//ind[0] = 0;
	//ind[1] = f1;
	//ind[2] = f2;
	double num[3];
	num[0] = fSum(source->freq[0], f1) / (f1);
	num[1] = fSum(source->freq[0] + f1, f2 - f1) / (f2 - f1);
	num[2] = fSum(source->freq[0] + f2, (source->samples / 2) - f2) / ((source->samples / 2) - f2);
	//printf("%f\n ",num[0]);
	avg->add(num);
	double zw[3];
	double zw2[3];
	for (int i = 0;i < 3;i++) {

		zw[i] = num[i] / avg->val[i];


		out[i] = (int)(pow(avmul*rgb->val[i] * c[i], gpf));
	}
	rgb->add(zw);

	//mod->mod(out);
	for (int i = 0;i < 3;i++) {
		zw2[i] = out[i];
	}
	outV->add(zw2);
	enhanceRGB(out, gpf2);
	cap(out);
	//remMin(out);
}

ColCalcHSV::ColCalcHSV(snippet* inp) :ColCalc(inp)
{
	source = inp;
	Init();
}

ColCalcHSV::~ColCalcHSV()
{
	delete tAvg;
	delete avgs;
	delete outVal;
	delete cenAvg;
}

void ColCalcHSV::calcEuc(double* avg) {
	*avg = 0;
	for (int i = 0; i < source->samples; i++) {
		dist[i] = sqrt(pow(source->freq[0][i].r, 2) + pow(source->freq[0][i].i, 2));
		(*avg) += dist[i];
	}
	*avg /= source->samples;
	tAvg->add(avg);
	avgs->add(dist);
	for (int i = 0; i < source->samples; i++) {
		dist[i] =  dist[i]-avgs->val[i];
		dist[i] = dist[i] < 0 ? 0 : dist[i];
	}
}

double ColCalcHSV::calcCenter(double* avg) {
	double sum = 0;
	double s2 = 0;
	for (int i = 0; i < source->samples/2; i++) {
			sum +=dist[i]*i;
			s2 += dist[i];

	}
	*avg= s2 / (source->samples / 2);
	//tAvg->add(avg);
	sum = (sum / s2) / (*avg);
	return(sum>50?50:sum);
};

void ColCalcHSV::CalcRGB(int * out)
{
	float r,g,b, h, s, v;
	s = 1;
	v = 1;
	double av = 0;
	double av2 = 0;

	calcEuc(&av);
	
	
	double cent=calcCenter(&av2);
	cenAvg->add(&cent);
	h = speed*(cent/cenAvg->val[0])*360;
	while (h >= 360) { h -= 360; }
	while (h < 0) { h += 360; }
	v = (float)av / (tAvg->val[0]*2);
	v = (float)pow(v,brigh)*valMul;
	HSVtoRGB(r,g,b,h,s,v);
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
	outVal->add(rgb);
	for (int i = 0; i < 3; i++) {
		out[i] = (int)(outVal->val[i]*255);
	}
	cap(out);
}

void ColCalcHSV::Init()
{
	avgs = new flAvg(source->samples, 0.005);
	outVal = new flAvg(3,0.1);
	tAvg = new flAvg(1, 0.005);
	cenAvg = new flAvg(1, 0.005);
	dist = new double[source->samples];
}



