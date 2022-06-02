/*
 * TransportPdu.h
 *
 *  Created on: 03.08.2018
 *      Author: wmarkowski
 */

#ifndef MESH_TRANSPORT_PACKET_TRANSPORTPDU_H_
#define MESH_TRANSPORT_PACKET_TRANSPORTPDU_H_

#include "../../network/packet/core/IotPacketHeader.h"

#define TRANSPORT_PDU_LENGTH DEFAULT_PACKET_PAYLOAD_SIZE - 2

class TransportPdu
{
private:
   uint8_t port;
   uint8_t payloadLength;
   uint8_t payload[TRANSPORT_PDU_LENGTH];

public:
   void setPort(uint8_t port);
   uint8_t getPort();
   uint8_t getPayloadLength();
   void setPayload(uint8_t* payload, uint8_t length);
};

#endif /* MESH_TRANSPORT_PACKET_TRANSPORTPDU_H_ */
