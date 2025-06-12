#include "config.h"

namespace config {
    // Wireless
    const char *ssid = "TP-Link_7A50";
    const char *password = "89741238";
    const int port = 80;

    // Motors
    const int stepperStepsPerRev = 2048;
    const int axis2ServoMin = 500;
    const int axis2ServoMax = 2550;
    const int axis3ServoMin = 600;
    const int axis3ServoMax = 2100;

    // Pinouts
    const int IN1 = 13;
    const int IN2 = 12;
    const int IN3 = 14;
    const int IN4 = 27;
    const int axis2ServoPin = 4;
    const int axis3ServoPin = 15;
}