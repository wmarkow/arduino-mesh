/*
 * Ifconfig.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ifconfig.h"

#include "../../mesh/network/interface/Interface.h"

extern Interface radio;

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

	if(!radio.isChipConnected())
	{
		Serial.println(F("    down, chip not connected "));

		return;
	}

	serial->print(F("  Model         "));
	serial->println(radio.getDevice()->getModel());
	serial->print(F("  inet addr     "));
	serial->println((int)radio.getIpAddress(), DEC);
	serial->print(F("  HWaddr        "));
	serial->println(radio.getDevice()->getLinkAddress());

	serial->print(F("  PA level      "));
	serial->print(radio.getDevice()->getPALevelInDbm());
	serial->println(F("dBm"));

	serial->print(F("  Data rate     "));
	serial->print(radio.getDevice()->getDataRateInKbs());
	serial->println(F("kbps"));

	serial->print(F("  RF channel    "));
	serial->println(radio.getDevice()->getRFChannel());

	serial->print(F("  Payload size  "));
	serial->println(radio.getDevice()->getPayloadSize());

	serial->println(F("  TX"));
	serial->print(F("   + TCP success ")); Serial.println(radio.getCounters()->getTransmittedTcpSuccess());
	serial->print(F("   + TCP fail ")); Serial.println(radio.getCounters()->getTransmittedTcpFailed());
	serial->print(F("   + UDP ACK ")); Serial.println(radio.getCounters()->getTransmittedUdpAck());
	serial->print(F("   \\ UDP other ")); Serial.println(radio.getCounters()->getTransmittedUdpOther());
}

void Ifconfig::processIfconfigUp(HardwareSerial *serial)
{
	radio.up();
	processIfconfig(serial);
}

void Ifconfig::processIfconfigSetIp(uint8_t address, HardwareSerial *serial)
{
	radio.setIpAddress(address);
	processIfconfig(serial);
}
