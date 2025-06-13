#include <Arduino.h>
#include "wireless.h"
#include "motors.h"

void setup() {
   Serial.begin(115200);
   while (!Serial) {
      ;
   }

   wireless::initializeWireless();
   motors::initializeMotors();
   wireless::runWiFiServer();
}

void loop() {
   vTaskDelete(NULL);
}
