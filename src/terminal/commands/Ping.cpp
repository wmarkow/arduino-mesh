/*
 * Ping.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ping.h"

#include "../../mesh/network/interface/Interface.h"

extern Interface radioRF24;

const __FlashStringHelper* Ping::getName()
{
	return F("ping");
}

void Ping::process(CommandParams *params, HardwareSerial *serial)
{
	if(params->getNumberOfParameters() != 2)
	{
		serial->println(F("error: Usage is ping <address> "));

		return;
	}

	String param = String(params->getParam(1));
	uint8_t address = param.toInt();

	processPing(address, serial);
	this->setBackground(true);
}

void Ping::processPing(uint8_t address, HardwareSerial* serial)
{
	lastCommandExecutionMillis = millis();
	pingAddress = address;

	PingResult pingResult = radioRF24.ping(address);
	if(pingResult.success)
	{
		serial->print(pingResult.packetSize);
		serial->print(F(" bytes from '"));
		serial->print(address);
		serial->print(F("': time= "));
		serial->print(pingResult.timeInUs);
		serial->println(F(" us"));

		return;
	}

	serial->print(F("Destination host '"));
	serial->print(address);
	serial->println(F("' is unreachable."));
}

void Ping::processBackground(HardwareSerial *serial)
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

	processPing(this->pingAddress, serial);
}
