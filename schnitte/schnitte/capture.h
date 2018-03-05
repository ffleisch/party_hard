#pragma once

#ifndef COMMON_INC
#include "common.h"
#endif

#ifndef SNIPPET_INC
#include "snippet.h"
#endif

class capture
{
public:
	WAVEFORMATEX* format;

	IMMDevice* device;
	IAudioClient* client;
	IAudioCaptureClient* cclient;

	int snippetSize = 100;


	void dumpBuffer();//inhalt des buffers entleeren, nutzlos
	void getSnippet(snippet* out);//gegebenes snippet mit samples füllen
	void printFotmat();//infos in konsole ausgeben


	capture();
	~capture();
};

//standard Audioausgabe-gerät finden 
HRESULT get_default_device(IMMDevice **ppMMDevice);