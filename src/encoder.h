#pragma once

#include <Arduino.h>
#include <driver/pcnt.h>
#include <soc/pcnt_struct.h>

// static void IRAM_ATTR caller(void *arg);
// int intr_count = 0;
// pcnt_isr_handle_t user_isr_handle = NULL;

class encoder
{
    private:
        int intr_count = 0;
        pcnt_isr_handle_t user_isr_handle = NULL;
        pcnt_unit_t pcnt_unit;
    public:
        encoder(pcnt_unit_t unit) : pcnt_unit(unit) {}
        void begin(int pinA, int pinB);
        int32_t getcount();
        static void IRAM_ATTR caller(void *arg);
        void processInterrupt();
};

void encoder::begin(int pinA, int pinB)
{
    pcnt_config_t pcnt_config_A = {
        .pulse_gpio_num = pinA,
        .ctrl_gpio_num = pinB,
        .lctrl_mode = PCNT_MODE_KEEP,
        .hctrl_mode = PCNT_MODE_REVERSE,
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DEC,
        .counter_h_lim = INT16_MAX,
        .counter_l_lim = INT16_MIN,
        .unit = pcnt_unit,
        .channel = PCNT_CHANNEL_0,
    };   
    pcnt_config_t pcnt_config_B = {
        .pulse_gpio_num = pinB,//↓A相用のconfigで設定したのと入れ替える
        .ctrl_gpio_num = pinA,//↑A相用のconfigで設定したのと入れ替える
        .lctrl_mode = PCNT_MODE_REVERSE,//↓A相用のconfigで設定したのと入れ替える
        .hctrl_mode = PCNT_MODE_KEEP,//↑A相用のconfigで設定したのと入れ替える
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DEC,
        .counter_h_lim = INT16_MAX,
        .counter_l_lim = INT16_MIN,                
        .unit = pcnt_unit,//UNITはA相と同じものに        
        .channel = PCNT_CHANNEL_1,//チャンネルを1に設定
    };
    pcnt_unit_config(&pcnt_config_B);
    pcnt_unit_config(&pcnt_config_A);
    pcnt_counter_pause(pcnt_unit);
    pcnt_counter_clear(pcnt_unit);
    pcnt_filter_disable(pcnt_unit);//フィルターを無効化
    pcnt_counter_resume(pcnt_unit);//カウント開始
    pcnt_event_enable(pcnt_unit, PCNT_EVT_H_LIM);//イベントを有効化
    pcnt_event_enable(pcnt_unit, PCNT_EVT_L_LIM);//イベントを有効化
    pcnt_isr_register(caller, NULL, 0, &user_isr_handle);//割り込みハンドラを登録
    pcnt_intr_enable(pcnt_unit);//割り込みを有効化
}
int32_t encoder::getcount()
{
    int16_t count;
    pcnt_get_counter_value(pcnt_unit, &count);
    return intr_count*INT16_MAX + count;
}
void IRAM_ATTR encoder::caller(void *arg)
{
    encoder *obj = static_cast<encoder*>(arg); // cast arg back to encoder
    obj->processInterrupt(); // call non-static method
}
void encoder::processInterrupt()
{
    if (PCNT.status_unit[pcnt_unit].h_lim_lat) {
        intr_count++;
    } else if (PCNT.status_unit[pcnt_unit].l_lim_lat) {
        intr_count--;
    }
    PCNT.int_clr.val = BIT(pcnt_unit);
}