#pragma once
#include "helpers/cdelayer.hpp"
#define A_CONTROL_DELAY (5*1000)
class CController : public helper::CDelayer
{
public:
  CController();
  ~CController();
  void check();
private:
  void controlExhaust();
  void controlRoomHeater();
};
