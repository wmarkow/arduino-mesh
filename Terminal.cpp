/*
 * Terminal.cpp
 *
 *  Created on: 8 lut 2016
 *      Author: witek
 */
#include "Terminal.h"
#include "IotRadio.h"

extern IotRadio radio;

void Terminal::println(String &message) {
	Serial.println(message);
}

void Terminal::loop() {
	// TODO: perform current command


	// check for incoming commands
	if(!readString())
	{
		return;
	}

	String command = String(readBuffer[0]);

	if(state == Busy)
	{
		cleanReadBuffer();
		return;
	}

	if(command.equals(""))
	{
		cleanReadBuffer();
		printTerminalReady();
		return;
	}

	if(command.equals("ifconfig"))
	{
		processIfconfig();

		cleanReadBuffer();
		printTerminalReady();
		return;
	}

	Serial.print(command);
	Serial.println(": unknown command");

	cleanReadBuffer();
	printTerminalReady();
}

bool Terminal::readString()
{
	while(Serial.available())
	{
		char byte = Serial.read();

		if(byte == ' ' && readColumnIndex == 0)
		{
			// skip extra spaces before any words
			continue;
		}

		if(byte == '\r')
		{
			// end of line
			readBuffer[readRowIndex][readColumnIndex] = 0;

			return true;
		}

		if(byte == ' ' && readColumnIndex > 0)
		{
			// end of word in line
			readRowIndex++;
			readColumnIndex = 0;

			if(readRowIndex > INCOMING_BUFFER_MAX_WORDS_IN_LINE - 1)
			{
				// incoming buffer overflow
				Serial.flush();
				Serial.println();
				Serial.println("error: to many words per line");
				Serial.flush();
				cleanReadBuffer();

				return false;
			}

			continue;
		}

		// a regular character of the word
		readBuffer[readRowIndex][readColumnIndex] = byte;
		readColumnIndex ++;

		if(readColumnIndex > INCOMING_BUFFER_MAX_CHARACTERS_IN_WORD)
		{
			// incoming buffer overflow
			Serial.flush();
			Serial.println();
			Serial.println("error: to many characters per word");
			Serial.flush();
			cleanReadBuffer();

			return false;
		}
	}

	return false;
}

void Terminal::cleanReadBuffer()
{
	readRowIndex = 0;
	readColumnIndex = 0;
}

uint8_t Terminal::getNumberOfParameters()
{
	return readRowIndex + 1;
}

String Terminal::getParameter(uint8_t index)
{
	return String(readBuffer[index]);
}

void Terminal::printTerminalReady()
{
	Serial.print("arduino$");
}

void Terminal::processIfconfig()
{
	Serial.println("rf24l01");

	if(!radio.isChipConnected())
	{
		Serial.println("    down, chip not connected ");

		return;
	}

	Serial.print("    inet addr ");
	Serial.println((int)radio.getIpAddress(), DEC);
	Serial.print("    HWaddr    ");
	Serial.println(radio.getLinkAddress());
	Serial.print("    PA level  ");
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

	Serial.print("    Data rate  ");
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

	Serial.print("    RF channel  ");
	Serial.println(radio.getRFChannel());

	Serial.print("    Payload size  ");
	Serial.println(radio.getPayloadSize());
}


