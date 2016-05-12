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
	performBackgroundCommands();

	// check for incoming commands
	if(!readString())
	{
		return;
	}

	String command = commandParams.getParam(0);

	if(command.equals(F("break")))
	{
		cancelBackgroundCommands();

		commandParams.reset();
		printTerminalReadyIfNeeded();

		return;
	}

	if(areBackgroundCommands())
	{
		commandParams.reset();
		Serial.println(F("terminal busy"));

		return;
	}

	if(command.equals(""))
	{
		commandParams.reset();
		printTerminalReadyIfNeeded();
		return;
	}

	if(command.equals(F("ifconfig")))
	{
		ifconfig.process(&commandParams);

		commandParams.reset();
		printTerminalReadyIfNeeded();
		return;
	}
	if(command.equals(F("ping")))
	{
		ping.process(&commandParams);
		commandParams.reset();
		printTerminalReadyIfNeeded();

		return;
	}
	if(command.equals(F("flooder")))
	{
		flooderCmd.process(&commandParams);

		commandParams.reset();
		printTerminalReadyIfNeeded();
		return;
	}

	Serial.print(command);
	Serial.println(F(": unknown command"));

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
	Serial.print(F("arduino$"));
}

void Terminal::printTerminalReadyIfNeeded()
{
	if(areBackgroundCommands())
	{
		return;
	}

	printTerminalReady();
}

bool Terminal::areBackgroundCommands()
{
	if(ping.isBackground())
	{
		return true;
	}

	return false;
}

void Terminal::performBackgroundCommands()
{
	ping.loopBackgroundIfNeeded();
}

void Terminal::cancelBackgroundCommands()
{
	ping.cancelBackground();
}
