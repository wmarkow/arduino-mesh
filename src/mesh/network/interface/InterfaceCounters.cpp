/*
 * InterfaceCounters.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "InterfaceCounters.h"

void InterfaceCounters::incTransmittedTcpSuccess()
{
	transmittedTcpSuccess ++;
}

void InterfaceCounters::incTransmittedTcpFailed()
{
	transmittedTcpFailed ++;
}

void InterfaceCounters::incTransmittedUdpAck()
{
	transmittedUdpAck ++;
}

void InterfaceCounters::incTransmittedUdpOther()
{
	transmittedUdpOther ++;
}

unsigned int InterfaceCounters::getTransmittedTcpSuccess()
{
	return transmittedTcpSuccess;
}

unsigned int InterfaceCounters::getTransmittedTcpFailed()
{
	return transmittedTcpFailed;
}

unsigned int InterfaceCounters::getTransmittedUdpAck()
{
	return transmittedUdpAck;
}

unsigned int InterfaceCounters::getTransmittedUdpOther()
{
	return transmittedUdpOther;
}
