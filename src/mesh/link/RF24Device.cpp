/*
 * RF24Device.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */
#include <RF24.h>
#include <nRF24L01.h>

#include "RF24Device.h"

RF24Device::RF24Device() :
        rf24(RF24(IOT_HARDWARE_CE_PIN, IOT_HARDWARE_CS_PIN))
{
    linkAddress[0] = 0xc1;
    linkAddress[1] = 0xc1;
    linkAddress[2] = 0xc1;
    linkAddress[3] = 0xc1;
    linkAddress[4] = 0xc1;

    paLevelInPercentage = 0;
}

bool RF24Device::up()
{
    if (!rf24.begin())
    {
        return false;
    }

    rf24.powerUp();

    rf24.openWritingPipe(linkAddress);
    rf24.openReadingPipe(1, linkAddress);
    setPALevel(paLevelInPercentage);
    rf24.setDataRate(RF24_250KBPS);
    rf24.setAutoAck(false);
    rf24.setPayloadSize(RF24_DEFAULT_PACKET_SIZE);
    rf24.openWritingPipe(linkAddress);
    rf24.openReadingPipe(1, linkAddress);
    rf24.startListening();

    return isChipConnected();
}

bool RF24Device::powerDown()
{
    rf24.powerDown();

    return true;
}

bool RF24Device::isChipConnected()
{
    return rf24.isChipConnected();
}

bool RF24Device::readPacket(IotPacket* packet)
{
    if (rf24.available() && isChipConnected())
    {
        rf24.read((uint8_t*) packet, DEFAULT_PACKET_SIZE);

        return true;
    }

    return false;
}

bool RF24Device::writePacket(IotPacket* packet)
{
    rf24.stopListening();

    bool result = rf24.write((uint8_t*) packet, DEFAULT_PACKET_SIZE);

    rf24.startListening();

    return result;
}

String RF24Device::getLinkAddress()
{
    String result;

    result.concat(linkAddress[0]);
    result.concat(':');
    result.concat(linkAddress[1]);
    result.concat(':');
    result.concat(linkAddress[2]);
    result.concat(':');
    result.concat(linkAddress[3]);
    result.concat(':');
    result.concat(linkAddress[4]);

    return result;
}

int8_t RF24Device::getPALevelInDbm()
{
    switch (rf24.getPALevel())
    {
    case RF24_PA_MIN:
        return -18;
    case RF24_PA_LOW:
        return -12;
    case RF24_PA_HIGH:
        return -6;
    case RF24_PA_MAX:
        return 0;
    case RF24_PA_ERROR:
    default:
        return -99;
    }
}

uint16_t RF24Device::getDataRateInKbs()
{
    switch (rf24.getDataRate())
    {
    case RF24_1MBPS:
        return 1000;
    case RF24_2MBPS:
        return 2000;
    case RF24_250KBPS:
        return 250;
    default:
        return 0;
    }
}

uint8_t RF24Device::getPayloadSize()
{
    return rf24.getPayloadSize();
}

uint8_t RF24Device::getRFChannel()
{
    return rf24.getChannel();
}

String RF24Device::getModel()
{
    if (rf24.isPVariant())
    {
        return F("nRF24L01+");
    }

    return F("nRF24L01");
}

String RF24Device::getInterfaceName()
{
    return F("rf24");
}

void RF24Device::setPALevel(uint8_t paLevelInPercentage)
{
    this->paLevelInPercentage = paLevelInPercentage;

    if (paLevelInPercentage <= 25)
    {
        rf24.setPALevel(RF24_PA_MIN);
    }
    else if (paLevelInPercentage <= 50)
    {
        rf24.setPALevel(RF24_PA_LOW);
    }
    else if (paLevelInPercentage <= 75)
    {
        rf24.setPALevel(RF24_PA_HIGH);
    }
    else
    {
        rf24.setPALevel(RF24_PA_MAX);
    }
}
