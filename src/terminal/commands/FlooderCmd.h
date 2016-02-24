/*
 * FlooderCmd.h
 *
 *  Created on: 24 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_FLOODERCMD_H_
#define TERMINAL_COMMANDS_FLOODERCMD_H_

#include "AbstractCommand.h"

class FlooderCmd : public AbstractCommand
{
private:
	void processFlooder();
public:
	void process(CommandParams *params);
};


#endif /* TERMINAL_COMMANDS_FLOODERCMD_H_ */
