// #define SYSTEMID
// #define MAIN

#include <Arduino.h>

#ifdef MAIN

#include "UARTManager.h"
#include "gimbalpin.h"

UARTManager uartManager;

void setup() { uartManager.begin(); }
void loop() { uartManager.update(); }

#endif
#ifdef SYSTEMID
#include <systemid.h>

Systemid systemid;

COUNTER encoder1;

void setup() { systemid.begin(); }

void loop() { systemid.loop(); }
#endif

// PWMtest

// #include "PWMwrapper.h"
// #include "gimbalpin.h"

// PWMWrapper pitch_cw;
// PWMWrapper pitch_ccw;
// PWMWrapper roll_cw;
// PWMWrapper roll_ccw;

// void setup() {
//     pitch_cw.begin(PIN::P_PWM_CW);
//     pitch_ccw.begin(PIN::P_PWM_CCW);
//     roll_cw.begin(PIN::R_PWM_CW);
//     roll_ccw.begin(PIN::R_PWM_CCW);
//     pitch_cw.setVoltage(2.5);
//     pitch_ccw.setVoltage(0);
//     roll_cw.setVoltage(2.5);
//     roll_ccw.setVoltage(0);
// }
// void loop() {}

// #include "gimbalpin.h"
// void setup() {
//     Serial.begin(115200);
//     while (!Serial)
//         ;
//     Serial2.begin(115200, SERIAL_8N1, PIN::RTD_RX, PIN::RTD_TX);
//     // Serial2.begin(115200, SERIAL_8N1, 17, 16);
//     while (!Serial2)
//         ;
//     delay(1000);
// }

// char a;

// void loop() {
//     // delay(1000);
//     // char b = 'a';
//     // Serial2.write(b);
//     if (Serial2.available()) {
//         a = Serial2.read();
//         Serial.print(a);
//     }
// }

// para

// void setup() { Serial.begin(115200); }
// void loop() {
//     Serial.println("hello");
//     delay(1000);
// }