#include <Arduino.h>
#include <Wifi.h>
#include <ESP32Servo.h>

const char *ssid = "TP-Link_7A50";
const char *password = "89741238";

WiFiServer server(80);
Servo blueServo;
Servo blackServo;

const int blueServoPin = 4;
const int blackServoPin = 15;

int blueServoPos = 0;
int blackServoPos = 0;

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

  // Servo init. Idk this was just copied and paseted from the library example and modified
  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	blueServo.setPeriodHertz(50);    // standard 50 hz servo
  blackServo.setPeriodHertz(50);
  blackServo.attach(blackServoPin, 500, 2600);
	blueServo.attach(blueServoPin, 500, 2100); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep

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

  // Servo sweeping
  for (blueServoPos = 0; blueServoPos < 180; blueServoPos++) {
    blueServo.write(blueServoPos);
    delay(15);
  }
  for (blueServoPos = 180; blueServoPos > 0; blueServoPos--) {
    blueServo.write(blueServoPos);
    delay(15);
  }

  for (blackServoPos = 0; blackServoPos < 120; blackServoPos++) {
    blackServo.write(blackServoPos);
    delay(15);
  }
  for (blackServoPos = 120; blackServoPos > 0; blackServoPos--) {
    blackServo.write(blackServoPos);
    delay(15);
  }

  delay(100);

}

