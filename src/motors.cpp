#include "motors.h"
#include "config.h"
#include <ESP32Servo.h>
#include <Stepper.h>

namespace motors {
    Stepper axis1Stepper(config::stepperStepsPerRev, config::IN1, config::IN3, config::IN2, config::IN4);
    Servo axis2Servo;
    Servo axis3Servo;

    int axis2ServoPos = 0;
    int axis3ServoPos = 0;

    void initializeMotors(void) {
        axis1Stepper.setSpeed(1);

        axis2Servo.attach(config::axis2ServoPin, config::axis2ServoMin, config::axis2ServoMax);
        axis3Servo.attach(config::axis3ServoPin, config::axis3ServoMin, config::axis3ServoMax);

        axis2Servo.write(0);
        axis3Servo.write(0);
    }
}