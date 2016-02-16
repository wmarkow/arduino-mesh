/*
 * Terminal.h
 *
 *  Created on: 8 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <Arduino.h>

#include "CommandParams.h"
#include "commands/Ifconfig.h"
#include "commands/Ping.h"

typedef enum TerminalState
{
	Ready,
	Busy
};

class Terminal {

private:
	TerminalState state = Ready;
	CommandParams commandParams;

	/* commands */
	Ifconfig ifconfig;
	Ping ping;

	void printTerminalReady();
public:
	void println(String &message);
	void loop();
	bool readString();
};



#endif /* TERMINAL_H_ */
