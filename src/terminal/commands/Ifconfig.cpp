/*
 * Ifconfig.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "../Terminal.h"
#include "../../../IotRadio.h"

extern IotRadio radio;

void processIfconfig();
void processIfconfigUp();
void processIfconfigSetIp(uint8_t address);

void Terminal::processIfconfigCommand()
{
	if(this->getNumberOfParameters() == 1)
	{
		processIfconfig();

		return;
	}

	if(this->getNumberOfParameters() == 3)
	{
		String interface = String(readBuffer[1]);
		if(!interface.equals("rf24"))
		{
			Serial.print("Unknown interface ");
			Serial.println(interface);
			return;
		}

		String subcommand = String(readBuffer[2]);
		if(subcommand.equals("up"))
		{
			processIfconfigUp();
			return;
		}

		uint8_t address = atoi(readBuffer[2]);
		processIfconfigSetIp(address);

		return;
	}

	Serial.println("Unknown parameters");
}

void processIfconfig()
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
}

void processIfconfigUp()
{
	radio.begin();
	processIfconfig();
}

void processIfconfigSetIp(uint8_t address)
{
	radio.setIpAddress(address);
	processIfconfig();
}
