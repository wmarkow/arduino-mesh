/*
 * Flooder.h
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#ifndef FLOODER_FLOODER_H_
#define FLOODER_FLOODER_H_

#include "FlooderCounters.h"
#include "../interface/RF24Interface.h"

class RF24Interface;

class Flooder
{
private:
	FlooderCounters counters;
	RF24Interface *rf24Interface;

public:
	void setRF24Interface(RF24Interface *rf24Interface);
	void flood(GenericPacketData* packet);
	FlooderCounters* getCounters();
};



#endif /* FLOODER_FLOODER_H_ */
