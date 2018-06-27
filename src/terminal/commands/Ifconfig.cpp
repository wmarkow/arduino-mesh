/*
 * Ifconfig.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ifconfig.h"

#include "../../mesh/network/interface/Interface.h"

extern Interface radioRF24;

const __FlashStringHelper* Ifconfig::getName()
{
	return F("ifconfig");
}

void Ifconfig::process(CommandParams *params, HardwareSerial *serial)
{
	if(params->getNumberOfParameters() == 1)
	{
		processIfconfig(serial);

		return;
	}

	if(params->getNumberOfParameters() == 3)
	{
		String interface = params->getParam(1);
		if(!interface.equals(F("rf24")))
		{
			serial->print(F("Unknown interface "));
			serial->println(interface);
			return;
		}

		String subcommand = params->getParam(2);
		if(subcommand.equals(F("up")))
		{
			processIfconfigUp(serial);
			return;
		}

		uint8_t address = subcommand.toInt();
		processIfconfigSetIp(address, serial);

		return;
	}

	serial->println(F("Unknown parameters"));
}

void Ifconfig::processBackground(HardwareSerial *serial)
{

}

void Ifconfig::processIfconfig(HardwareSerial *serial)
{
	serial->println(F("rf24"));

	if(!radioRF24.isChipConnected())
	{
		Serial.println(F("    down, chip not connected "));

		return;
	}

	serial->print(F("  Model         "));
	serial->println(radioRF24.getDevice()->getModel());
	serial->print(F("  inet addr     "));
	serial->println((int)radioRF24.getIpAddress(), DEC);
	serial->print(F("  HWaddr        "));
	serial->println(radioRF24.getDevice()->getLinkAddress());

	serial->print(F("  PA level      "));
	serial->print(radioRF24.getDevice()->getPALevelInDbm());
	serial->println(F("dBm"));

	serial->print(F("  Data rate     "));
	serial->print(radioRF24.getDevice()->getDataRateInKbs());
	serial->println(F("kbps"));

	serial->print(F("  RF channel    "));
	serial->println(radioRF24.getDevice()->getRFChannel());

	serial->print(F("  Payload size  "));
	serial->println(radioRF24.getDevice()->getPayloadSize());

	serial->println(F("  TX"));
	serial->print(F("   + TCP success ")); Serial.println(radioRF24.getCounters()->getTransmittedTcpSuccess());
	serial->print(F("   + TCP fail ")); Serial.println(radioRF24.getCounters()->getTransmittedTcpFailed());
	serial->print(F("   + UDP ACK ")); Serial.println(radioRF24.getCounters()->getTransmittedUdpAck());
	serial->print(F("   \\ UDP other ")); Serial.println(radioRF24.getCounters()->getTransmittedUdpOther());
}

void Ifconfig::processIfconfigUp(HardwareSerial *serial)
{
   radioRF24.up();
	processIfconfig(serial);
}

void Ifconfig::processIfconfigSetIp(uint8_t address, HardwareSerial *serial)
{
   radioRF24.setIpAddress(address);
	processIfconfig(serial);
}
