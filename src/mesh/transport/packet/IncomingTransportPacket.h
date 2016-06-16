/*
 * TransportPacket.h
 *
 *  Created on: 16 cze 2016
 *      Author: witek
 */

#ifndef LIBRARIES_IOTRADIO_SRC_MESH_TRANSPORT_PACKET_INCOMINGTRANSPORTPACKET_H_
#define LIBRARIES_IOTRADIO_SRC_MESH_TRANSPORT_PACKET_INCOMINGTRANSPORTPACKET_H_

#include "../../network/packet/core/IotPacketHeader.h"

struct IncomingTransportPacket
{
	uint8_t srcAddress;
	uint8_t payload[DEFAULT_PACKET_PAYLOAD_SIZE];
};

#endif /* LIBRARIES_IOTRADIO_SRC_MESH_TRANSPORT_PACKET_INCOMINGTRANSPORTPACKET_H_ */
