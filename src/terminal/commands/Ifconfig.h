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
	void processIfconfig(HardwareSerial *serial);
	void processIfconfigUp(HardwareSerial *serial);
	void processIfconfigSetIp(uint8_t address, HardwareSerial *serial);
public:
	const __FlashStringHelper* getName();
	void process(CommandParams *params, HardwareSerial *serial);
	void processBackground(HardwareSerial *serial);
};

#endif /* TERMINAL_COMMANDS_IFCONFIG_H_ */
