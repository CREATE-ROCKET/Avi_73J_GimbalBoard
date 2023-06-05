#include <Arduino.h>
#include <driver/pcnt.h>
#include <driver/ledc.h>

class motor{
    private:
        int PWM_CHANNEL0 = 0;
        int PWM_CHANNEL1 = 1;
        int PWM_FREQ = 1000;
        int PWM_RESOLUTION = 8; 
        //pidtest
        float t = 0;
        float u = 0;
        int u_pwm = 0;
        float f = 0.1;
        float Ts = 0.001;
    public:
        void begin(int IN1, int IN2, int PWM_FREQ, int PWM_RESOLUTION);
        void pwmtest();
        void sysad();
        void getpwm();
};

void motor::begin(int IN1,int IN2, int PWM_FREQ, int PWM_RESOLUTION)
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    ledcSetup(PWM_CHANNEL0, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(IN1, PWM_CHANNEL0);
    ledcSetup(PWM_CHANNEL1, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(IN2, PWM_CHANNEL1);
    t = 0;
    u = 0;
    u_pwm = 0;
    f = 0.1;
}
void motor::pwmtest()
{
    ledcWrite(PWM_CHANNEL1, PWM_FREQ);
    for (int i = 0; i < PWM_FREQ; i++)
    {
        ledcWrite(PWM_CHANNEL0, i);
        delay(100);
    }
    for(int i = PWM_FREQ; i >= 0; i--)
    {
        ledcWrite(PWM_CHANNEL1, i);
        delay(100);
    }
}
void motor::sysad()
{
    t += Ts;//時間の更新
    u = sin(2 * PI * f * t);//入力の計算
    u_pwm = int(u * 1023);  //入力をPWMのDuty比に変換
    //モータドライバへの出力
    if (u_pwm >= 0)
    {
        ledcWrite(PWM_CHANNEL0, LOW);
        ledcWrite(PWM_CHANNEL1, u_pwm);
    }
    else {
        ledcWrite(PWM_CHANNEL1, LOW);
        ledcWrite(PWM_CHANNEL0, -u_pwm);
    }
    //入力周波数の更新
    if (t > 1/f) {
        t = 0;
        if      (f < 1)  f += 0.1;
        else if (f < 10) f += 1;
        else if (f < 100)f += 10;
    }
    //入力周波数が100Hz以上になったら終了
    if (f >= 100) {
        ledcWrite(PWM_CHANNEL0, 0);
        ledcWrite(PWM_CHANNEL1, 0);
        t = 0;
        u = 0;
        u_pwm = 0;
        f = 0.1;
    }
}
void motor::getpwm()
{
    Serial.print("u_pwm =");
    Serial.print(u_pwm);
    Serial.print(" ");
    Serial.print("f =");
    Serial.print(f);
}