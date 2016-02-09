#include "test.h"

/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/
#include "IotRadio.h"
#include "Terminal.h"

IotRadio radio;
Terminal terminal;

// Used to control whether this node is sending or receiving
bool role = 0;
uint8_t pingerAddress = 1;
uint8_t ackAddress = 2;

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  //radio.begin();
  //radio.setIpAddress(ackAddress);
}

void loop() {
	//delay(1000);
	//Serial.println("test");
	terminal.loop();

	/****************** Ping Out Role ***************************/
/*	if (role == 1)
	{
		radio.ping(ackAddress);

		// Try again 1s later
		delay(1000);
		Serial.println("tu jestem");
	}

	if(role == 0)
	{
		radio.arduinoLoop();
	}
*/
	/****************** Change Roles via Serial Commands ***************************/
/*
	if ( Serial.available() )
	{
		char c = toupper(Serial.read());
		if ( c == 'T' && role == 0 )
		{
			Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
			role = 1;                  // Become the primary transmitter (ping out)
			radio.setIpAddress(pingerAddress);
		} else if ( c == 'R' && role == 1 )
		{
			Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
			role = 0;                // Become the primary receiver (pong back)
			radio.setIpAddress(ackAddress);
		} else if (c == 'C')
		{
			radio.getCounters()->printCounters();
		}
  }
  */
} // Loop
