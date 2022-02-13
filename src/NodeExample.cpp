#include <Arduino.h>

#include "NodeCore.hpp"

void setup() {
  // put your setup code here, to run once:

  NodeCore nc;
  nc.test();
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(1000);
}