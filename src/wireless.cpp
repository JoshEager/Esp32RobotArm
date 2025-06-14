#include "wireless.h"
#include "config.h"
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

}
