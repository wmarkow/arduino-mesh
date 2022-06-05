/*
 * Ping.h
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_PING_H_
#define TERMINAL_COMMANDS_PING_H_

#include "AbstractCommand.h"
#include "../../mesh/network/node/MeshNode.h"

class Ping : public AbstractCommand
{
private:
	uint8_t pingAddress = 0;
	unsigned long lastCommandExecutionMillis = 0;
	MeshNode* meshNodePtr;
	void processPing(uint8_t address, HardwareSerial* serial);
public:
    Ping(MeshNode* meshNodePtr);
	const __FlashStringHelper* getName();
	void process(CommandParams *params, HardwareSerial *serial);
	void processBackground(HardwareSerial *serial);
};



#endif /* TERMINAL_COMMANDS_PING_H_ */
