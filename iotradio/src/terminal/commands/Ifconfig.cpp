/*
 * Ifconfig.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ifconfig.h"
#include "../../interface/RF24Interface.h"

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
		if(!interface.equals("rf24"))
		{
			Serial.print("Unknown interface ");
			Serial.println(interface);
			return;
		}

		String subcommand = params->getParam(2);
		if(subcommand.equals("up"))
		{
			processIfconfigUp();
			return;
		}

		uint8_t address = subcommand.toInt();
		processIfconfigSetIp(address);

		return;
	}

	Serial.println("Unknown parameters");
}

void Ifconfig::processIfconfig()
{
	Serial.println("rf24");

	if(!radio.isChipConnected())
	{
		Serial.println("    down, chip not connected ");

		return;
	}

	Serial.print("  Model         ");
	Serial.println(radio.getModel());
	Serial.print("  inet addr     ");
	Serial.println((int)radio.getIpAddress(), DEC);
	Serial.print("  HWaddr        ");
	Serial.println(radio.getLinkAddress());

	Serial.print("  PA level      ");
	switch(radio.getPALevel())
	{
		case RF24_PA_MIN:
			Serial.println("RF24_PA_MIN (-18dBm)");
			break;
		case RF24_PA_LOW:
			Serial.println("RF24_PA_LOW (-12dBm)");
			break;
		case RF24_PA_HIGH:
			Serial.println("RF24_PA_HIGH (-6dBm)");
			break;
		case RF24_PA_MAX:
			Serial.println("RF24_PA_MAX (0dBm)");
			break;
		case RF24_PA_ERROR:
			Serial.println("RF24_PA_ERROR");
			break;
		default:
			Serial.println("unknown");
	}

	Serial.print("  Data rate     ");
	switch(radio.getDataRate())
	{
		case RF24_1MBPS:
			Serial.println("RF24_1MBPS");
			break;
		case RF24_2MBPS:
			Serial.println("RF24_2MBPS");
			break;
		case RF24_250KBPS:
			Serial.println("RF24_250KBPS");
			break;
		default:
			Serial.println("unknown");
			break;
	}

	Serial.print("  RF channel    ");
	Serial.println(radio.getRFChannel());

	Serial.print("  Payload size  ");
	Serial.println(radio.getPayloadSize());

	Serial.println("  TX");
	Serial.print("   + TCP success "); Serial.println(radio.getCounters()->getTransmittedTcpSuccess());
	Serial.print("   + TCP fail "); Serial.println(radio.getCounters()->getTransmittedTcpFailed());
	Serial.print("   + UDP ACK "); Serial.println(radio.getCounters()->getTransmittedUdpAck());
	Serial.print("   \\ UDP other "); Serial.println(radio.getCounters()->getTransmittedUdpOther());
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
