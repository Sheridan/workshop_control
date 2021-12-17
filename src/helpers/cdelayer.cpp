#include "cdelayer.hpp"
#include <Arduino.h>
namespace helper
{
CDelayer::CDelayer(const unsigned long &delay) : m_delay(delay) {}
CDelayer::~CDelayer() {}

bool CDelayer::ready()
{
  unsigned long time = millis();
  bool result = time < m_lastTime || (m_lastTime + m_delay) < time;
  if(result)
  {
    m_lastTime = time;
  }
  return result;
}
}
