/*
 * HC12.cpp
 *
 *  Created on: 27.06.2018
 *      Author: wmarkowski
 */

#include <Arduino.h>
#include "HC12.h"

HC12::HC12() :
      softwareSerial(HC12_TXD_UNO_RX_PIN, HC12_RXD_UNO_TX_PIN)
{

}

void HC12::begin()
{
   softwareSerial.begin(HC12_DEFAULT_BAUDRATE);

   enterTransparentMode();
}

bool HC12::icChipConnected()
{
   // switch to AT mode
   enterCommandMode();
   // send some command
   softwareSerial.print(F("AT"));
   // wait with timeout for response
   String response = getCommandResponse();
   // switch back to direct send mode
   enterTransparentMode();

   if (response.equals(F("OK")))
   {
      return true;
   }

   Serial.println(response);
   return false;
}

void HC12::enterTransparentMode()
{
   pinMode(HC12_SET_PIN, OUTPUT);
   digitalWrite(HC12_SET_PIN, HIGH);

   delay(50);
}

void HC12::enterCommandMode()
{
   pinMode(HC12_SET_PIN, OUTPUT);
   digitalWrite(HC12_SET_PIN, LOW);

   delay(50);
}

String HC12::getCommandResponse()
{
   unsigned long time = millis();
   String resultString = "";

   while (millis() - time < 200)
   {
      if (softwareSerial.available())
      {
         char character = softwareSerial.read();

         if (character == '\n')
         {
            break;
         }

         if (character != '\r')
         {
            resultString += character;
         }

      }
   }

   return resultString;
}
