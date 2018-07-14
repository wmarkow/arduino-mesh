/*
 * Flooder.h
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#ifndef FLOODER_FLOODER_H_
#define FLOODER_FLOODER_H_

#include "../../../mesh/network/flooder/FlooderCounters.h"
#include "../../../mesh/network/interface/Interface.h"

class Interface;

class Flooder
{
private:
	FlooderCounters counters;
	Interface *rf24interface;
	Interface *hc12interface;

public:
	Flooder();
	void setRF24Interface(Interface *interface);
	void setHC12Interface(Interface *interface);
	void flood(IotPacket* packet);
	FlooderCounters* getCounters();
};



#endif /* FLOODER_FLOODER_H_ */
