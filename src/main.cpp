#include <Arduino.h>
#include <driver/pcnt.h>
#include <RotaryEncoder.h>
#include <driver/ledc.h>

#define IN1 18
#define IN2 16
#define PWM_FREQ 256
#define PWM_CHANNEL0 0
#define PWM_CHANNEL1 1
#define PWM_RESOLUTION 8

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  ledcSetup(PWM_CHANNEL0, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN1, PWM_CHANNEL0);
  ledcSetup(PWM_CHANNEL1, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN2, PWM_CHANNEL1);
}

void loop() {
  ledcWrite(PWM_CHANNEL1, 256);
  for (int i = 0; i < 256; i++)
  {
    ledcWrite(PWM_CHANNEL0, i);
    delay(100);
  }
  
  for(int i = 255; i >= 0; i--)
  {
    ledcWrite(PWM_CHANNEL1, i);
    delay(100);
  }
}