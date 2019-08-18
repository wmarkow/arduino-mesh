/*
 * RF24Device.h
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#ifndef MESH_LINK_RF24DEVICE_H_
#define MESH_LINK_RF24DEVICE_H_

#include <RF24.h>

#include "../../mesh/link/Device.h"
#include "../../mesh/network/packet/core/IotPacket.h"

#if defined(ESP8266)
	#define IOT_HARDWARE_CE_PIN D4
	#define IOT_HARDWARE_CS_PIN D8
#else
#define IOT_HARDWARE_CE_PIN 9
#define IOT_HARDWARE_CS_PIN 10
#endif

#define IOT_ADDRESS_LENGTH 5
#define RF24_DEFAULT_PACKET_SIZE 32

class RF24Device: public Device
{
private:
    RF24 rf24;
    byte linkAddress[IOT_ADDRESS_LENGTH];
    uint8_t paLevelInPercentage;
public:
    RF24Device();
    bool up();
    bool powerDown();
    bool isChipConnected();
    bool readPacket(IotPacket* packet);
    bool writePacket(IotPacket* packet);

    String getLinkAddress();
    int8_t getPALevelInDbm();
    uint16_t getDataRateInKbs();
    uint8_t getPayloadSize();
    uint8_t getRFChannel();
    String getModel();
    String getInterfaceName();

    void setPALevel(uint8_t levelInPercent);
};

#endif /* MESH_LINK_RF24DEVICE_H_ */
