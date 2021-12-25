#include "adkeyboard.hpp"
#include "st.hpp"
#include "debug.hpp"
namespace module
{

CAdKeyboard::CAdKeyboard(const uint8_t &pin) :
  CModule(), helper::CDelayer(A_KEY_READ_DELAY),
  m_pin(pin)
{
  m_lastKey = EAdKeyboardKey::akkNone;
}

void CAdKeyboard::init()
{
  pinMode(m_pin, INPUT);
}

CAdKeyboard::~CAdKeyboard() {}

EAdKeyboardKey CAdKeyboard::getKey()
{
  int a = analogRead(m_pin);
  delay(8);
  int b = analogRead(m_pin);
  if(b >= a-10 && b <= a+10)
  {
    if(a >=  0 && a < 100) { return EAdKeyboardKey::akkSW1 ; }
    if(a > 100 && a < 200) { return EAdKeyboardKey::akkSW2 ; }
    if(a > 300 && a < 500) { return EAdKeyboardKey::akkSW3 ; }
    if(a > 500 && a < 600) { return EAdKeyboardKey::akkSW4 ; }
    if(a > 700 && a < 800) { return EAdKeyboardKey::akkSW5 ; }
    if(a > 1000          ) { return EAdKeyboardKey::akkNone; }
    A_DLOG(b);
  }
  return EAdKeyboardKey::akkUnknown;
}

void CAdKeyboard::check()
{
  if(!ready()) { return; }
  EAdKeyboardKey key = getKey();
  if(key != m_lastKey && key != EAdKeyboardKey::akkUnknown)
  {
    m_lastKey = key;
    switch(key)
    {
      case EAdKeyboardKey::akkSW1:
      {
        A_DLOG("adk: SW1");
        return;
      }
      case EAdKeyboardKey::akkSW2:
      {
        A_DLOG("adk: SW2");
        ST->mainLCD()->incrementView();
        return;
      }
      case EAdKeyboardKey::akkSW3:
      {
        A_DLOG("adk: SW3");
        ST->mainLCD()->decrementView();
        return;
      }
      case EAdKeyboardKey::akkSW4:
      {
        A_DLOG("adk: SW4");
        return;
      }
      case EAdKeyboardKey::akkSW5:
      {
        A_DLOG("adk: SW5");
        return;
      }
      case EAdKeyboardKey::akkNone:
      {
        A_DLOG("adk: Unpressed");
        return;
      }
      default: { A_DLOG(int(key)); return; }
    }
  }
}

}
