#include <Arduino.h>
#include <Magenta_lib.h>

#include "SPI.h"
#include "Wire.h"

void setup() {
	Serial.begin(9600);
  	magentaobj.init();
}


void loop() {
	magentaobj.read_io();
	delay(100);
}