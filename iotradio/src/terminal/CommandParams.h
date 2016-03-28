/*
 * CommandParams.h
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDPARAMS_H_
#define TERMINAL_COMMANDPARAMS_H_

#define INCOMING_BUFFER_MAX_WORDS_IN_LINE 6
#define INCOMING_BUFFER_MAX_CHARACTERS_IN_WORD 12

#include <Arduino.h>

class CommandParams
{
private:
	char readBuffer[INCOMING_BUFFER_MAX_WORDS_IN_LINE][INCOMING_BUFFER_MAX_CHARACTERS_IN_WORD + 1];
	uint8_t readRowIndex = 0;
	uint8_t readColumnIndex = 0;
	bool endOfWordDetected = false;
public:
	String getParam(uint8_t index);
	uint8_t getNumberOfParameters();
	void reset();
	bool appendChar(char data);
};

#endif /* TERMINAL_COMMANDPARAMS_H_ */
