#pragma once
#include <Arduino.h>
#include <driver/ledc.h>

class PWMWrapper {
   private:
    uint8_t pin;
    uint32_t frequency;
    uint8_t resolution;
    static uint8_t nextChannel;
    uint8_t channel;
    uint16_t maxDuty;

   public:
    PWMWrapper(uint32_t f = 312500, uint8_t res = 8)
        : frequency(f), resolution(res), maxDuty((1 << res) - 1) {
        channel = nextChannel++;
    }

    void begin(int pwmpin) {
        pin = pwmpin;
        ledcSetup(channel, frequency, resolution);
        ledcAttachPin(pin, channel);
        ledcWrite(channel, 0);
    }
    void setVoltage(double voltage);
    void setDuty(uint16_t duty) { ledcWrite(channel, duty); }
};

uint8_t PWMWrapper::nextChannel = 0;

void PWMWrapper::setVoltage(double voltage) {
    uint16_t duty = (voltage / 5.0) * maxDuty;
    ledcWrite(channel, duty);
}