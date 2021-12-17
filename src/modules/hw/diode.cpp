#include "diode.hpp"

namespace module
{
CDiode::CDiode(int pin) : m_pin(pin)
{}

void CDiode::init()
{
  pinMode(m_pin, OUTPUT);
  off();
}

CDiode::~CDiode()
{}

void CDiode::on()
{
  digitalWrite(m_pin, HIGH);
  m_on = true;
}
void CDiode::off()
{
  digitalWrite(m_pin, LOW);
  m_on = false;
}

void CDiode::reverse()
{
  if(m_on) { off(); }
  else { on(); }
}

}
