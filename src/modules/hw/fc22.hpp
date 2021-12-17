#pragma once
#include "modules/csensor.hpp"

namespace module
{

class CFC22 : public CSensor
{
public:
  CFC22(int pin);
  ~CFC22();
  int value();
private:
  void readData();
  int m_pin;
  int m_lastValue;
};

}
