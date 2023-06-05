#include <Arduino.h>
#include <driver/pcnt.h>
#include <RotaryEncoder.h>
#include <driver/ledc.h>
#include <motortest.h>
#include <encoder.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <soc/pcnt_struct.h>

#define sysads
// #define encoder
#ifdef sysads
motor roll;
encoder renc(PCNT_UNIT_0);

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
#define PCNT_H_LIM_VAL INT16_MAX //カウンタの上限
#define PCNT_L_LIM_VAL INT16_MIN //カウンタの下限
int16_t count = 0;
#endif

// static void IRAM_ATTR caller(void *arg);
// int intr_count = 0;
// pcnt_isr_handle_t user_isr_handle = NULL;

// static void IRAM_ATTR caller(void *arg){
//      if (PCNT.status_unit[PCNT_UNIT_0].h_lim_lat) {
//         intr_count++;
//     } else if (PCNT.status_unit[PCNT_UNIT_0].l_lim_lat) {
//         intr_count--;
//     }
//     PCNT.int_clr.val = BIT(PCNT_UNIT_0);
// }

void setup() 
{
  Serial.begin(115200);
  #ifdef sysads
  roll.begin(18, 16, 1024, 10);
  renc.begin(25, 26);
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &tick, true);
  timerAlarmWrite(timer, 1000, true); 
  timerAlarmEnable(timer);
  #endif

  #ifdef encoder
  pcnt_config_t pcnt_config_A;//設定用の構造体の宣言   
        pcnt_config_A.pulse_gpio_num = PULSE_INPUT_PIN;
        pcnt_config_A.ctrl_gpio_num = PULSE_CTRL_PIN;
        pcnt_config_A.lctrl_mode = PCNT_MODE_KEEP;
        pcnt_config_A.hctrl_mode = PCNT_MODE_REVERSE;
        pcnt_config_A.channel = PCNT_CHANNEL_0;
        pcnt_config_A.unit = PCNT_UNIT_0;
        pcnt_config_A.pos_mode = PCNT_COUNT_INC;
        pcnt_config_A.neg_mode = PCNT_COUNT_DEC;
        pcnt_config_A.counter_h_lim = PCNT_H_LIM_VAL;
        pcnt_config_A.counter_l_lim = PCNT_L_LIM_VAL;
  // pcnt_config_t pcnt_config_B;//設定用の構造体の宣言(B相)   
  //       pcnt_config_B.pulse_gpio_num = PULSE_CTRL_PIN;//↓A相用のconfigで設定したのと入れ替える
  //       pcnt_config_B.ctrl_gpio_num = PULSE_INPUT_PIN;//↑A相用のconfigで設定したのと入れ替える
  //       pcnt_config_B.lctrl_mode = PCNT_MODE_REVERSE;//↓A相用のconfigで設定したのと入れ替える
  //       pcnt_config_B.hctrl_mode = PCNT_MODE_KEEP;//↑A相用のconfigで設定したのと入れ替える
  //       pcnt_config_B.channel = PCNT_CHANNEL_1;//チャンネルを1に設定
  //       pcnt_config_B.unit = PCNT_UNIT_0;//UNITはA相と同じものに
  //       pcnt_config_B.pos_mode = PCNT_COUNT_INC;
  //       pcnt_config_B.neg_mode = PCNT_COUNT_DEC;
  //       pcnt_config_B.counter_h_lim = PCNT_H_LIM_VAL;
  //       pcnt_config_B.counter_l_lim = PCNT_L_LIM_VAL;
  // pcnt_unit_config(&pcnt_config_B);
  pcnt_unit_config(&pcnt_config_A);//ユニット初期化
  pcnt_counter_pause(PCNT_UNIT_0);//カウンタ一時停止
  pcnt_counter_clear(PCNT_UNIT_0);//カウンタ初期化
  pcnt_filter_disable(PCNT_UNIT_0);//フィルターを無効化
  pcnt_counter_resume(PCNT_UNIT_0);//カウント開始
  pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_H_LIM);//イベントを有効化
  pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_L_LIM);//イベントを有効化
  pcnt_isr_register(caller, NULL, 0, &user_isr_handle);//割り込みハンドラを登録
  pcnt_intr_enable(PCNT_UNIT_0);//割り込みを有効化
  #endif
}

void loop() 
{
  #ifdef sysads
  if(tickflag >0 && sysflag == 0){
    roll.sysad();
    roll.getpwm();
    eccount = renc.getcount();
    Serial.print(" count= ");
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
  Serial.print("Counter value: ");
  Serial.println(count);
  delay(100);
  #endif
}