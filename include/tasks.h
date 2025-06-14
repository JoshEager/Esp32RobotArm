#pragma once
#include <Arduino.h>

namespace tasks {
    struct moveInstructions {
        String command;
        int speed;
    };

    extern QueueHandle_t axis1InstructionsQueue;
    extern QueueHandle_t axis2InstructionsQueue;
    extern QueueHandle_t axis3InstructionsQueue;

    void initializeTasks(void);
}