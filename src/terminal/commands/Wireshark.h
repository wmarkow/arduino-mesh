/*
 * Wireshark.h
 *
 *  Created on: 05.08.2018
 *      Author: wmarkowski
 */

#ifndef TERMINAL_COMMANDS_WIRESHARK_H_
#define TERMINAL_COMMANDS_WIRESHARK_H_

#include "AbstractCommand.h"
#include "../../mesh/network/interface/Interface.h"

class Wireshark : public AbstractCommand
{
public:
   const __FlashStringHelper* getName();
   void process(CommandParams *params, HardwareSerial *serial);
   void processBackground(HardwareSerial *serial);
};

#endif /* TERMINAL_COMMANDS_WIRESHARK_H_ */
