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

   // Test axis1
   motors::axis1Stepper.setSpeed(15);
   motors::axis1Stepper.step(2048 / 4);
   motors::axis1Stepper.step(-2048 / 4);

   // Test axis2
   for (int i = 0; i < 180; i++) {
      motors::axis2ServoPos = i;
      motors::axis2Servo.write(i);
      vTaskDelay(15 / portTICK_PERIOD_MS);
   }
   for (int i = 180; i > 0; i--) {
      motors::axis2ServoPos = i;
      motors::axis2Servo.write(i);
      vTaskDelay(15 / portTICK_PERIOD_MS);
   }

   // Test axis3
   for (int i = 0; i < 180; i++) {
      motors::axis3ServoPos = i;
      motors::axis3Servo.write(i);
      vTaskDelay(15 / portTICK_PERIOD_MS);
   }
   for (int i = 180; i > 0; i--) {
      motors::axis3ServoPos = i;
      motors::axis3Servo.write(i);
      vTaskDelay(15 / portTICK_PERIOD_MS);
   }

   vTaskDelay(1000 / portTICK_PERIOD_MS);
}
