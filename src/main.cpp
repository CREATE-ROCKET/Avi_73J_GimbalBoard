#define SYSTEMID

#include <Arduino.h>
#ifdef SYSTEMID
#include <systemid.h>

Systemid systemid;

COUNTER encoder1;

void setup() { systemid.begin(); }

void loop() { systemid.loop(); }
#endif