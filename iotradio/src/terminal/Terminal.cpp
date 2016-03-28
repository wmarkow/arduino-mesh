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

	if(command.equals("break"))
	{
		cancelBackgroundCommands();

		commandParams.reset();
		printTerminalReadyIfNeeded();

		return;
	}

	if(areBackgroundCommands())
	{
		commandParams.reset();
		Serial.println("terminal busy");

		return;
	}

	if(command.equals(""))
	{
		commandParams.reset();
		printTerminalReadyIfNeeded();
		return;
	}

	if(command.equals("ifconfig"))
	{
		ifconfig.process(&commandParams);

		commandParams.reset();
		printTerminalReadyIfNeeded();
		return;
	}
	if(command.equals("ping"))
	{
		ping.process(&commandParams);
		commandParams.reset();
		printTerminalReadyIfNeeded();

		return;
	}
	if(command.equals("flooder"))
	{
		flooderCmd.process(&commandParams);

		commandParams.reset();
		printTerminalReadyIfNeeded();
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
