/*
 * Flooder.h
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#ifndef FLOODER_FLOODER_H_
#define FLOODER_FLOODER_H_

#include "../interface/RF24Interface.h"

class RF24Interface;

class Flooder
{
private:
	RF24Interface *rf24Interface;

public:
	void setRF24Interface(RF24Interface *rf24Interface);
	void flood(GenericPacketData* packet);
};



#endif /* FLOODER_FLOODER_H_ */
