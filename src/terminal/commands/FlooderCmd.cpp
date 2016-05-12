/*
 * FlooderCmd.cpp
 *
 *  Created on: 24 lut 2016
 *      Author: witek
 */

#include "FlooderCmd.h"

#include "../../mesh/network/flooder/Flooder.h"

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
	Serial.print(F("Packets: dropped "));
	Serial.println(flooder.getCounters()->getDroppedCount());

	Serial.print(F("Packets: flooded "));
	Serial.println(flooder.getCounters()->getFloodedCount());
}
