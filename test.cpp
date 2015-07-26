#include "test.h"

/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/
#include "IotRadio.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
//bool radioNumber = 0; //uno
bool radioNumber = 1 ;//yun

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
/**********************************************************/

byte transmitterAddress[5] = {0xc1, 0xc1, 0xc1, 0xc1, 0xc1};
byte receiverAddress[5] ={0xc2, 0xc1, 0xc1, 0xc1, 0xc1};

IotRadio radio(receiverAddress);

// Used to control whether this node is sending or receiving
bool role = 0;
bool chipConnetced = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  radio.begin();
  radio.openDirectChannel(transmitterAddress);

  // Start the radio listening for data
  radio.startListening();
}

void loop() {


	/****************** Ping Out Role ***************************/
	if (role == 1)
	{
		radio.ping();

		// Try again 1s later
		delay(1000);
		Serial.println("tu jestem");
	}


	/****************** Pong Back Role ***************************/
	if ( role == 0 )
	{
		radio.pong();
	}

	/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
      radio.setMyAddress(transmitterAddress);
      radio.openDirectChannel(receiverAddress);
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
       role = 0;                // Become the primary receiver (pong back)
       radio.setMyAddress(receiverAddress);
       radio.openDirectChannel(transmitterAddress);
       radio.startListening();
    }
  }


} // Loop
