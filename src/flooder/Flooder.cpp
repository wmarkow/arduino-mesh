/*
 * Flooder.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "Flooder.h"

void Flooder::setRF24Interface(RF24Interface *rf24Interface)
{
	this->rf24Interface = rf24Interface;
}

void Flooder::flood(GenericPacketData* packet)
{
	// do nothing
}


