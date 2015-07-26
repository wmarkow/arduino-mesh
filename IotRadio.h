/*
 * IotRadio.h
 *
 *  Created on: 25 lip 2015
 *      Author: witek
 */

#ifndef IOTRADIO_H_
#define IOTRADIO_H_

#include <SPI.h>
#include <RF24.h>


#define IOT_HARDWARE_CE_PIN 7
#define IOT_HARDWARE_CS_PIN 8
#define IOT_ADDRESS_LENGTH 5


class IotRadio
{
  private:
	byte myAddress[IOT_ADDRESS_LENGTH];
	RF24 radio;

  public:
	IotRadio(byte address[IOT_ADDRESS_LENGTH]);
	bool begin();
	void setMyAddress(byte address[IOT_ADDRESS_LENGTH]);
    void openDirectChannel(byte otherAddress[IOT_ADDRESS_LENGTH]);
    void startListening();
    void stopListening();
    bool ping();
    void pong();
};




#endif /* IOTRADIO_H_ */
