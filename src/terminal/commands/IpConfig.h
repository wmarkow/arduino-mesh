/*
 * Ifconfig.h
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_IPCONFIG_H_
#define TERMINAL_COMMANDS_IPCONFIG_H_

#include "AbstractCommand.h"
#include "../../mesh/network/interface/Interface.h"

class IpConfig : public AbstractCommand
{
private:
	void processIpConfig(HardwareSerial *serial);
	void processIpConfigSetIp(uint8_t address, HardwareSerial *serial);
public:
	const __FlashStringHelper* getName();
	void process(CommandParams *params, HardwareSerial *serial);
	void processBackground(HardwareSerial *serial);
};

#endif /* TERMINAL_COMMANDS_IPCONFIG_H_ */
