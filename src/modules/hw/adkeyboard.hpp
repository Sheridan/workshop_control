#pragma once
#include <Arduino.h>
#include "modules/cmodule.hpp"
#include "helpers/cdelayer.hpp"
#define A_KEY_READ_DELAY 100

namespace module
{
  enum EAdKeyboardKey
  {
    akkNone,
    akkUnknown,
    akkSW1,
    akkSW2,
    akkSW3,
    akkSW4,
    akkSW5
  };

  class CAdKeyboard : public CModule, public helper::CDelayer
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
