/*
 * PacketCounters.h
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#ifndef INTERFACE_FLOODERCOUNTERS_H_
#define INTERFACE_FLOODERCOUNTERS_H_

class FlooderCounters
{
private:
	unsigned int droppedCount = 0;
	unsigned int floodedCount = 0;

public:
	void incDroppedCount();
	void incFloodedCount();

	unsigned int getDroppedCount();
	unsigned int getFloodedCount();
};



#endif /* INTERFACE_FLOODERCOUNTERS_H_ */
