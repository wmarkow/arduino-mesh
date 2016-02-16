/*
 * AbstractCommand.h
 *
 *  Created on: 16 lut 2016
 *      Author: witek
 */

#ifndef TERMINAL_COMMANDS_ABSTRACTCOMMAND_H_
#define TERMINAL_COMMANDS_ABSTRACTCOMMAND_H_

#include <Arduino.h>
#include "../CommandParams.h"

class AbstractCommand
{
public:
	virtual void process(CommandParams *params);
};



#endif /* TERMINAL_COMMANDS_ABSTRACTCOMMAND_H_ */
