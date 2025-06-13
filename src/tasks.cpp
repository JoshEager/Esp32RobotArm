#include <Arduino.h>
#include <motors.h>
#include <malloc.h>

namespace tasks {
    TaskHandle_t axis1MoveTaskHandle = NULL;
    TaskHandle_t axis2MoveTaskHandle = NULL;
    TaskHandle_t axis3MoveTaskHandle = NULL;
    

    void axis1MoveTask(void *parameters) {
        int *amountAndSpeed = (int *)parameters;
        int amount = amountAndSpeed[0];
        int speed = amountAndSpeed[1];

        motors::axis1Stepper.setSpeed(speed);
        motors::axis1Stepper.step(amount);

        free(amountAndSpeed);
        axis1MoveTaskHandle = NULL;
        vTaskDelete(NULL);
    }

    void axis2MoveTask(void *parameters) {
        int *amountAndSpeed = (int *)parameters;
        int amount = amountAndSpeed[0];
        int speed = amountAndSpeed[1];

        if (motors::axis2ServoPos < amount) {
            for (int i = motors::axis2ServoPos; i < amount; i++) {
                motors::axis2ServoPos = i;
                motors::axis2Servo.write(i);
                vTaskDelay(speed / portTICK_PERIOD_MS);
            }
        } else {
            for (int i = motors::axis2ServoPos; i > amount; i--) {
                motors::axis2ServoPos = i;
                motors::axis2Servo.write(i);
                vTaskDelay(speed / portTICK_PERIOD_MS);      
            }
        }

        free(amountAndSpeed);
        axis2MoveTaskHandle = NULL;
        vTaskDelete(NULL);
    }

    void axis3MoveTask(void *parameters) {
        int *amountAndSpeed = (int *)parameters;
        int amount = amountAndSpeed[0];
        int speed = amountAndSpeed[1];

        if (motors::axis3ServoPos < amount) {
            for (int i = motors::axis3ServoPos; i < amount; i++) {
                motors::axis3ServoPos = i;
                motors::axis3Servo.write(i);
                vTaskDelay(speed / portTICK_PERIOD_MS);
            }
        } else {
            for (int i = motors::axis3ServoPos; i > amount; i--) {
                motors::axis3ServoPos = i;
                motors::axis3Servo.write(i);
                vTaskDelay(speed / portTICK_PERIOD_MS);      
            }
        }

        free(amountAndSpeed);
        axis3MoveTaskHandle = NULL;
        vTaskDelete(NULL);
    }
    
    // Non-blocking, general purpose move function for axis 1
    void moveAxis1(int amount, int speed) {
        if (axis1MoveTaskHandle != NULL) {
            return;
        }
        
        int *amountAndSpeed = (int *)malloc(2 * sizeof(int));
        amountAndSpeed[0] = amount;
        amountAndSpeed[1] = speed;

        xTaskCreatePinnedToCore(
            axis1MoveTask, 
            "Axis 1 Move Task",
            2048,
            amountAndSpeed,
            1, 
            &axis1MoveTaskHandle, 
            1
        );
    }

    void moveAxis2(int amount, int speed) {
        if (axis2MoveTaskHandle != NULL) {
            return;
        }
        
        int *amountAndSpeed = (int *)malloc(2 * sizeof(int));
        amountAndSpeed[0] = amount;
        amountAndSpeed[1] = speed;

        xTaskCreatePinnedToCore(
            axis2MoveTask, 
            "Axis 2 Move Task",
            2048,
            amountAndSpeed, 
            1, 
            &axis2MoveTaskHandle, 
            1
        );
    }

    void moveAxis3(int amount, int speed) {
        if (axis3MoveTaskHandle != NULL) {
            return;
        }
        
        int *amountAndSpeed = (int *)malloc(2 * sizeof(int));
        amountAndSpeed[0] = amount;
        amountAndSpeed[1] = speed;

        xTaskCreatePinnedToCore(
            axis3MoveTask, 
            "Axis 3 Move Task",
            2048,
            amountAndSpeed, 
            1, 
            &axis3MoveTaskHandle,
            1
        );
    }
    
}
