/*
 * Ifconfig.h
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_IFCONFIG_H_
#define TERMINAL_COMMANDS_IFCONFIG_H_

#include "AbstractCommand.h"
#include "../../mesh/network/interface/Interface.h"

class Ifconfig : public AbstractCommand
{
private:
    MeshNode* meshNodePtr;
	void processIfconfig(HardwareSerial *serial);
	void processIfconfig(HardwareSerial *serial, Interface *interface);
	void processIfconfigUp(HardwareSerial *serial, Interface *interface);
	void processIfconfigDown(HardwareSerial *serial, Interface *interface);
	Interface* getInterfaceByName(String name);
public:
    Ifconfig(MeshNode* meshNodePtr);
	const __FlashStringHelper* getName();
	void process(CommandParams *params, HardwareSerial *serial);
	void processBackground(HardwareSerial *serial);
};

#endif /* TERMINAL_COMMANDS_IFCONFIG_H_ */
