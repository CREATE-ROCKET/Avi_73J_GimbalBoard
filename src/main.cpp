#include <Arduino.h>

#define SW1 23
#define SW2 22
#define SW3 21
#define SW4 19

void setup()
{
    pinMode(SW1, OUTPUT);
    digitalWrite(SW1, LOW);
    delay(1);
    pinMode(SW2, OUTPUT);
    digitalWrite(SW2, LOW);
    delay(1);
    pinMode(SW3, OUTPUT);
    digitalWrite(SW3, LOW);
    delay(1);
    pinMode(SW4, OUTPUT);
    digitalWrite(SW4, LOW);
    delay(1);
}

void loop()
{   
    //正転
    digitalWrite(SW2, LOW);
    delay(1);
    digitalWrite(SW3, LOW);
    delay(1);
    digitalWrite(SW1, HIGH);
    delay(1);
    digitalWrite(SW4, HIGH);
    delay(2000);

    //停止
    digitalWrite(SW1, LOW);
    delay(1);
    digitalWrite(SW2, LOW);
    delay(1);
    digitalWrite(SW3, LOW);
    delay(1);
    digitalWrite(SW4, LOW);
    delay(2000);

    //逆転
    digitalWrite(SW1, LOW);
    delay(1);
    digitalWrite(SW4, LOW);
    delay(1);
    digitalWrite(SW2, HIGH);
    delay(1);
    digitalWrite(SW3, HIGH);
    delay(2000);
}