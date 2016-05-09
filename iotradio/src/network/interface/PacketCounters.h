/*
 * PacketCounters.h
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#ifndef INTERFACE_PACKETCOUNTERS_H_
#define INTERFACE_PACKETCOUNTERS_H_

class PacketCounters
{
private:
	unsigned int transmittedTcpSuccess = 0;
	unsigned int transmittedTcpFailed = 0;

	unsigned int transmittedUdpAck = 0;
	unsigned int transmittedUdpOther = 0;

public:
	void incTransmittedTcpSuccess();
	void incTransmittedTcpFailed();
	void incTransmittedUdpAck();
	void incTransmittedUdpOther();

	unsigned int getTransmittedTcpSuccess();
	unsigned int getTransmittedTcpFailed();
	unsigned int getTransmittedUdpAck();
	unsigned int getTransmittedUdpOther();
};



#endif /* INTERFACE_PACKETCOUNTERS_H_ */
