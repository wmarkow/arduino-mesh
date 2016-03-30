#include "test.h"

#include "src/interface/RF24Interface.h"
#include "src/terminal/Terminal.h"

RF24Interface radio;
Flooder flooder;

Terminal terminal;

void setup() {
  Serial.begin(115200);

  randomSeed(analogRead(0));

  radio.setFlooder(&flooder);
  flooder.setRF24Interface(&radio);

  radio.up();
}

void loop() {
	terminal.loop();
	radio.loop();
}
