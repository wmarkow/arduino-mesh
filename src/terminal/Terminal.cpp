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

	String command = commandParams.getParam(0);

	if(state == Busy)
	{
		commandParams.reset();
		return;
	}

	if(command.equals(""))
	{
		commandParams.reset();
		printTerminalReady();
		return;
	}

	if(command.equals("ifconfig"))
	{
		ifconfig.process(&commandParams);

		commandParams.reset();
		printTerminalReady();
		return;
	}
	if(command.equals("ping"))
	{
		ping.process(&commandParams);

		commandParams.reset();
		printTerminalReady();
		return;
	}

	Serial.print(command);
	Serial.println(": unknown command");

	commandParams.reset();
	printTerminalReady();
}

bool Terminal::readString()
{
	while(Serial.available())
	{
		char byte = Serial.read();

		if(commandParams.appendChar(byte))
		{
			return true;
		}
	}

	return false;
}

void Terminal::printTerminalReady()
{
	Serial.print("arduino$");
}
