#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "gimbalpin.h"

enum class GimbalState {
    IDLE,           // アイドル状態
    MOTOR_CONTROL,  // モーター制御状態
    MOTOR_SETTING,  // モーター初期位置設定
    LOGGING,        // データロギング状態
    LOW_POWER       // deepsleep
};

enum class CameraState {
    POWER_ON,   // ラズパイ電源オン,カメラロギングオフ
    POWER_OFF,  // ラズパイ電源オフ
    CAMERA_ON   // ラズパイ電源オン、カメラロギングオン
};

class UARTManager {
   private:
    HardwareSerial& RTD;
    HardwareSerial& CAMERA;
    GimbalState gimbalstate = GimbalState::IDLE;
    CameraState camerastate = CameraState::POWER_OFF;

   public:
    UARTManager(HardwareSerial& rtdPort = Serial1, HardwareSerial& cameraPort = Serial2)
        : RTD(rtdPort), CAMERA(cameraPort) {}
    void begin(long boudRateRTD = 115200, long boudRateCAMERA = 115200);
    void update();
};

void UARTManager::begin(long baudRateRTD, long baudRateCAMERA) {
    RTD.begin(baudRateRTD, PIN::RTD_RX, PIN::RTD_TX);
    CAMERA.begin(baudRateCAMERA, PIN::CAM_RX, PIN::CAM_TX);
}

void UARTManager::update() {
    if (RTD.available()) {
        char command = RTD.read();
        switch (command) {
            case 's':
                gimbalstate = GimbalState::IDLE;
                break;
            case 'm':
                gimbalstate = GimbalState::MOTOR_CONTROL;
                break;
            case 't':
                gimbalstate = GimbalState::MOTOR_SETTING;
                break;
            case 'l':
                gimbalstate = GimbalState::LOGGING;
                break;
            case 'u':
                gimbalstate = GimbalState::LOW_POWER;
                break;
            default:
                command = 'e';
                break;
        }
        RTD.printf("%c", command);
    }
    if (CAMERA.available()) {
        char command = CAMERA.read();
        switch (command) {
            case 'p':
                camerastate = CameraState::POWER_ON;
                break;
            case 'o':
                camerastate = CameraState::POWER_OFF;
                break;
            case 'c':
                camerastate = CameraState::CAMERA_ON;
                break;
            default:
                command = 'e';
                break;
        }
        RTD.printf("r%c", command);
    }
}