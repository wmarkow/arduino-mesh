/*
 * FlooderCmd.h
 *
 *  Created on: 24 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_FLOODERCMD_H_
#define TERMINAL_COMMANDS_FLOODERCMD_H_

#include <AbstractCommand.h>
#include "../../mesh/network/node/MeshNode.h"

class FlooderCmd : public AbstractCommand
{
private:
	MeshNode* meshNodePtr;
	void processFlooder(HardwareSerial *serial);
public:
    FlooderCmd(MeshNode* meshNodePtr);
	const __FlashStringHelper* getName();
	void process(CommandParams *params, HardwareSerial *serial);
	void processBackground(HardwareSerial *serial);
};


#endif /* TERMINAL_COMMANDS_FLOODERCMD_H_ */
