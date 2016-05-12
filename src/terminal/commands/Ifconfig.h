/*
 * Ifconfig.h
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_IFCONFIG_H_
#define TERMINAL_COMMANDS_IFCONFIG_H_

#include "AbstractCommand.h"

class Ifconfig : public AbstractCommand
{
private:
	void processIfconfig();
	void processIfconfigUp();
	void processIfconfigSetIp(uint8_t address);
public:
	void process(CommandParams *params);
};

#endif /* TERMINAL_COMMANDS_IFCONFIG_H_ */
