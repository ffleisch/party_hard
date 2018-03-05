#pragma once
#ifndef ARDUINOSENDER_H
#define ARDUINOSENDER_H
#include "SerialPort.h"


class arduinoSender
{
public:
	SerialPort* arduino;
	int datalen = 20;
	arduinoSender();
	void sendRGB(int* inp);
	~arduinoSender();
};
#endif // !ARDUINOSENDER_H

