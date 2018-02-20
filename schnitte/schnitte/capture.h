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


	void dumpBuffer();
	void getSnippet(snippet* out);
	void printFotmat();


	capture();
	~capture();
};

HRESULT get_default_device(IMMDevice **ppMMDevice);