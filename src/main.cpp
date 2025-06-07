#include <Arduino.h>
#include <Wifi.h>

const char *ssid = "TP-Link_7A50";
const char *password = "89741238";

WiFiServer server(80);

void setup() {
  // Serial init
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  // Wifi init
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connection Success");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // Server init
  server.begin();

}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      // Main Loop of the connection
      Serial.println("Connected to Client");
      client.println("Hello from the server!");

      // char buffer[64]; 
      // client.read((uint8_t*)buffer, 64);
      
      String message = client.readStringUntil('\n');
      Serial.println(message);

    }

    client.stop();
    Serial.println("Disconnected from client");
  }

}

