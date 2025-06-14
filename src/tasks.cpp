#include <Arduino.h>
#include <motors.h>
#include "wireless.h"
#include "parser.h"
#include "tasks.h"
#include <esp_task_wdt.h>


namespace tasks {
    QueueHandle_t axis1InstructionsQueue = NULL;
    QueueHandle_t axis2InstructionsQueue = NULL;
    QueueHandle_t axis3InstructionsQueue = NULL;


    void axis1Task(void *parameters) {
        // This should run at startup because it constantly listens for commands via a queue
        moveInstructions localInstructions = {"stop", 15};
        while (1) {
            xQueueReceive(axis1InstructionsQueue, &localInstructions, 0);
            if (localInstructions.command != "stop") {
                motors::axis1Stepper.setSpeed(localInstructions.speed);
                if (localInstructions.command == "cw") {
                    motors::axis1Stepper.step(-5);
                } else if (localInstructions.command == "ccw") {
                    motors::axis1Stepper.step(5);
                }
                vTaskDelay((60000 / (localInstructions.speed * config::stepperStepsPerRev)) / portTICK_PERIOD_MS);
            } else {
                vTaskDelay(50 / portTICK_PERIOD_MS);
            }
        }
    }

    void axis2Task(void *parameters) {
        moveInstructions localInstructions = {"stop", 15};
        while (1) {
            xQueueReceive(axis2InstructionsQueue, &localInstructions, 0);
            if (localInstructions.command != "stop") {
                if (localInstructions.command == "cw" && motors::axis2ServoPos > 0) {
                    motors::axis2ServoPos -= 1;
                    motors::axis2Servo.write(motors::axis2ServoPos);
                } else if (localInstructions.command == "ccw" && motors::axis2ServoPos < 180) {
                    motors::axis2ServoPos += 1;
                    motors::axis2Servo.write(motors::axis2ServoPos);
                }
                vTaskDelay(localInstructions.speed);
            } else {
                vTaskDelay(50 / portTICK_PERIOD_MS);
            }
        }
    }

    void axis3Task(void *parameters) {
        moveInstructions localInstructions = {"stop", 15};
        while (1) {
            xQueueReceive(axis3InstructionsQueue, &localInstructions, 0);
            if (localInstructions.command != "stop") {
                if (localInstructions.command == "cw" && motors::axis3ServoPos > 0) {
                    motors::axis3ServoPos -= 1;
                    motors::axis3Servo.write(motors::axis3ServoPos);
                } else if (localInstructions.command == "ccw" && motors::axis3ServoPos < 180) {
                    motors::axis3ServoPos += 1;
                    motors::axis3Servo.write(motors::axis3ServoPos);
                }
                vTaskDelay(localInstructions.speed);
            } else {
                vTaskDelay(50 / portTICK_PERIOD_MS);
            }
        }
    }

    void runWiFiServerTask(void *paramters) {
        while (1) {
            WiFiClient client = wireless::server.available();
            if (client) {
                Serial.println("Connected to client");
                while (client.connected()) {
                    String command = wireless::recieveCommand(&client);
                    if (command.length() != 0) {
                        Serial.println("Received: " + command);
                        String response = parser::parseCommand(command);
                        wireless::sendResponse(&client, response);
                        vTaskDelay(20 / portTICK_PERIOD_MS);
                    } else { // Keepalive packet
                        vTaskDelay(20 / portTICK_PERIOD_MS);
                    }
                }
                client.stop();
                Serial.println("Disconnected from client");
            }

            vTaskDelay(30 / portTICK_PERIOD_MS);
        }
    }

    void initializeTasks(void) {
        // Create queues for intertask communucation   
        axis1InstructionsQueue = xQueueCreate(3, sizeof(struct moveInstructions));
        axis2InstructionsQueue = xQueueCreate(3, sizeof(struct moveInstructions));
        axis3InstructionsQueue = xQueueCreate(3, sizeof(struct moveInstructions));

        // Start all the actual tasks
        xTaskCreatePinnedToCore(
            runWiFiServerTask, 
            "WiFi Server Task",
            8192,
            NULL,
            1,
            NULL,
            0
        );
        
        xTaskCreatePinnedToCore(
            axis1Task, 
            "Axis 1 Task",
            4096,
            NULL,
            1,
            NULL,
            1
        );

        xTaskCreatePinnedToCore(
            axis2Task, 
            "Axis 2 Task",
            4096,
            NULL,
            1,
            NULL,
            1
        );

        xTaskCreatePinnedToCore(
            axis3Task, 
            "Axis 3 Task",
            4096,
            NULL,
            1,
            NULL,
            1
        );
    }
    
}
