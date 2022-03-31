#include "hw307.hpp"
#include "debug.hpp"

namespace module
{
CHW307::CHW307(int pin) : m_pin(pin)
{
  m_on = true;
}

void CHW307::init()
{
  pinMode(m_pin, OUTPUT);
  off();
}

CHW307::~CHW307() {}

void CHW307::on(const String &reason)
{
  m_reason = reason;
  if(!m_on)
  {
    A_DLOG("Turn hw307 on");
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, LOW);
    m_on = true;
    delay(16);
  }
}

void CHW307::off()
{
  m_reason = "off";
  if(m_on)
  {
    A_DLOG("Turn hw307 off");
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, HIGH);
    m_on = false;
    delay(16);
  }
}

void CHW307::reverse()
{
  if(m_on) { off(); }
  else     { on (); }
}

bool CHW307::isOn()
{
  return m_on;
}

String CHW307::state()
{
  return m_on ? "on" : "off";
}

String CHW307::reason()
{
  return m_reason;
}

}
