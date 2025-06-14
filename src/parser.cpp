#include <Arduino.h>
#include "tasks.h"

namespace parser {
    void executeCommand(int axis, String cmd, int speed) {
        tasks::moveInstructions insToSend = {cmd, speed};    
        switch (axis) {
            case (1):
                xQueueSend(tasks::axis1InstructionsQueue, (void *)&insToSend, 0);
                break;
            case (2):
                xQueueSend(tasks::axis2InstructionsQueue, (void *)&insToSend, 0);
                break;
            case (3):
                xQueueSend(tasks::axis3InstructionsQueue, (void *)&insToSend, 0);
                break;
        }
    }

    String parseCommand(String command) { 
        String firstPhrase;
        String secondPhrase;
        String thirdPhrase;

        // Any command should consist of 3 different phrases, seperated by spaces. Start by getting them
        int firstSpaceIndex = command.indexOf(" ", 0);
        int secondSpaceIndex = command.indexOf(" ", firstSpaceIndex + 1);

        if (firstSpaceIndex == -1 || secondSpaceIndex == -1) {
            return "Invalid command!";
        }

        firstPhrase = command.substring(0, firstSpaceIndex);
        secondPhrase = command.substring(firstSpaceIndex + 1, secondSpaceIndex);
        thirdPhrase = command.substring(secondSpaceIndex + 1);

        if (!(firstPhrase.toInt() && thirdPhrase.toInt() && (secondPhrase == "cw" || secondPhrase == "ccw" || secondPhrase == "stop"))) {
            return "Invalid command!";
        }

        // Then execute
        executeCommand(firstPhrase.toInt(), secondPhrase, thirdPhrase.toInt());

        return "ok";
    }
}