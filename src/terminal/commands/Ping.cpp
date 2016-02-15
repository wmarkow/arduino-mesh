/*
 * Ping.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "../Terminal.h"
#include "../../interface/RF24Interface.h"

extern RF24Interface radio;

void processPing(uint8_t address);


void Terminal::processPingCommand()
{
	if(this->getNumberOfParameters() != 2)
	{
		Serial.println("error: Usage is ping <address> ");

		return;
	}

	uint8_t address = this->getParameter(1).toInt();

	processPing(address);
}

void processPing(uint8_t address)
{
	PingResult pingResult = radio.ping(address);
	if(pingResult.success)
	{
		Serial.print("Host ");
		Serial.print(address);
		Serial.println(" is alive");

		Serial.print(pingResult.packetSize);
		Serial.print(F(" bytes from '"));
		Serial.print(address);
		Serial.print(F("': time= "));
		Serial.print(pingResult.timeInUs);
		Serial.println(F(" us"));

		return;
	}

	Serial.print(F("Destination host '"));
	Serial.print(address);
	Serial.println(F("' is unreachable."));
}
