#include <Arduino.h>
#include "wireless.h"

void setup() {
   Serial.begin(115200);
   while (!Serial) {
      ;
   }

   wireless::initializeWireless();
}

void loop() {
   WiFiClient client = wireless::server.available();
   if (client) {
      Serial.println("Connected to client");
      while (client.connected()) {
         String command = wireless::recieveCommand(&client);
         if (command.length() != 0) {
            Serial.println("Received: " + command);
            wireless::sendResponse(&client, "Hello from the server!");
         }
      }
      client.stop();
      Serial.println("Disconnected from client");
   }
}