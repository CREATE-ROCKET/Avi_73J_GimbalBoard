#include <Arduino.h>
#include <driver/pcnt.h>
#include <RotaryEncoder.h>
#include <driver/ledc.h>
#include <motortest.h>
//#include <encoder.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>



//#define sysad
#define encoder
#ifdef sysad
motor roll;
encoder renc;


void IRAM_ATTR sysad(){
  roll.sysad();
}
// xTaskHandle xlogHandle;
// TaskHandle_t taskHandle;
hw_timer_t * timer = NULL;
int tickflag =0;
int sysflag = 0;

int16_t eccount;

void IRAM_ATTR tick(){
  tickflag = 1;
}
#endif

#ifdef encoder
#define PULSE_INPUT_PIN 4 //パルスの入力ピン 今回はエンコーダのA相を接続
#define PULSE_CTRL_PIN 5 //制御ピン 今回はエンコーダのB相を接続
#define PCNT_H_LIM_VAL INT16_MAX //カウンタの上限 今回は使ってない
#define PCNT_L_LIM_VAL INT16_MIN //カウンタの下限 今回は使ってない
int16_t count = 0;
#endif

void setup() 
{
  Serial.begin(115200);
  #ifdef sysad
  roll.begin(18, 16, 1024, 10);
  renc.begin(25);
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &tick, true);
  timerAlarmWrite(timer, 1000, true); 
  timerAlarmEnable(timer);
  #endif

  #ifdef encoder
    pcnt_config_t pcnt_config_A;//設定用の構造体の宣言   
        pcnt_config_A.pulse_gpio_num = PULSE_INPUT_PIN;
        pcnt_config_A.ctrl_gpio_num = PULSE_CTRL_PIN;
        pcnt_config_A.lctrl_mode = PCNT_MODE_REVERSE;
        pcnt_config_A.hctrl_mode = PCNT_MODE_KEEP;
        pcnt_config_A.channel = PCNT_CHANNEL_0;
        pcnt_config_A.unit = PCNT_UNIT_0;
        pcnt_config_A.pos_mode = PCNT_COUNT_INC;
        pcnt_config_A.neg_mode = PCNT_COUNT_DEC;
        pcnt_config_A.counter_h_lim = PCNT_H_LIM_VAL;
        pcnt_config_A.counter_l_lim = PCNT_L_LIM_VAL;
  pcnt_config_t pcnt_config_B;//設定用の構造体の宣言(B相)   
        pcnt_config_B.pulse_gpio_num = PULSE_CTRL_PIN;//↓A相用のconfigで設定したのと入れ替える
        pcnt_config_B.ctrl_gpio_num = PULSE_INPUT_PIN;//↑A相用のconfigで設定したのと入れ替える
        pcnt_config_B.lctrl_mode = PCNT_MODE_KEEP;//↓A相用のconfigで設定したのと入れ替える
        pcnt_config_B.hctrl_mode = PCNT_MODE_REVERSE;//↑A相用のconfigで設定したのと入れ替える
        pcnt_config_B.channel = PCNT_CHANNEL_1;//チャンネルを1に設定
        pcnt_config_B.unit = PCNT_UNIT_0;//UNITはA相と同じものに
        pcnt_config_B.pos_mode = PCNT_COUNT_INC;
        pcnt_config_B.neg_mode = PCNT_COUNT_DEC;
        pcnt_config_B.counter_h_lim = PCNT_H_LIM_VAL;
        pcnt_config_B.counter_l_lim = PCNT_L_LIM_VAL;
  pcnt_unit_config(&pcnt_config_B);
  pcnt_unit_config(&pcnt_config_A);//ユニット初期化
  pcnt_counter_pause(PCNT_UNIT_0);//カウンタ一時停止
  pcnt_counter_clear(PCNT_UNIT_0);//カウンタ初期化
  pcnt_counter_resume(PCNT_UNIT_0);//カウント開始
  #endif
}

void loop() 
{
  #ifdef sysad
  if(tickflag >0 && sysflag == 0){
    roll.sysad();
    roll.getpwm();
    eccount = renc.getcount();
    Serial.print(" count=");
    Serial.println(eccount);
    tickflag = 0;
  }
  if (sysflag == 1)
  {
    Serial.println("finish");
    sysflag = 0;
    delay(1000);
  }
  #endif
  #ifdef encoder
  pcnt_get_counter_value(PCNT_UNIT_0, &count);
  if(count > INT8_MAX) pcnt_counter_clear(PCNT_UNIT_0);
  if(count < INT8_MIN) pcnt_counter_clear(PCNT_UNIT_0);
  Serial.print("Counter value: ");
  Serial.println(count);
  #endif
}