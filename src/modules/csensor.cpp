#include "modules/csensor.hpp"

namespace module
{
CSensor::CSensor() : CModule(), helper::CDelayer(A_DATA_REFRESH_DELAY) {}
CSensor::~CSensor() {}

void CSensor::tryReadData()
{
  if(ready())
  {
    readData();
  }
}
}
