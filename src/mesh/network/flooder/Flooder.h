/*
 * Flooder.h
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#ifndef FLOODER_FLOODER_H_
#define FLOODER_FLOODER_H_

#include "../../../mesh/network/interface/Interface.h"
#include "MeshNodeCounters.h"

class Flooder
{
private:
   MeshNodeCounters counters;
	Interface *rf24interface;
	Interface *hc12interface;

public:
	Flooder();
	void setRF24Interface(Interface *interface);
	void setHC12Interface(Interface *interface);
	void flood(IotPacket* packet);
	MeshNodeCounters* getCounters();
};



#endif /* FLOODER_FLOODER_H_ */
