#include <Arduino.h>
#include <Wifi.h>
#include <ESP32Servo.h>
#include <Stepper.h>

#define DEBUG
#undef DEBUG

const char *ssid = "TP-Link_7A50";
const char *password = "89741238";
const int stepsPerRev = 2048;

const int blueServoPin = 4;
const int blackServoPin = 15;
const int in1 = 13;
const int in2 = 12;
const int in3 = 14;
const int in4 = 27;

int blueServoPos = 0;
int blackServoPos = 0;

WiFiServer server(80);
Servo blueServo;
Servo blackServo;
Stepper myStepper(stepsPerRev, in1, in3, in2, in4);

TaskHandle_t stepperTaskHandle = NULL;
TaskHandle_t blueServoTaskHandle = NULL;
TaskHandle_t blackServoTaskHandle = NULL;

void stepperTask(void *parameters)
{
   int *stepsAndSpeed = (int *)parameters;
   int steps = stepsAndSpeed[0];
   int speed = stepsAndSpeed[1];

   #ifdef DEBUG
      Serial.println("Running Stepper Thread...");
      Serial.print("Steps: ");
      Serial.println(steps);
      Serial.print("Speed: ");
      Serial.println(speed);
   #endif

   myStepper.setSpeed(speed);
   myStepper.step(steps);

   // kys
   vTaskDelete(NULL);
}

void blueServoTask(void *parameters) 
{
   int *degreesAndSpeed = (int *)parameters;
   int degrees = degreesAndSpeed[0];
   int speed = degreesAndSpeed[1];

   #ifdef DEBUG
      Serial.println("Running Blue Servo Task...");
      Serial.print("Degrees: ");
      Serial.println(degrees);
      Serial.print("Speed: ");
      Serial.println(speed);
   #endif

   if (degrees > blueServoPos)
   {
      for (int i = blueServoPos; i < degrees; i++)
      {
         blueServo.write(i);
         delay(speed);
      }
      blueServoPos = degrees;
   }
   else 
   {
      for (int i = blueServoPos; i > degrees; i--)
      {
         blueServo.write(i);
         delay(speed);
      }
      blueServoPos = degrees;
   }

   // kys
   vTaskDelete(NULL);
}

void blackServoTask(void *parameters)
{
   int *degreesAndSpeed = (int *)parameters;
   int degrees = degreesAndSpeed[0];
   int speed = degreesAndSpeed[1];

   #ifdef DEBUG
      Serial.println("Running Black Servo Task...");
      Serial.print("Degrees: ");
      Serial.println(degrees);
      Serial.print("Speed: ");
      Serial.println(speed);
   #endif

   if (degrees > blackServoPos)
   {
      for (int i = blackServoPos; i < degrees; i++)
      {
         blackServo.write(i);
         delay(speed);
      }
      blackServoPos = degrees;
   }
   else 
   {
      for (int i = blackServoPos; i > degrees; i--)
      {
         blackServo.write(i);
         delay(speed);
      }
      blackServoPos = degrees;
   }

   // kys
   vTaskDelete(NULL);
}

void parseCommand(String command)
{
   // Figure out what the first word of the command is
   String baseCommand;
   for (int i = 0; command[i] != ' '; i++) 
   {
      baseCommand += command[i];
   }

   if (baseCommand == "baseAxis")
   {
      // Figure out the number of steps specified
      String stepsCommand; 
      for (int i = baseCommand.length() + 1; command[i] != ' '; i++)
      {
         stepsCommand += command[i];
      }
      int steps = stepsCommand.toInt();

      // Figure out the speed specified
      String speedCommand;
      for (int i = baseCommand.length() + 1 + stepsCommand.length() + 1; command[i] != '\0'; i++)
      {
         speedCommand += command[i];
      }
      int speed = speedCommand.toInt();

      // bundle arguments 
      static int stepsAndSpeed[] = {steps, speed};

      // Start the task to move hte motor
      xTaskCreate(
         stepperTask, 
         "Stepper Task",
         4096,
         &stepsAndSpeed,
         1,
         &stepperTaskHandle
      );
   }
   else if (baseCommand == "elbowAxis") 
   {
      // Figure out the number of degrees specified
      String degreesCommand;
      for (int i = baseCommand.length() + 1; command[i] != ' '; i++) 
      {
         degreesCommand += command[i];
      }
      int degrees = degreesCommand.toInt();

      // Figure out the speed specified
      String speedCommand;
      for (int i = baseCommand.length() + 1 + degreesCommand.length() + 1; command[i] != '\0'; i++)
      {
         speedCommand += command[i];
      }
      int speed = speedCommand.toInt();

      // bundle arguments
      static int degreesAndSpeed[] = {degrees, speed};

      // Start the task to move the stepper
      xTaskCreate(
         blueServoTask, 
         "Blue Stepper Task",
         4096,
         &degreesAndSpeed,
         1,
         &blueServoTaskHandle
      );
   }
   else if (baseCommand == "pinchAxis") 
   {
      // Figure out the number of degrees specified
      String degreesCommand;
      for (int i = baseCommand.length() + 1; command[i] != ' '; i++) 
      {
         degreesCommand += command[i];
      }
      int degrees = degreesCommand.toInt();

      // Figure out the speed specified
      String speedCommand;
      for (int i = baseCommand.length() + 1 + degreesCommand.length() + 1; command[i] != '\0'; i++)
      {
         speedCommand += command[i];
      }
      int speed = speedCommand.toInt();

      // bundle arguments
      static int degreesAndSpeed[] = {degrees, speed};

      // Start the task to move the stepper
      xTaskCreate(
         blackServoTask, 
         "Black Stepper Task",
         4096,
         &degreesAndSpeed,
         1,
         &blackServoTaskHandle
      );
   }
   else 
   {
      Serial.println("Invalid Commmand Received");
   }
}

void setup()
{
   // Serial init
   Serial.begin(115200);
   while (!Serial)
   {
      ;
   }

   // Wifi init
   Serial.print("Connecting to ");
   Serial.print(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
      Serial.print(".");
      delay(500);
   }
   Serial.println();
   Serial.println("Connection Success");
   Serial.print("Local IP: ");
   Serial.println(WiFi.localIP());

   // Server init
   server.begin();

   // Servo init
   // Allow allocation of all timers
   ESP32PWM::allocateTimer(0);
   ESP32PWM::allocateTimer(1);
   ESP32PWM::allocateTimer(2);
   ESP32PWM::allocateTimer(3);
   blueServo.setPeriodHertz(50);
   blackServo.setPeriodHertz(50);
   blackServo.attach(blackServoPin, 500, 2600);
   blueServo.attach(blueServoPin, 500, 2550);
   blueServo.write(0);
   blackServo.write(0);

   // Stepper init
   myStepper.setSpeed(15);

}

void loop()
{
   WiFiClient client = server.available();
   if (client)
   {
      #ifdef DEBUG
         Serial.println("Client Connected");
      #endif
      while (client.connected())
      {
         // Need to receive instructions as to what motors/servos need to be driven and how
         // Commands should be one of the following (with arguments attached):
         //   - baseAxis steps speed
         //   - elbowAxis degrees speed
         //   - pinchAxis degrees speed

         String command = client.readStringUntil('\n'); // receive the command over the network
         if (command.length() != 0) 
         {
            #ifdef DEBUG
               Serial.println("Received: " + command);
            #endif
            parseCommand(command); 
         }

      }

      client.stop();
      #ifdef DEBUG
         Serial.println("Disconnected from client");
      #endif
   }

   delay(100);
}
