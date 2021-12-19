#include "fc22.hpp"
#include <Arduino.h>
#include "debug.hpp"

namespace module
{

CFC22::CFC22(int pin) :
  m_pin(pin)
{
  m_lastValue = 0;
  pinMode(m_pin, INPUT);
}
CFC22::~CFC22() {}

void CFC22::readData()
{
  A_DLOG("Reading cf-22");
  m_lastValue = analogRead(m_pin);
}

int CFC22::value()
{
  tryReadData();
  return m_lastValue;
}

}
