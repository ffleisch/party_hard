#include "capturePar.h"



capturePar::capturePar()
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
	printf("IMM Device activate: 0x%08x\n", erg);
	format = new WAVEFORMATEX();

	erg = client->GetMixFormat(&format);
	if (FAILED(erg)) {
		printf("IAudioClient::GetMixFormat failed: hr = 0x%08x", erg);
	}


	REFERENCE_TIME hnsRequestedDuration = 10000000;

	erg = client->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_LOOPBACK,
		0, 0, format, 0
	);

	/*erg = client->Initialize(
	AUDCLNT_SHAREMODE_SHARED,
	0,
	hnsRequestedDuration, 0, format, 0
	);*///does not work
	erg = client->GetService(
		__uuidof(IAudioCaptureClient),
		(void**)&cclient
	);


	if (FAILED(erg)) {
		printf("IAudioClient::GetService(IAudioCaptureClient) failed: hr = 0x%08x", erg);
	}
	//client->Start();

	//captureThread = std::thread(this.fillList);

}


capturePar::~capturePar()
{
	cclient->Release();
	client->Release();
	device->Release();
	CoUninitialize();
}


const void capturePar::fillList()
{

	HRESULT erg = 0;
	erg = client->Start();
	BYTE* t;


	if (FAILED(erg)) {
		printf("client couldnt start 0x%08x\n", erg);
	}

	unsigned int s = 0;
	unsigned int pad = 0;

	while (run) {
		while (readingL) {

		}
		writingL = true;
		cclient->GetNextPacketSize(&s);
		unsigned int num = s;

		DWORD bflags = 0;

		cclient->GetBuffer(&t, &num, &bflags, NULL, NULL);
		cclient->ReleaseBuffer(num);
		for (int i = 0; i < num; i++) {
			byteList.push_front(t[i]);
			if (byteList.size() > byteNum) {
				byteList.pop_back();
			}
		}
		//if (!(!bflags&AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY)) {
		//printf("disc\n");
		//}//hehe wichtig
		//client->GetCurrentPadding(&pad);
		writingL = false;
	}
	erg = client->Stop();
	//printf("total:%d\n", total);
	if (FAILED(erg)) {
		printf("client couldnt stop 0x%08x\n", erg);
	}
}

void capturePar::getSnippet(snippet * out)
{
	while (writingL) {

	}
	readingL = true;
	out->reset();
	byteNum = format->nBlockAlign*out->samples;

	int total = 0;
	if (byteList.size() >= byteNum) {
		for (std::list<BYTE>::iterator it = byteList.begin(); (it!=byteList.end())&&(total < out->samples*format->nBlockAlign);++it,total++) {
			out->data[total] = *it;
		}
	}
	else {
		printf("List not large enough\n");
	}
	out->getInts();
	readingL = false;
}

void capturePar::printFormat()
{
	printf("Samples per second %d\n", format->nSamplesPerSec);
	printf("Channels: %d\n", format->nChannels);
	printf("Bytes per Frame %d\n", format->nBlockAlign);

}

//ausgeliehen von loopback capture example
HRESULT capturePar::get_default_device(IMMDevice **ppMMDevice) {
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

