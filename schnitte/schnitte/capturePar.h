#pragma once

#include "common.h"
#include "snippet.h"

#ifndef THREAD_H
#define THREAD_H
#include <thread>
#endif // !THREAD_H

#ifndef CAPTUREPAR_H
#define CAPTUREPAR_H


class capturePar
{
public:
	WAVEFORMATEX * format;

	IMMDevice* device;
	IAudioClient* client;
	IAudioCaptureClient* cclient;
	
	std::thread captureThread;

	std::list<BYTE> byteList;
	int byteNum = 0;
	boolean run = true;
	boolean readingL;
	boolean writingL;

	void printFormat();//infos in konsole ausgeben
	void getSnippet(snippet* out);//gegebenes snippet mit samples füllen
	const void fillList();

	HRESULT get_default_device(IMMDevice **ppMMDevice);


	capturePar();
	~capturePar();
};

#endif // !CAPTUREPAR_H
