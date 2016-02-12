/*
 * Terminal.cpp
 *
 *  Created on: 8 lut 2016
 *      Author: witek
 */
#include "Terminal.h"

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
		processIfconfigCommand();

		cleanReadBuffer();
		printTerminalReady();
		return;
	}
	if(command.equals("ping"))
	{
		processPingCommand();

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
			endOfWordDetected = true;
			readBuffer[readRowIndex][readColumnIndex] = 0;
			continue;
		}

		if(byte != ' ' && endOfWordDetected)
		{
			// new word detected
			endOfWordDetected = false;

			readRowIndex++;
			readColumnIndex = 0;

			if(readRowIndex >= INCOMING_BUFFER_MAX_WORDS_IN_LINE)
			{
				// incoming buffer overflow
				Serial.flush();
				Serial.println();
				Serial.println("error: to many words per line");
				Serial.flush();
				cleanReadBuffer();

				return false;
			}
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
