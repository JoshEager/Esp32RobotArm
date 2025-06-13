#pragma once 
#include <Arduino.h>

namespace parser {
    void executeCommand(int axis, int amount, int speed);
    String parseCommand(String command);
}
