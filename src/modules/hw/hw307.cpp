#include "hw307.hpp"

namespace module
{
CHW307::CHW307(int pin) : m_pin(pin) {}

void CHW307::init()
{
  pinMode(m_pin, OUTPUT);
  off();
}

CHW307::~CHW307() {}

void CHW307::on()
{
  digitalWrite(m_pin, LOW);
  m_on = true;
}

void CHW307::off()
{
  digitalWrite(m_pin, HIGH);
  m_on = false;
}

void CHW307::reverse()
{
  if(m_on) { off(); }
  else     { on (); }
}

unsigned int CHW307::state()
{
  return static_cast<unsigned int>(m_on);
}

}
