#include <Arduino.h>
#include <systemid.h>

Systemid systemid;

COUNTER encoder1;

void setup() { systemid.begin(); }

void loop() { systemid.loop(); }