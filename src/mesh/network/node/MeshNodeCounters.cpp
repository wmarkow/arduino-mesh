/*
 * MeshNodeCounters.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "MeshNodeCounters.h"

void MeshNodeCounters::incDroppedCount()
{
	droppedCount ++;
}
void MeshNodeCounters::incFloodedCount()
{
	floodedCount ++;
}

unsigned int MeshNodeCounters::getDroppedCount()
{
	return droppedCount;
}

unsigned int MeshNodeCounters::getFloodedCount()
{
	return floodedCount;
}
