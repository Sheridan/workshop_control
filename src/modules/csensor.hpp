#pragma once
#include "modules/cmodule.hpp"
#include "helpers/cdelayer.hpp"
#define A_DATA_REFRESH_DELAY 1000

#define A_READ_SENSOR(_call,_type,_variable,_hard_min,_hard_max) \
{ \
  _type tmpVal = _call; \
  if (tmpVal >= _hard_min && tmpVal <= _hard_max) \
  { \
    _variable = tmpVal; \
  } \
}

namespace module
{
class CSensor : public CModule, public helper::CDelayer
{
public:
  CSensor();
  virtual ~CSensor();
  void tryReadData();
protected:
  virtual void readData() = 0;
};

}
