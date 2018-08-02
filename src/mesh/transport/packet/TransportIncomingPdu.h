/*
 * TransportPacket.h
 *
 *  Created on: 16 cze 2016
 *      Author: witek
 */

#ifndef LIBRARIES_IOTRADIO_SRC_MESH_TRANSPORT_PACKET_INCOMINGTRANSPORTPACKET_H_
#define LIBRARIES_IOTRADIO_SRC_MESH_TRANSPORT_PACKET_INCOMINGTRANSPORTPACKET_H_

#include "../../network/packet/core/IotPacketHeader.h"
#include "TransportPdu.h"

struct TransportIncomingPdu
{
private:
	uint8_t srcAddress;
	TransportPdu transportPdu;

public:
	uint8_t getSrcAddress();
	void setSrcAddress(uint8_t);
	TransportPdu* getTransportPdu();
	void setTransportPdu(TransportPdu* pdu);
};

#endif /* LIBRARIES_IOTRADIO_SRC_MESH_TRANSPORT_PACKET_INCOMINGTRANSPORTPACKET_H_ */
