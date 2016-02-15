#include "test.h"

#include "src/interface/RF24Interface.h"
#include "src/terminal/Terminal.h"

RF24Interface radio;
Terminal terminal;

void setup() {
  Serial.begin(115200);

  radio.up();
}

void loop() {
	//delay(1000);
	//Serial.println("tu jestem 1");
	terminal.loop();
	radio.loop();
}
