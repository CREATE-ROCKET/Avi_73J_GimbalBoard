#pragma once
#include "PWMwrapper.h"

class MotorDriver {
private:
    PWMWrapper cwpwm;
    PWMWrapper ccwpwm;

public:
    MotorDriver(uint32_t f = 312500, uint8_t res = 8)
        : cwpwm(f, res), ccwpwm(f, res) {}
    void begin(uint8_t cwpin, uint8_t ccwpin);
    void setRotation(double voltage);
    void brake();
}; 
void MotorDriver::begin(uint8_t cwpin, uint8_t ccwpin) {
    cwpwm.begin(cwpin);
    ccwpwm.begin(ccwpin);
}

void MotorDriver::setRotation(double voltage) {
    if (voltage > 5) voltage = 5;
    else if (voltage < -5) voltage = -5;
    if (voltage > 0) {
        cwpwm.setVoltage(voltage);
        ccwpwm.setVoltage(0);
    } else if (voltage < 0) {
        cwpwm.setVoltage(0);
        ccwpwm.setVoltage(-voltage);
    } 
} 

void MotorDriver::brake() {
    cwpwm.setVoltage(5);
    ccwpwm.setVoltage(5);
}
