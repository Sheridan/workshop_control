# pragma once
#include "modules/cmodule.hpp"
#include <Arduino.h>
#define INTEGRATED_PIN 13

namespace module
{
class CDiode : public CModule
{
public:
  CDiode(int pin);
  ~CDiode();
  void init();
  void on();
  void off();
  void reverse();
private:
  int m_pin;
  bool m_on;
};


}
