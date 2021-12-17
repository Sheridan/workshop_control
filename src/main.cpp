#include <Arduino.h>
#include <SPI.h>
#include "st.hpp"
#include "ccontroller.hpp"

CController ctrl;

void setup()
{
  A_DEBUG_INIT;
  A_DLOG("Initialize start");
  ST->init();
  ST->mainLCD()->reset();
  A_DLOG("Initialize end");
}

void loop()
{
  ST->eth()->checkQuery();
  ST->mainLCD()->showStatus();
  ctrl.check();
  // delay(500);
  // ST->releExhaust()->on();
}
