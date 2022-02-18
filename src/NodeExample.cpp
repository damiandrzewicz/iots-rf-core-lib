#include <Arduino.h>

#include "NodeCore.hpp"

NodeCore nc;

void setup() {
  nc.setup();
}

void loop() {
  nc.loop();
}