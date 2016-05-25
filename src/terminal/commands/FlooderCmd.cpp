/*
 * FlooderCmd.cpp
 *
 *  Created on: 24 lut 2016
 *      Author: witek
 */

#include "FlooderCmd.h"

#include "../../mesh/network/flooder/Flooder.h"

extern Flooder flooder;

const __FlashStringHelper* FlooderCmd::getName()
{
	return F("flooder");
}

void FlooderCmd::process(CommandParams *params, HardwareSerial *serial)
{
	if(params->getNumberOfParameters() == 1)
	{
		processFlooder(serial);

		return;
	}
}

void FlooderCmd::processBackground(HardwareSerial *serial)
{

}

void FlooderCmd::processFlooder(HardwareSerial *serial)
{
	serial->print(F("Packets: dropped "));
	serial->println(flooder.getCounters()->getDroppedCount());

	serial->print(F("Packets: flooded "));
	serial->println(flooder.getCounters()->getFloodedCount());
}
