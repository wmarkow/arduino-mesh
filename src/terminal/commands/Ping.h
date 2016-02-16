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
	void processPing(uint8_t address);
public:
	void process(CommandParams *params);
};



#endif /* TERMINAL_COMMANDS_PING_H_ */
