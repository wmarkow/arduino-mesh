/*
 * Terminal.h
 *
 *  Created on: 8 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <Arduino.h>

#define INCOMING_BUFFER_MAX_WORDS_IN_LINE 6
#define INCOMING_BUFFER_MAX_CHARACTERS_IN_WORD 12

typedef enum TerminalState
{
	Ready,
	Busy
};

class Terminal {

private:
	TerminalState state = Ready;
	char readBuffer[INCOMING_BUFFER_MAX_WORDS_IN_LINE][INCOMING_BUFFER_MAX_CHARACTERS_IN_WORD + 1];
	uint8_t readRowIndex = 0;
	uint8_t readColumnIndex = 0;
	bool endOfWordDetected = false;
	void printTerminalReady();
	uint8_t getNumberOfParameters();
	String getParameter(uint8_t index);
	void processIfconfigCommand();
	void processIfconfig();
	void processIfconfigUp();
	void cleanReadBuffer();
public:
	void println(String &message);
	void loop();
	bool readString();
};



#endif /* TERMINAL_H_ */
