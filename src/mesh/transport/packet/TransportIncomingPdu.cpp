/*
 * TransportIncomingPdu.cpp
 *
 *  Created on: 03.08.2018
 *      Author: wmarkowski
 */

#include "TransportIncomingPdu.h"
#include <string.h>

uint8_t TransportIncomingPdu::getSrcAddress()
{
   return srcAddress;
}

void TransportIncomingPdu::setSrcAddress(uint8_t address)
{
   this->srcAddress = address;
}

TransportPdu* TransportIncomingPdu::getTransportPdu()
{
   return &transportPdu;
}

void TransportIncomingPdu::setTransportPdu(TransportPdu* pdu)
{
   memcpy(&transportPdu, pdu, sizeof(TransportPdu));
}
