/*
 * Ifconfig.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ifconfig.h"
#include "../../network/interface/rf24/RF24Interface.h"

extern RF24Interface radio;

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
	Serial.println(radio.getRF24Device()->getModel());
	Serial.print(F("  inet addr     "));
	Serial.println((int)radio.getIpAddress(), DEC);
	Serial.print(F("  HWaddr        "));
	Serial.println(radio.getRF24Device()->getLinkAddress());

	Serial.print(F("  PA level      "));
	switch(radio.getRF24Device()->getPALevel())
	{
		case RF24_PA_MIN:
			Serial.println(F("RF24_PA_MIN (-18dBm)"));
			break;
		case RF24_PA_LOW:
			Serial.println(F("RF24_PA_LOW (-12dBm)"));
			break;
		case RF24_PA_HIGH:
			Serial.println(F("RF24_PA_HIGH (-6dBm)"));
			break;
		case RF24_PA_MAX:
			Serial.println(F("RF24_PA_MAX (0dBm)"));
			break;
		case RF24_PA_ERROR:
			Serial.println(F("RF24_PA_ERROR"));
			break;
		default:
			Serial.println(F("unknown"));
	}

	Serial.print(F("  Data rate     "));
	switch(radio.getRF24Device()->getDataRate())
	{
		case RF24_1MBPS:
			Serial.println(F("RF24_1MBPS"));
			break;
		case RF24_2MBPS:
			Serial.println(F("RF24_2MBPS"));
			break;
		case RF24_250KBPS:
			Serial.println(F("RF24_250KBPS"));
			break;
		default:
			Serial.println(F("unknown"));
			break;
	}

	Serial.print(F("  RF channel    "));
	Serial.println(radio.getRF24Device()->getRFChannel());

	Serial.print(F("  Payload size  "));
	Serial.println(radio.getRF24Device()->getPayloadSize());

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
