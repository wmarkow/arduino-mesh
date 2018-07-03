/*
 * RF24Device.h
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#ifndef MESH_LINK_HC12DEVICE_H_
#define MESH_LINK_HC12DEVICE_H_

#include "HC12.h"

#include "../../mesh/link/Device.h"
#include "../../mesh/network/packet/core/IotPacket.h"

#define HC12_PREAMBLE 0b01010101
/***
 * With serial port speed 9600 bps it take around 1ms to send one bit
 */
#define HC12_OUT_OF_SYNC_TIMEOUT_IN_MILLIS 1000

enum  HC12ReceiverState
{
   HC12_RECEIVE_WAIT_FOR_PREAMBLE,
   HC12_RECEIVE_WAIT_FOR_PAYLOAD
};

class HC12Device : public Device
{
private:
   HC12 hc12;
   bool chipConnected = false;
   uint8_t receivedPacket[DEFAULT_PACKET_SIZE];
   uint8_t receiverState = HC12_RECEIVE_WAIT_FOR_PREAMBLE;
   uint8_t receiverIndex = 0;
   unsigned long lastReceivedByteInMillis = 0;
   void resetReceiver();
public:
	HC12Device();
	bool up();
	bool powerDown();
	bool isChipConnected();
	bool available();
	bool readPacket(IotPacket* data);
	bool writePacket(IotPacket* packet);

	String getLinkAddress();
	int8_t getPALevelInDbm();
	uint16_t getDataRateInKbs();
	uint8_t getPayloadSize();
	uint8_t getRFChannel();
	String getModel();
	String getInterfaceName();
};

#endif /* MESH_LINK_HC12DEVICE_H_ */
