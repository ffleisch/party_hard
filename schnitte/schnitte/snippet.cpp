#include "snippet.h"


snippet::snippet(int samples,WAVEFORMATEXTENSIBLE* wformat)
{
	fnum = samples;
	format = wformat;
	data=new BYTE[samples*format->Format.nBlockAlign];
	end = data;
	size = samples * format->Format.nBlockAlign;
}


snippet::~snippet()
{
	if (data != NULL) {
		delete[] data;
	}
	if (idata != NULL) {
		delete[] idata;
	}
}

void snippet::print() {
	printf("Size: %d Frames %d Bytes\n",fnum,size);
	printf("Data:\n");
	for (int i = 0;i < size;i++) {
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
		if ((end-data)/sizeof(BYTE)<size) {
			*end = nData[i];
			end++;
		}
	}
	return false;
}

void snippet::getInts()
{
	delete[] idata;
	WORD intnum = size /(format->Format.nBlockAlign);
	idata = new kiss_fft_cpx[intnum];

	for (int i = 0;i < intnum;i++) {
		idata[i].r = *(float*)(data+i*format->Format.nBlockAlign);
		idata[i].i = 0;
	}

}

void snippet::reset()
{
	end = data;
}
