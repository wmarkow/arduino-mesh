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
}

uint8_t Host::getIpAddress()
{
   return ipAddress;
}

void Host::setIpAddress(uint8_t ipAddress)
{
   this->ipAddress = ipAddress;
}

Host Localhost;
