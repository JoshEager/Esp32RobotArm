#include <Arduino.h>
#include "tasks.h"

namespace parser {
    void executeCommand(int axis, int amount, int speed) {
        Serial.print("Moving axis ");
        Serial.print(axis);
        Serial.print(" by ");
        Serial.print(amount); 
        Serial.print(" at speed ");
        Serial.println(speed);

        switch (axis) {
            case (1):
                tasks::moveAxis1(amount, speed);
                break;
            case (2):
                tasks::moveAxis2(amount, speed);
                break;
            case (3):
                tasks::moveAxis3(amount, speed);
                break;
            default:
                Serial.println("Invalid axis!");
        }
    }

    String parseCommand(String command) { 
        String firstPhrase;
        String secondPharse;
        String thirdPhrase;

        // Any command should consist of 3 different phrases, seperated by spaces. Start by getting them
        int firstSpaceIndex = command.indexOf(" ", 0);
        int secondSpaceIndex = command.indexOf(" ", firstSpaceIndex + 1);

        if (firstSpaceIndex == -1 || secondSpaceIndex == -1) {
            return "Invalid command!";
        }

        firstPhrase = command.substring(0, firstSpaceIndex);
        secondPharse = command.substring(firstSpaceIndex + 1, secondSpaceIndex);
        thirdPhrase = command.substring(secondSpaceIndex + 1);

        if (!(firstPhrase.toInt() && secondPharse.toInt() && thirdPhrase.toInt())) {
            return "Invalid command!";
        }

        // Then execute
        executeCommand(firstPhrase.toInt(), secondPharse.toInt(), thirdPhrase.toInt());

        return "ok";
    }
}