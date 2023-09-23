#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID{
    private:
        double Kp, Ki, Kd;
        double previousRadian = 0;
        double error = 0;
        double integral = 0;
        double targetRadian = 0;
        const double dt = 0.001;
        const int volt = 5;
        double inputVoltage = 0;

    public:
        PID(double p, double i, double d);
        double calculateIPD(double radian);
        void setTarget(double target) {targetRadian = target;}
        double getVoltage() {return inputVoltage;}
        void printParameter();

};

PID::PID(double proportionalGain, double integralGain, double derivativeGain){
    Kp = proportionalGain;
    Ki = integralGain;
    Kd = derivativeGain;
}
double PID::calculateIPD(double radian){
    error =  targetRadian  - radian;
    integral += error*dt;
    inputVoltage = -Kp*radian + Ki*integral - Kd*(radian-previousRadian)/dt;
    previousRadian = radian;
    if(inputVoltage > volt) {inputVoltage = volt; integral -= error*dt;}
    else if(inputVoltage < -volt) {inputVoltage = -volt; integral -= error*dt;}
    return inputVoltage;
}
void PID::printParameter(){
    Serial.printf("Kp: %f, Ki: %f, Kd: %f, previousRadian: %f, error: %f, integral %f\n", Kp, Ki, Kd, previousRadian, error, integral);
}
#endif