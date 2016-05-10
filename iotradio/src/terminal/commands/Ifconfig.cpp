/*
 * Ifconfig.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ifconfig.h"

#include "../../network/interface/rf24/Interface.h"

extern Interface radio;

void Ifconfig::process(CommandParams *params)
{
	if(params->getNumberOfParameters() == 1)
	{
		processIfconfig();

		return;
	}

	if(params->getNumberOfParameters() == 3)
	{
		String interface = params->getParam(1);
		if(!interface.equals(F("rf24")))
		{
			Serial.print(F("Unknown interface "));
			Serial.println(interface);
			return;
		}

		String subcommand = params->getParam(2);
		if(subcommand.equals(F("up")))
		{
			processIfconfigUp();
			return;
		}

		uint8_t address = subcommand.toInt();
		processIfconfigSetIp(address);

		return;
	}

	Serial.println(F("Unknown parameters"));
}

void Ifconfig::processIfconfig()
{
	Serial.println(F("rf24"));

	if(!radio.isChipConnected())
	{
		Serial.println(F("    down, chip not connected "));

		return;
	}

	Serial.print(F("  Model         "));
	Serial.println(radio.getDevice()->getModel());
	Serial.print(F("  inet addr     "));
	Serial.println((int)radio.getIpAddress(), DEC);
	Serial.print(F("  HWaddr        "));
	Serial.println(radio.getDevice()->getLinkAddress());

	Serial.print(F("  PA level      "));
	Serial.print(radio.getDevice()->getPALevelInDbm());
	Serial.println(F("dBm"));

	Serial.print(F("  Data rate     "));
	Serial.print(radio.getDevice()->getDataRateInKbs());
	Serial.println(F("kbps"));

	Serial.print(F("  RF channel    "));
	Serial.println(radio.getDevice()->getRFChannel());

	Serial.print(F("  Payload size  "));
	Serial.println(radio.getDevice()->getPayloadSize());

	Serial.println(F("  TX"));
	Serial.print(F("   + TCP success ")); Serial.println(radio.getCounters()->getTransmittedTcpSuccess());
	Serial.print(F("   + TCP fail ")); Serial.println(radio.getCounters()->getTransmittedTcpFailed());
	Serial.print(F("   + UDP ACK ")); Serial.println(radio.getCounters()->getTransmittedUdpAck());
	Serial.print(F("   \\ UDP other ")); Serial.println(radio.getCounters()->getTransmittedUdpOther());
}

void Ifconfig::processIfconfigUp()
{
	radio.up();
	processIfconfig();
}

void Ifconfig::processIfconfigSetIp(uint8_t address)
{
	radio.setIpAddress(address);
	processIfconfig();
}
