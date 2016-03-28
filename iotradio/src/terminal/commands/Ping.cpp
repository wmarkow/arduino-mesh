/*
 * Ping.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ping.h"
#include "../../interface/RF24Interface.h"

extern RF24Interface radio;

void Ping::process(CommandParams *params)
{
	if(params->getNumberOfParameters() != 2)
	{
		Serial.println("error: Usage is ping <address> ");

		return;
	}

	uint8_t address = params->getParam(1).toInt();

	processPing(address);
	this->setBackground(true);
}

void Ping::processPing(uint8_t address)
{
	lastCommandExecutionMillis = millis();
	pingAddress = address;

	PingResult pingResult = radio.ping(address);
	if(pingResult.success)
	{
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

void Ping::loopBackgroundIfNeeded()
{
	if(!this->isBackground())
	{
		return;
	}

	unsigned long now = millis();

	if(now < lastCommandExecutionMillis + 1000)
	{
		return;
	}

	processPing(this->pingAddress);
}
