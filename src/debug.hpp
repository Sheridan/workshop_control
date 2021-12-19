#pragma once
#include <Arduino.h>

#define A_DEBUG

#ifdef A_DEBUG
#define A_DEBUG_INIT Serial.begin(9600);
#define A_DLOG(_msg) Serial.print(_msg); Serial.println();
#else
#define A_DEBUG_INIT
#define A_DLOG(_msg)
#endif // #A_DEBUG
