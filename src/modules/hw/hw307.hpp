#pragma once
#include "modules/cmodule.hpp"
#include <Arduino.h>
#define INTEGRATED_PIN 13

namespace module
{
class CHW307 : public CModule
{
public:
  CHW307(int pin);
  ~CHW307();
  void init();
  void on(const String &reason = "generic");
  void off();
  void reverse();
  String state();
  String reason();
private:
  int m_pin;
  bool m_on;
  String m_reason;
};


}
