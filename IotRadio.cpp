/*
 * IotRadio.cpp
 *
 *  Created on: 25 lip 2015
 *      Author: witek
 */

#include "IotRadio.h"

IotRadio::IotRadio() : radio(RF24(IOT_HARDWARE_CE_PIN, IOT_HARDWARE_CS_PIN)) {
	myAddress[0] = 0xc1;
	myAddress[1] = 0xc1;
	myAddress[2] = 0xc1;
	myAddress[3] = 0xc1;
	myAddress[4] = 0xc1;
}

bool IotRadio::begin()
{
	if(radio.begin()){
		Serial.println("Chip connected");
	}else{
		Serial.println("Chip disconnected");

		return false;
	}

	radio.setPALevel(RF24_PA_MAX);
	radio.setAutoAck(true);

	return true;
}

void IotRadio::setMyAddress(byte address[IOT_ADDRESS_LENGTH])
{
	memcpy(myAddress, address, sizeof(byte[IOT_ADDRESS_LENGTH]));
}

void IotRadio::openDirectChannel(byte otherAddress[IOT_ADDRESS_LENGTH])
{
	radio.openWritingPipe(otherAddress);
	radio.openReadingPipe(1, myAddress);
}

void IotRadio::startListening()
{
	radio.startListening();
}

void IotRadio::stopListening()
{
	radio.stopListening();
}

bool IotRadio::ping()
{
	radio.stopListening();                                    // First, stop listening so we can talk.

	Serial.println(F("Now sending"));

	unsigned long time = micros();                             // Take the time, and send it.  This will block until complete
	if (!radio.write( &time, sizeof(unsigned long) )){
		Serial.println(F("failed"));
		return false;
	}

	radio.startListening();                                    // Now, continue listening

	unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
	boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

	while ( ! radio.available() ){                             // While nothing is received
		if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
			timeout = true;
			break;
		}
	}

	if ( timeout ){                                             // Describe the results
		Serial.println(F("Failed, response timed out."));
		return false;
	}else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long time = micros();

        // Spew it
        Serial.print(F("Sent "));
        Serial.print(time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(time-got_time);
        Serial.println(F(" microseconds"));
    }

	return true;
}

void IotRadio::pong()
{
	unsigned long got_time;

	    if( radio.available()){
	                                                                    // Variable for the received timestamp
	      while (radio.available()) {                                   // While there is data ready
	        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
	      }

	      radio.stopListening();                                        // First, stop listening so we can talk
	      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.
	      radio.startListening();                                       // Now, resume listening so we catch the next packets.
	      Serial.print(F("Sent response "));
	      Serial.println(got_time);
	    }
}

