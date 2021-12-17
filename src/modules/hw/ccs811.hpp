#pragma once
#include "modules/csensor.hpp"
#include <Adafruit_CCS811.h>
namespace module
{

class CCCS811 : public CSensor
{
public:
  CCCS811();
  ~CCCS811();
  void init();
  int getOrganic();
  int getCO2();
private:
  void readData();
  Adafruit_CCS811 *m_module;
  int m_lastOrganic;
  int m_lastCO2;

};

}
