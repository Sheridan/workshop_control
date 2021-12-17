#pragma once
#include "modules/cmodule.hpp"
#include "helpers/cdelayer.hpp"
#define A_DATA_REFRESH_DELAY 1000

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
