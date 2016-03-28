/*
 * FlooderCmd.cpp
 *
 *  Created on: 24 lut 2016
 *      Author: witek
 */

#include "FlooderCmd.h"
#include "../../flooder/Flooder.h"

extern Flooder flooder;

void FlooderCmd::process(CommandParams *params)
{
	if(params->getNumberOfParameters() == 1)
	{
		processFlooder();

		return;
	}
}

void FlooderCmd::processFlooder()
{
	Serial.print("Packets: dropped ");
	Serial.println(flooder.getCounters()->getDroppedCount());

	Serial.print("Packets: flooded ");
	Serial.println(flooder.getCounters()->getFloodedCount());
}
