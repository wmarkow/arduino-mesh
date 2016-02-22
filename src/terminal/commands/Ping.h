/*
 * Ping.h
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_PING_H_
#define TERMINAL_COMMANDS_PING_H_

#include "AbstractCommand.h"

class Ping : public AbstractCommand
{
private:
	uint8_t pingAddress = 0;
	unsigned long lastCommandExecutionMillis = 0;
	void processPing(uint8_t address);
public:
	void process(CommandParams *params);
	void loopBackgroundIfNeeded();
};



#endif /* TERMINAL_COMMANDS_PING_H_ */
