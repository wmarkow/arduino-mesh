/*
 * Host.cpp
 *
 *  Created on: 15.07.2018
 *      Author: wmarkowski
 */

#include "Host.h"

Host::Host()
{
   ipAddress = 1;
   rf24interface = NULL;
   hc12interface = NULL;
}

uint8_t Host::getIpAddress()
{
   return ipAddress;
}

void Host::setIpAddress(uint8_t ipAddress)
{
   this->ipAddress = ipAddress;
}

void Host::setRF24Interface(Interface *interface)
{
   this->rf24interface = interface;
   this->flooder.setRF24Interface(interface);
   interface->setFlooder(&flooder);
}

void Host::setHC12Interface(Interface *interface)
{
   this->hc12interface = interface;
   this->flooder.setHC12Interface(interface);
   interface->setFlooder(&flooder);
}

Flooder* Host::getFlooder()
{
   return &flooder;
}

Host Localhost;
