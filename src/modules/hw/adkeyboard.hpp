#pragma once
#include <Arduino.h>
#include "modules/cmodule.hpp"

namespace module
{
  enum EAdKeyboardKey
  {
    akkUnknown,
    akkSW1,
    akkSW2,
    akkSW3,
    akkSW4,
    akkSW5
  };

  class CAdKeyboard : public CModule
  {
  public:
    CAdKeyboard(const uint8_t &pin);
    ~CAdKeyboard();
    void init();
    void check();
  private:
    uint8_t m_pin;
    EAdKeyboardKey m_lastKey;

    EAdKeyboardKey getKey();
  };



}
