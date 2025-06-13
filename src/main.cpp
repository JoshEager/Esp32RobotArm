#include <Arduino.h>
#include "parser.h"
#include "wireless.h"
#include "motors.h"

void setup() {
   Serial.begin(115200);
   while (!Serial) {
      ;
   }

   wireless::initializeWireless();
   motors::initializeMotors();
}

void loop() {
   WiFiClient client = wireless::server.available();
   if (client) {
      Serial.println("Connected to client");
      while (client.connected()) {
         String command = wireless::recieveCommand(&client);
         if (command.length() != 0) {
            Serial.println("Received: " + command);
            String response = parser::parseCommand(command);
            wireless::sendResponse(&client, response);
         }
      }
      client.stop();
      Serial.println("Disconnected from client");
   }

   vTaskDelay(100 / portTICK_PERIOD_MS);
}
