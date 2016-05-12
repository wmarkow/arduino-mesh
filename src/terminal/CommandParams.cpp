/*
 * CommandParams.cpp
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#include "CommandParams.h"

String CommandParams::getParam(uint8_t index)
{
	return String(readBuffer[index]);
}

void CommandParams::reset()
{
	readRowIndex = 0;
	readColumnIndex = 0;
}

uint8_t CommandParams::getNumberOfParameters()
{
	return readRowIndex + 1;
}

bool CommandParams::appendChar(char byte)
{
	if(byte == ' ' && readColumnIndex == 0)
	{
		// skip extra spaces before any words
		return false;
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
		return false;
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
			Serial.println(F("error: to many words per line"));
			Serial.flush();
			reset();

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
		Serial.println(F("error: to many characters per word"));
		Serial.flush();
		reset();

		return false;
	}

	return false;
}
