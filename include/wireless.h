#pragma once
#include <WiFi.h>
#include <Arduino.h>

namespace wireless {
    extern WiFiServer server;
    void initializeWireless(void);
    String recieveCommand(WiFiClient *client);
    void sendResponse(WiFiClient *client, String response);
}
