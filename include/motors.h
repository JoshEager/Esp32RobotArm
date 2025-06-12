#pragma once
#include "config.h"
#include <ESP32Servo.h>
#include <Stepper.h>

namespace motors {
    extern Stepper axis1Stepper;
    extern Servo axis2Servo;
    extern Servo axis3Servo;
    extern int axis2ServoPos;
    extern int axis3ServoPos;

    void initializeMotors(void);
}
