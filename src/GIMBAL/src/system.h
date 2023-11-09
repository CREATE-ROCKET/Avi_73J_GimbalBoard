#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum SystemState {
    IDLE,
    MOTOR_CONTROL,
    MOTOR_SETTING,
    LOW_POWER
}

class UARTManager {
   private:
}