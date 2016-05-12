/*
 * PacketCounters.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "../../../mesh/network/interface/PacketCounters.h"

void PacketCounters::incTransmittedTcpSuccess()
{
	transmittedTcpSuccess ++;
}

void PacketCounters::incTransmittedTcpFailed()
{
	transmittedTcpFailed ++;
}

void PacketCounters::incTransmittedUdpAck()
{
	transmittedUdpAck ++;
}

void PacketCounters::incTransmittedUdpOther()
{
	transmittedUdpOther ++;
}

unsigned int PacketCounters::getTransmittedTcpSuccess()
{
	return transmittedTcpSuccess;
}

unsigned int PacketCounters::getTransmittedTcpFailed()
{
	return transmittedTcpFailed;
}

unsigned int PacketCounters::getTransmittedUdpAck()
{
	return transmittedUdpAck;
}

unsigned int PacketCounters::getTransmittedUdpOther()
{
	return transmittedUdpOther;
}
