/*
 * HC12Example.cpp
 *
 *  Created on: 27.06.2018
 *      Author: wmarkowski
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

#include <Array.h>
#include <FixedSizeArray.h>
#include <Terminal.h>
#include <UptimeCommand.h>
#include "../src/mesh/link/RF24Device.h"
#include "../src/mesh/link/HC12Device.h"
#include "../src/mesh/network/host/MeshNode.h"
#include "../src/terminal/commands/FlooderCmd.h"
#include "../src/terminal/commands/Ifconfig.h"
#include "../src/terminal/commands/IpConfig.h"
#include "../src/terminal/commands/Ping.h"

RF24Device rf24Device;
Interface radioRF24 = Interface(&rf24Device);

HC12Device hc12Device;
Interface radioHC12 = Interface(&hc12Device);

FixedSizeArray<AbstractCommand*, 5> commands;
Array<AbstractCommand*> *commandsArray = &commands;
UptimeCommand uptimeCommand;
Ping pingCommand;
Ifconfig ifconfigCommand;
IpConfig ipConfigCommand;
FlooderCmd flodderCommand;
Terminal terminal(&Serial, commandsArray);

void setup()
{
   Serial.begin(9600);
   _SPI.begin();

   randomSeed(analogRead(0));

   radioHC12.up();

   commandsArray->add(&uptimeCommand);
   commandsArray->add(&pingCommand);
   commandsArray->add(&ifconfigCommand);
   commandsArray->add(&flodderCommand);
   commandsArray->add(&ipConfigCommand);

   LocalMeshNode.setRF24Interface(&radioRF24);
   LocalMeshNode.setHC12Interface(&radioHC12);
   LocalMeshNode.setIpAddress(1);
}

void loop()
{
   terminal.loop();
   LocalMeshNode.loop();
}
