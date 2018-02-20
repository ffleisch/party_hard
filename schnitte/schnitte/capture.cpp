#include "capture.h"



void capture::dumpBuffer()
{
	HRESULT erg = 0;

	erg = client->Start();

	if (FAILED(erg)) {
		printf("client couldnt start 0x%08x\n",erg);
	}

	unsigned int s = 0;
	cclient->GetNextPacketSize(&s);
	//printf("Next Packet is %d bytes big\n", s);
	while (s) {
		cclient->GetBuffer(NULL,&s, NULL, NULL, NULL);
		cclient->ReleaseBuffer(s);
		cclient->GetNextPacketSize(&s);
	}
	erg = client->Stop();
	if (FAILED(erg)) {
		printf("client couldnt stop 0x%08x\n", erg);
	}

}

void capture::getSnippet(snippet * out)
{
	HRESULT erg = 0;
	//dumpBuffer();
	out->reset();
	erg = client->Start();

	if (FAILED(erg)) {
		printf("client couldnt start 0x%08x\n", erg);
	}
	
	
	unsigned int s = 0;
	unsigned int pad = 0;

	int total = 0;
	BYTE* t;
	while (total < out->fnum) {


		//do {
		cclient->GetNextPacketSize(&s);
		//} while (s == 0);

		unsigned int num = s;

		DWORD bflags = 0;

		cclient->GetBuffer(&t, &num, &bflags, NULL, NULL);
		cclient->ReleaseBuffer(num);

		//if (!(!bflags&AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY)) {
			//printf("disc\n");
		//}//hehe wichtig
		client->GetCurrentPadding(&pad);
		out->add(t,(num-pad)*format->nBlockAlign);
		total += (num-pad);
	}
	erg = client->Stop();
	//printf("total:%d\n", total);
	if (FAILED(erg)) {
		printf("client couldnt stop 0x%08x\n", erg);
	}
}

void capture::printFotmat()
{
	printf("Samples per second %d\n", format->nSamplesPerSec);
	printf("Channels: %d\n", format->nChannels);
	printf("Bytes per Frame %d\n", format->nBlockAlign);

}

capture::capture()
{



	HRESULT erg;


	erg = CoInitialize(NULL);
	printf("CoInitialize: 0x%08x\n", erg);

	erg = get_default_device(&device);
	if (FAILED(erg)) {
		printf("couldnt get default device\n");
	}

	erg = device->Activate(
		__uuidof(IAudioClient),
		CLSCTX_ALL, NULL,
		(void**)&client
	);
	printf("IMM Device activate: 0x%08x\n",erg);
	format = new WAVEFORMATEX();
		
	erg= client->GetMixFormat(&format);
	if (FAILED(erg)) {
		printf("IAudioClient::GetMixFormat failed: hr = 0x%08x", erg);
	}

	erg = client->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_LOOPBACK,
		0, 0, format, 0
	);

	erg = client->GetService(
		__uuidof(IAudioCaptureClient),
		(void**)&cclient
	);


	if (FAILED(erg)) {
		printf("IAudioClient::GetService(IAudioCaptureClient) failed: hr = 0x%08x", erg);
	}
	//client->Start();
	
}


capture::~capture()
{
	cclient->Release();
	client->Release();
	device->Release();
	CoUninitialize();
}



//ausgeliehen von loopback capture example
HRESULT get_default_device(IMMDevice **ppMMDevice) {
	HRESULT hr = S_OK;
	IMMDeviceEnumerator *pMMDeviceEnumerator;

	// activate a device enumerator
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(void**)&pMMDeviceEnumerator
	);
	if (FAILED(hr)) {
		printf("CoCreateInstance(IMMDeviceEnumerator) failed: hr = 0x%08x", hr);
		return hr;
	}
	//ReleaseOnExit releaseMMDeviceEnumerator(pMMDeviceEnumerator);

	// get the default render endpoint
	hr = pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, ppMMDevice);
	if (FAILED(hr)) {
		printf("IMMDeviceEnumerator::GetDefaultAudioEndpoint failed: hr = 0x%08x", hr);
		return hr;
	}

	return S_OK;
}
