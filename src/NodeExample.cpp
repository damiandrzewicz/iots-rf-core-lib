#if defined(__AVR__)

  #include <Arduino.h>
  #include <ArduinoLog.h>
  #include "Node.hpp"

  Node node;

  void setup() {
    // Initialize UART interface
    Serial.begin(115200);

    // Initialize logger
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    node.setup();
  }

  void loop() {
    node.loop();
  }

#endif