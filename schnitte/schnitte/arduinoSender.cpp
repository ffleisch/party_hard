#include "arduinoSender.h"

arduinoSender::arduinoSender()
{
	const char* pn = "\\\\.\\COM3";
	arduino = new SerialPort((char*)pn);
	
}

void arduinoSender::sendRGB(int * inp)
{
	char* out=new char[datalen];
	sprintf_s(out,datalen, "R%3d %3d %3d\n", inp[0],inp[1], inp[2]);
	arduino->writeSerialPort(out, datalen);

}


arduinoSender::~arduinoSender()
{
}
