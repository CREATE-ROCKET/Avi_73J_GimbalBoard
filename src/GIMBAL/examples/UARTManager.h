#include <Arduino.h>

#include "UARTManager.h"
#include "gimbalpin.h"

UARTManager uartManager;

void setup() { uartManager.begin(); }
void loop() { uartManager.update(); }