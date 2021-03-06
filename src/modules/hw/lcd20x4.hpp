# pragma once
#include "modules/cmodule.hpp"
#include "helpers/cdelayer.hpp"
#include <LiquidCrystal_I2C.h>
#define REFRESH_DELAY 1600

namespace module
{
enum EDisplayView
{
  dvDashboard,
  dvRoomTemperatureHumidity,
  dvLobbyTemperatureHumidity,
  dvStreetTemperatureHumidity,
  dvServerTemperatureHumidity,
  dvRoomAirStatus,
  dvEffectorStatus
};

class CLCD20x4 : public CModule, public helper::CDelayer
{
public:
  CLCD20x4();
  ~CLCD20x4();
  void init();
  void reset();
  void showStatus();
  void indicate(const String &symbol);
  void indicateReset();
  void showMessage(const char *msg);
  template <typename T> void print(uint8_t x, uint8_t y, T data)
  {
    setCursor(x,y);
    m_module->print(data);
  }
  template <typename T> void print(T data)
  {
    m_module->print(data);
  }
  void incrementView();
  void decrementView();

private:
  LiquidCrystal_I2C *m_module;
  unsigned long m_prevStatusTime;
  EDisplayView m_view;
  void setCursor(uint8_t x, uint8_t y);

  void viewDashboard();
  void viewRoomTemperatureHumidity();
  void viewLobbyTemperatureHumidity();
  void viewStreetTemperatureHumidity();
  void viewServerTemperatureHumidity();
  void viewRoomAirStatus();
  void viewEffectorStatus();
};


}
