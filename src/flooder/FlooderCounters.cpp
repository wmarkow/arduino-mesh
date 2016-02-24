/*
 * PacketCounters.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "FlooderCounters.h"

void FlooderCounters::incDroppedCount()
{
	droppedCount ++;
}
void FlooderCounters::incFloodedCount()
{
	floodedCount ++;
}

unsigned int FlooderCounters::getDroppedCount()
{
	return droppedCount;
}

unsigned int FlooderCounters::getFloodedCount()
{
	return floodedCount;
}
