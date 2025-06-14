#include <Arduino.h>
#include "wireless.h"
#include "motors.h"
#include "tasks.h"

void setup() {
   Serial.begin(115200);
   while (!Serial) {
      ;
   }

   wireless::initializeWireless();
   motors::initializeMotors();
   tasks::initializeTasks();
}

void loop() {
   vTaskDelete(NULL);
}
