#pragma once

#ifndef CAPTUREPAR_H
#define CAPTUREPAR_H

#ifndef COMMON_INC
#include "common.h"
#endif

#ifndef SNIPPET_INC
#include "snippet.h"
#endif

#ifndef THREAD_H
#define THREAD_H
#include <thread>
#endif // !THREAD_H


//enthält genau ein Sample
struct sam {
	byte* val;
};


//verwaltet eine Liste von type sam
class samChain{
public:
	WAVEFORMATEX * format;
	int len;
	int maxLen=1;
	void add(BYTE* inp,int num);
	void fillSnippet(snippet* out);
	bool isReading=false;
	bool isWriting=false;
	std::list<sam> raw;
	samChain(WAVEFORMATEX* f);
	~samChain();

private:

};


class capturePar
{
public:
	WAVEFORMATEX * format;

	IMMDevice* device;
	IAudioClient* client;
	IAudioCaptureClient* cclient;
	
	std::thread captureThread;

	samChain* data;

	int byteNum = 0;

	boolean run = true;

	void printFormat();//infos in konsole ausgeben
	//void getSnippet(snippet* out);//gegebenes snippet mit samples füllen
	void getSnippet(snippet* out);//gegebenes snippet mit samples füllen
	void fillList();

	HRESULT get_default_device(IMMDevice **ppMMDevice);


	capturePar();
	~capturePar();
};

#endif // !CAPTUREPAR_H
