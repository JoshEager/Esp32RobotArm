#pragma once 

// Config for compilation options like wireless, motors, etc

namespace config {
    // Wireless
    extern const char *ssid;
    extern const char *password;
    extern const int port;

    // Motors
    extern const int stepperStepsPerRev;
    extern const int axis2ServoMin;
    extern const int axis2ServoMax;
    extern const int axis3ServoMin;
    extern const int axis3ServoMax;

    // Pinouts
    extern const int IN1;
    extern const int IN2;
    extern const int IN3;
    extern const int IN4;
    extern const int axis2ServoPin;
    extern const int axis3ServoPin;
}

