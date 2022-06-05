/*
 * Wireshark.cpp
 *
 *  Created on: 05.08.2018
 *      Author: wmarkowski
 */

#include "Wireshark.h"

#include "../../mesh/network/node/MeshNode.h"

Wireshark::Wireshark(MeshNode* meshNodePtr)
{
   this->meshNodePtr = meshNodePtr;
}

const __FlashStringHelper* Wireshark::getName()
{
   return F("wireshark");
}

void Wireshark::process(CommandParams *params, HardwareSerial *serial)
{
   if (params->getNumberOfParameters() == 2)
   {
      String command = params->getParam(1);
      if (command.equals("on"))
      {
         meshNodePtr->setWiresharkEnabled(true);
      }
      else
      {
         meshNodePtr->setWiresharkEnabled(false);
      }

      return;
   }

   serial->println(F("Unknown parameters"));
}

void Wireshark::processBackground(HardwareSerial *serial)
{

}
