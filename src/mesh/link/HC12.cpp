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
   enterCommandMode();
   softwareSerial.print(F("AT"));
   String response = getCommandResponse();
   enterTransparentMode();

   if (response.equals(F("OK")))
   {
      return true;
   }

   Serial.println(response);
   return false;
}

int8_t HC12::getTransmitterPowerInDbm()
{
   enterCommandMode();
   softwareSerial.print(F("AT+RP"));
   String response = getCommandResponse();
   enterTransparentMode();

   if (!response.startsWith(F("OK+RP:")))
   {
      return -99;
   }

   String valueAsString = response.substring(6, response.indexOf(F("dBm")));
   return valueAsString.toInt();
}

uint16_t HC12::getDataRateInKbs()
{
   enterCommandMode();
   softwareSerial.print(F("AT+RB"));
   String response = getCommandResponse();
   enterTransparentMode();

   if (!response.startsWith(F("OK+B")))
   {
      return 0;
   }

   String valueAsString = response.substring(4);
   long int uartBaudRate = valueAsString.toInt();
   if (uartBaudRate <= 2400)
   {
      return 5;
   }
   if (uartBaudRate <= 9600)
   {
      return 15;
   }
   if (uartBaudRate <= 38400)
   {
      return 58;
   }

   return 236;
}

uint8_t HC12::getRFChannel()
{
   enterCommandMode();
   softwareSerial.print(F("AT+RC"));
   String response = getCommandResponse();
   enterTransparentMode();

   if (!response.startsWith(F("OK+RC")))
   {
      return 0;
   }

   String valueAsString = response.substring(5);
   return valueAsString.toInt();
}

bool HC12::write(uint8_t* data, uint8_t size)
{
   softwareSerial.write((char*) data, size);
}

uint8_t HC12::available()
{
   return softwareSerial.available();
}

int HC12::read(uint8_t *buffer, size_t length)
{
   return softwareSerial.readBytes(buffer, length);
}

int HC12::read()
{
   return softwareSerial.read();
}

int HC12::write(uint8_t data)
{
   return softwareSerial.write(data);
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
