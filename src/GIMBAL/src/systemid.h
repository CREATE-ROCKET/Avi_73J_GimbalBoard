#pragma once
#include <Arduino.h>
#include <driver/pcnt.h>
#include <soc/pcnt_struct.h>

#include "PWMwrapper.h"
#include "gimbalpin.h"

#define GEAR (32 * 33 * 35 * 38) / (float)(15 * 14 * 13 * 10)
#undef PI
#define PULSE 12.0
#define PI 3.141592653589793f

uint8_t tickflag = 0;

static void IRAM_ATTR caller(void *arg);
int intr_count = 0;
pcnt_isr_handle_t user_isr_handle = NULL;
static void IRAM_ATTR caller(void *arg) {
    if (PCNT.status_unit[PCNT_UNIT_0].h_lim_lat) {
        intr_count++;
    } else if (PCNT.status_unit[PCNT_UNIT_0].l_lim_lat) {
        intr_count--;
    }
    PCNT.int_clr.val = BIT(PCNT_UNIT_0);
}

class COUNTER {
   private:
    pcnt_config_t pcnt_config_A, pcnt_config_B;

   public:
    int16_t count = 0;
    void begin(int pinA, int pinB);
    int16_t getcount();
};
void COUNTER::begin(int pinA, int pinB) {
    pcnt_config_A.pulse_gpio_num = pinA;  // 入力ピン(A)
    pcnt_config_A.ctrl_gpio_num = pinB;   // 制御ピン(B)
    pcnt_config_A.lctrl_mode = PCNT_MODE_KEEP;  // 制御ピンがlowのときの振る舞い
    pcnt_config_A.hctrl_mode = PCNT_MODE_REVERSE;
    pcnt_config_A.channel = PCNT_CHANNEL_0;
    pcnt_config_A.unit = PCNT_UNIT_0;
    pcnt_config_A.pos_mode =
        PCNT_COUNT_INC;  // 入力ピンが立ち上がり時の振る舞い
    pcnt_config_A.neg_mode = PCNT_COUNT_DEC;
    pcnt_config_A.counter_h_lim = INT16_MAX;
    pcnt_config_A.counter_l_lim = INT16_MIN;

    pcnt_config_B.pulse_gpio_num = pinB;
    pcnt_config_B.ctrl_gpio_num = pinA;
    pcnt_config_B.lctrl_mode = PCNT_MODE_REVERSE;
    pcnt_config_B.hctrl_mode = PCNT_MODE_KEEP;
    pcnt_config_B.channel = PCNT_CHANNEL_1;
    pcnt_config_B.unit = PCNT_UNIT_0;
    pcnt_config_B.pos_mode =
        PCNT_COUNT_INC;  // 入力ピンが立ち上がり時の振る舞い
    pcnt_config_B.neg_mode = PCNT_COUNT_DEC;
    pcnt_config_B.counter_h_lim = INT16_MAX;
    pcnt_config_B.counter_l_lim = INT16_MIN;
    pcnt_unit_config(&pcnt_config_B);
    pcnt_unit_config(&pcnt_config_A);
    pcnt_counter_pause(PCNT_UNIT_0);
    pcnt_counter_clear(PCNT_UNIT_0);
    pcnt_filter_disable(PCNT_UNIT_0);  // フィルターを無効化
    pcnt_counter_resume(PCNT_UNIT_0);  // カウント開始
    pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_H_LIM);
    pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_L_LIM);
    pcnt_isr_register(caller, NULL, 0, &user_isr_handle);
    pcnt_intr_enable(PCNT_UNIT_0);
}
int16_t COUNTER::getcount() {
    int16_t countrw;
    // int count = 0;
    pcnt_get_counter_value(PCNT_UNIT_0, &countrw);
    // count = intr_count * INT16_MAX + count;
    return countrw;
}

class Systemid {
   private:
    const uint8_t pulse = 12;
    const double sampling_time = 0.001;
    hw_timer_t *timer = NULL;
    uint32_t last_time = 0;
    int16_t count;           // encoderのカウント値
    int16_t last_count = 0;  // 前回のカウント値
    float omega;             // 角速度
    float u;                 // 入力電圧
    float t = 0;             // 時間
    float total_time = 0;    // 経過時間
    float f = 0.1;           // 周波数
    int16_t u_pwm = 0;       // 入力電圧のPWM値

   public:
    IRAM_ATTR void begin();
    IRAM_ATTR void loop();
    IRAM_ATTR float calculate_omega();

    PWMWrapper pwm1;
    PWMWrapper pwm2;
    COUNTER encoder1;
};

IRAM_ATTR void tick() { tickflag = 1; }

void Systemid::begin() {
    Serial.begin(115200);
    pwm1.begin(PIN::P_PWM_CW);
    pwm2.begin(PIN::P_PWM_CCW);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, tick, true);
    delay(1000);
    Serial.printf("timer set\n");
    timerAlarmWrite(timer, 1000, true);
    timerAlarmEnable(timer);
    encoder1.begin(4, 16);
}

void Systemid::loop() {
    if (tickflag > 0) {
        tickflag = 0;
        count = encoder1.getcount();  // エンコーダのカウント値を取得
        calculate_omega();            // 角速度を計算
        u = sin(2 * PI * f * t);      // 入力電圧を計算
        u_pwm = (int16_t)(u * 1023);  // 入力電圧をPWM値に変換
        t += sampling_time;           // 時間を更新
        total_time += sampling_time;  // 経過時間を更新
        if (u_pwm >= 0) {
            pwm1.setDuty(u_pwm);
            pwm2.setDuty(0);
        } else {
            pwm1.setDuty(0);
            pwm2.setDuty(-u_pwm);
        }
        Serial.printf("%f,%f,%f,%d\n", total_time, u * 5, omega, count);
        if (t > 1 / f) {
            t = 0;
            if (f < 1)
                f += 0.1;
            else if (f < 10)
                f += 1;
            else if (f < 100)
                f += 10;
        }
        if (f >= 100) {
            pwm1.setDuty(0);
            pwm2.setDuty(0);
            timerAlarmDisable(timer);
            timerDetachInterrupt(timer);
            Serial.print("finish");
        }
    }
}

float Systemid::calculate_omega() {
    omega =
        (count - last_count) * 2 * PI / (float)(sampling_time * PULSE * GEAR);
    last_count = count;
    return omega;
}