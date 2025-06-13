#include "wireless.h"
#include "config.h"
#include "parser.h"
#include <Arduino.h>
#include <WiFi.h>


namespace wireless {
    WiFiServer server(config::port);

    void initializeWireless(void) {
        Serial.print("Connecting to ");
        Serial.print(config::ssid);

        const int timeoutSeconds = 5;
        int seconds = 0;
        WiFi.begin(config::ssid, config::password);
        while (WiFi.status() != WL_CONNECTED) {
            if (seconds < timeoutSeconds) {
                Serial.print(".");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                seconds++;
            } else {
                Serial.println();
                Serial.println("Failed to connect to WiFi!");
                return;
            }
        }

        Serial.println();
        Serial.println("Connection Success!");
        Serial.print("Local IP: ");
        Serial.println(WiFi.localIP());

        server.begin();
    }

    String recieveCommand(WiFiClient *client) {
        String command = client->readStringUntil('\n');
        return command;
    }

    void sendResponse(WiFiClient *client, String response) {
        client->println(response);
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
                        float parseBeginTime = millis();
                        String response = parser::parseCommand(command);
                        float parseEndTime = millis();
                        wireless::sendResponse(&client, response);
                        Serial.print("Parsed command in ");
                        Serial.print(parseEndTime - parseBeginTime);
                        Serial.println(" ms");
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

    void runWiFiServer(void) {
        xTaskCreatePinnedToCore(
            runWiFiServerTask,
            "WiFi Server Task",
            8192,
            NULL,
            1,
            NULL,
            0
        );
    }
}
