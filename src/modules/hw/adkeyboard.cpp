#include "adkeyboard.hpp"
#include "st.hpp"
#include "debug.hpp"
namespace module
{

CAdKeyboard::CAdKeyboard(const uint8_t &pin) :
  CModule(),
  m_pin(pin)
{
  m_lastKey = EAdKeyboardKey::akkUnknown;
}

void CAdKeyboard::init()
{
  pinMode(m_pin, INPUT);
}

CAdKeyboard::~CAdKeyboard() {}

EAdKeyboardKey CAdKeyboard::getKey()
{
  int a = analogRead(m_pin);
  delay(10);
  int b = analogRead(m_pin);
  EAdKeyboardKey key = EAdKeyboardKey::akkUnknown;
  if(b >= a-10 && b <= a+10)
  {
    if(a >=  0 && a < 100) { key = EAdKeyboardKey::akkSW1; }
    if(a > 100 && a < 200) { key = EAdKeyboardKey::akkSW2; }
    if(a > 300 && a < 500) { key = EAdKeyboardKey::akkSW3; }
    if(a > 500 && a < 600) { key = EAdKeyboardKey::akkSW4; }
    if(a > 700 && a < 800) { key = EAdKeyboardKey::akkSW5; }
  }
  return key;
}

void CAdKeyboard::check()
{
  EAdKeyboardKey key = getKey();
  if(key != m_lastKey)
  {
    m_lastKey = key;
    switch(key)
    {
      case EAdKeyboardKey::akkSW1:
      {
        A_DLOG("SW1");
        return;
      }
      case EAdKeyboardKey::akkSW2:
      {
        A_DLOG("SW2");
        return;
      }
      case EAdKeyboardKey::akkSW3:
      {
        A_DLOG("SW3");
        return;
      }
      case EAdKeyboardKey::akkSW4:
      {
        A_DLOG("SW4");
        return;
      }
      case EAdKeyboardKey::akkSW5:
      {
        A_DLOG("SW5");
        return;
      }
      default: return;
    }
  }
}

}
