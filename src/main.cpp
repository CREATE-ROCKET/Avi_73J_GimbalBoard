#include <Arduino.h>
#include <driver/pcnt.h>
#include <RotaryEncoder.h>
#include <driver/ledc.h>
#include <motortest.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

motor roll;

void IRAM_ATTR sysad()
{
  roll.sysad();
}

xTaskHandle xlogHandle;
TaskHandle_t taskHandle;
hw_timer_t * timer = NULL;
int tickflag =0;

void IRAM_ATTR tick(){
  tickflag = 1;
}

void setup() 
{
  Serial.begin(9600);
  roll.begin(18, 16, 1000, 8);
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &tick, true);
  timerAlarmWrite(timer, 1000, true); 
  timerAlarmEnable(timer);
}

void loop() 
{
  if(tickflag >0){
    sysad();
    tickflag = 0;
  }
}