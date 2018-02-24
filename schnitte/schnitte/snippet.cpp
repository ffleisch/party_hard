#include "snippet.h"



float fSum(kiss_fft_cpx* start,float num) {
	float s = 0;
	for (int i = 0;i < num;i++) {
		s += sqrt(pow(start[i].r,2)+ pow(start[i].i, 2));
	}
	return(s);
}

float fSumSt(kiss_fft_cpx* start, float num,float avg) {
	float s = 0;
	for (int i = 0;i < num;i++) {
		s += pow(sqrt(pow(start[i].r, 2) + pow(start[i].i, 2))-avg,2);
	}
	return(s);
}

void convolute() {

}

snippet::snippet(int samnum,WAVEFORMATEXTENSIBLE* wformat)
{
	samples = samnum;
	format = wformat;
	data=new BYTE[samples*format->Format.nBlockAlign];
	end = data;
	bytenum = samples * format->Format.nBlockAlign;
	sdata = new kiss_fft_cpx* [format->Format.nChannels];
	freq = new kiss_fft_cpx* [format->Format.nChannels];

	for (int i = 0;i < format->Format.nChannels;i++) {
		sdata[i] = new kiss_fft_cpx[samples];
		freq[i] = new kiss_fft_cpx[samples];
	}
	//to be removed//
	for (int i = 0;i < 3;i++) {//fuuuuck das ist unötig hehehehehhehehe
		rgb[i] = 0;
		havg[i] = 1;//retina schützen
		havg2[i] = 0;
	}
    //
}


snippet::~snippet()
{
	if (data != NULL) {
		delete[] data;
	}
	if (sdata != NULL) {
		for (int i = 0;i < format->Format.nChannels;i++) {
			delete sdata[i];
		}
		delete sdata;
	}
	if (freq != NULL) {
		for (int i = 0;i < format->Format.nChannels;i++) {
			delete freq[i];
		}
		delete freq;
	}
}

void snippet::print() {
	printf("Size: %d Frames %d Bytes\n",samples, bytenum);
	printf("Data:\n");
	for (int i = 0;i < bytenum;i++) {
		if (!(i % 8)) {
			printf("\n");
		}
		if(data+i!=NULL){
			printf("%03d ", *(data + i));
		}
	}
	printf("\nEnd\n");
}

bool snippet::add(BYTE* nData, int num)
{
	for (int i = 0;i < num;i++) {
		if ((end-data)/sizeof(BYTE)<bytenum) {
			*end = nData[i];
			end++;
		}
	}
	return false;
}

void snippet::getInts()
{
	for (int g = 0;g < format->Format.nChannels;g++) {
		for (int i = 0;i < samples;i++) {
			sdata[g][i].r = *(float*)(data + i * format->Format.nBlockAlign);
			sdata[g][i].i = 0;
		}
	}
}

void snippet::reset()
{
	end = data;
}

void snippet::transform()
{
	kiss_fft_cfg config = kiss_fft_alloc(samples, 0, NULL, NULL);

	for (int g = 0;g < format->Format.nChannels;g++) {
		kiss_fft(config, sdata[g], freq[g]);
	}
	kiss_fft_free(config);
}

void flM(float inV,float* out,float fNum) {
	*out = inV*(fNum)+*out*(1-fNum);
}

void snippet::getCol1(int* out,int* ind)
{
	float c[3] = {150,150,150};
	float g1 = 1320;
	float g2 = 5000;
	
	int f1 = (g1/format->Format.nSamplesPerSec)*samples;
	int f2 = (g2 / format->Format.nSamplesPerSec)*samples;

	ind[0] = 0;
	ind[1] = f1;
	ind[2] = f2;
	float num[3];
	num[0]=fSum(freq[0],f1)/(f1);
	num[1]=fSum(freq[0]+f1,f2-f1)/(f2 - f1);
	num[2]=fSum(freq[0]+f2,(samples/2)-f2)/((samples/2)-f2);

	for (int i = 0;i < 3;i++) {
		flM(num[i], &havg[i], 0.001);
		
		float s = pow(havg[i] - num[i], 2);
		flM(s, &havg2[i], 0.01);
		if (rgb[i] != rgb[i])rgb[i] = 0;
		flM(c[i]*(num[i]/havg[i]),&rgb[i],0.08);
		//flM(c[i] *(sqrt(s)/),&rgb[i],0.1 );
		if (rgb[i] > 255)rgb[i] = 255;
		float gpf = 1.6;//geheimer pop-faktor
		out[i] = (int)(pow((rgb[i]/255.f),gpf)*255);
	}
}

void snippet::getCol2(int * out,int* ind)
{
	float g1 = 10;
	float g2 = 440;

	int f1 = (g1 / format->Format.nSamplesPerSec)*samples;
	int f2 = (g2 / format->Format.nSamplesPerSec)*samples;
	
	ind[0] = 0;
	ind[1] = f1;
	ind[2] = f2;

	
	float c = 128;
	float num = fSum(freq[0] + f1, f2 - f1) / (f2 - f1);
	flM(num, &havg[0], 1 / 500.f);
	float s = pow(havg[0] - num, 2);
	flM(sqrt(s), &havg2[0], 0.01);
	for (int i = 0;i < 3;i++) {

		
		flM(c *(sqrt(s)/havg2[0]), &rgb[i], 0.07);

		if (rgb[i] > 255)rgb[i] = 255;
		out[i] = rgb[i];
	}
}
