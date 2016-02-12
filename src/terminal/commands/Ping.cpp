/*
 * Ping.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "../Terminal.h"
#include "../../../IotRadio.h"

extern IotRadio radio;

void processPing(uint8_t address);


void Terminal::processPingCommand()
{
	if(this->getNumberOfParameters() != 2)
	{
		Serial.print("error: Usage is ping <address> ");

		return;
	}

	uint8_t address = this->getParameter(1).toInt();

	processPing(address);
}

void processPing(uint8_t address)
{
	if(radio.ping(address))
	{
		Serial.print("Host ");
		Serial.print(address);
		Serial.println(" is alive");

		return;
	}

	Serial.print(F("Destination host '"));
	Serial.print(address);
	Serial.println(F("' is unreachable."));
}
