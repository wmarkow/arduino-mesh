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
#include <RF24Logging.h>
#include <RF24Loggers/ArduinoPrintLogger.h>
#include "../src/mesh/link/RF24Device.h"
#include "../src/mesh/link/HC12Device.h"
#include "../src/mesh/network/node/MeshNode.h"
#include "../src/terminal/commands/FlooderCmd.h"
#include "../src/terminal/commands/Ifconfig.h"
#include "../src/terminal/commands/IpConfig.h"
#include "../src/terminal/commands/Ping.h"
#include "../src/terminal/commands/Wireshark.h"

RF24Device rf24Device;
Interface radioRF24 = Interface(&rf24Device);

HC12Device hc12Device;
Interface radioHC12 = Interface(&hc12Device);

FixedSizeArray<AbstractCommand*, 6> commands;
Array<AbstractCommand*> *commandsArray = &commands;
UptimeCommand uptimeCommand;
Ping pingCommand;
Ifconfig ifconfigCommand;
IpConfig ipConfigCommand;
FlooderCmd flodderCommand;
Wireshark wiresharkCommand;
Terminal terminal(&Serial, commandsArray);

ArduinoPrintLogger serialLogAppender(&Serial);

void setup()
{
   Serial.begin(115200);
   rf24Logging.setHandler(&serialLogAppender);
   RF24Log_info("RF24LogExample", "setup called");

   SPI.begin();

   randomSeed(analogRead(0));

   radioHC12.up();

   commandsArray->add(&uptimeCommand);
   commandsArray->add(&pingCommand);
   commandsArray->add(&ifconfigCommand);
   commandsArray->add(&flodderCommand);
   commandsArray->add(&ipConfigCommand);
   commandsArray->add(&wiresharkCommand);

   LocalMeshNode.setRF24Interface(&radioRF24);
   LocalMeshNode.setHC12Interface(&radioHC12);
   LocalMeshNode.setIpAddress(1);
}

void loop()
{
   terminal.loop();
   LocalMeshNode.loop();
}
