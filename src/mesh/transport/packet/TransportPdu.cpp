/*
 * TransportPdu.cpp
 *
 *  Created on: 03.08.2018
 *      Author: wmarkowski
 */

#include <string.h>
#include "TransportPdu.h"

void TransportPdu::setPort(uint8_t port)
{
   this->port = port;
}
uint8_t TransportPdu::getPort()
{
   return this->port;
}

uint8_t TransportPdu::getPayloadLength()
{
   return this->payloadLength;
}

void TransportPdu::setPayload(uint8_t* payload, uint8_t length)
{
   this->payloadLength = length;
   memcpy(this->payload, payload, length);
}
