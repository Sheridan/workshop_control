#include "cdelayer.hpp"
#include <Arduino.h>
#include "debug.hpp"

namespace helper
{
CDelayer::CDelayer(const unsigned long &delay) : m_delay(delay) {}
CDelayer::~CDelayer() {}

bool CDelayer::ready()
{
  unsigned long time = millis();
  bool result = time < m_lastTime || (m_lastTime + m_delay) < time;
  // A_DLOG(time);
  // A_DLOG(m_lastTime);
  if(result)
  {
    m_lastTime = time;
    delay(16);
  }
  return result;
}
}
