#include "lcd20x4.hpp"
#include "st.hpp"

namespace module
{
CLCD20x4::CLCD20x4() : CModule(), helper::CDelayer(REFRESH_DELAY)
{
  m_module = new LiquidCrystal_I2C(0x27,20,4);
  m_prevStatusTime = 0;
  m_view = EDisplayView::dvDashboard;
}

void CLCD20x4::init()
{
  m_module->init();
  m_module->backlight();
  showMessage("Initialize...");
}

CLCD20x4::~CLCD20x4()
{
  reset();
  delete m_module;
}

void CLCD20x4::reset()
{
  m_module->clear();
  m_module->setCursor(0,0);
}

void CLCD20x4::setCursor(uint8_t x, uint8_t y)
{
  m_module->setCursor(x, y);
}

void CLCD20x4::showMessage(const char *msg)
{
  reset();
  print(msg);
}

void CLCD20x4::showStatus()
{
  if(ready())
  {
    switch(m_view)
    {
      case EDisplayView::dvDashboard: { viewDashboard(); return; }
      case EDisplayView::dvRoomTemperatureHumidity: { viewRoomTemperatureHumidity(); return; }
      case EDisplayView::dvLobbyTemperatureHumidity: { viewLobbyTemperatureHumidity(); return; }
      case EDisplayView::dvStreetTemperatureHumidity: { viewStreetTemperatureHumidity(); return; }
      case EDisplayView::dvServerTemperatureHumidity: { viewServerTemperatureHumidity(); return; }
      case EDisplayView::dvRoomAirStatus: { viewRoomAirStatus(); return; }
      case EDisplayView::dvEffectorStatus: { viewEffectorStatus(); return; }
    }
  }
}

#define A_VIEW_MORPH(_src,_dst) \
  case EDisplayView::_src: { m_view = EDisplayView::_dst; return; }

void CLCD20x4::incrementView()
{
  m_module->clear();
  switch(m_view)
  {
    A_VIEW_MORPH(dvDashboard                , dvRoomTemperatureHumidity  );
    A_VIEW_MORPH(dvRoomTemperatureHumidity  , dvLobbyTemperatureHumidity );
    A_VIEW_MORPH(dvLobbyTemperatureHumidity , dvStreetTemperatureHumidity);
    A_VIEW_MORPH(dvStreetTemperatureHumidity, dvServerTemperatureHumidity);
    A_VIEW_MORPH(dvServerTemperatureHumidity, dvRoomAirStatus            );
    A_VIEW_MORPH(dvRoomAirStatus            , dvEffectorStatus           );
    A_VIEW_MORPH(dvEffectorStatus           , dvDashboard                );
  }
}
void CLCD20x4::decrementView()
{
  m_module->clear();
  switch(m_view)
  {
    A_VIEW_MORPH(dvDashboard                , dvEffectorStatus           );
    A_VIEW_MORPH(dvEffectorStatus           , dvRoomAirStatus            );
    A_VIEW_MORPH(dvRoomAirStatus            , dvServerTemperatureHumidity);
    A_VIEW_MORPH(dvServerTemperatureHumidity, dvStreetTemperatureHumidity);
    A_VIEW_MORPH(dvStreetTemperatureHumidity, dvLobbyTemperatureHumidity );
    A_VIEW_MORPH(dvLobbyTemperatureHumidity , dvRoomTemperatureHumidity  );
    A_VIEW_MORPH(dvRoomTemperatureHumidity  , dvDashboard                );
  }
}

#define A_POSITIONAL_PRINT_BEGIN(_x,_y,_name) \
  m_module->setCursor(_x, _y); \
  m_module->print(_name ": ");
#define A_POSITIONAL_PRINT_END(_unit) \
  m_module->print(_unit " ");

#define A_POSITIONAL_FLOAT_PRINT(_x,_y,_name,_value,_unit) \
  A_POSITIONAL_PRINT_BEGIN(_x,_y,_name); \
  m_module->print(_value, 1); \
  A_POSITIONAL_PRINT_END(_unit);

#define A_POSITIONAL_INT_PRINT(_x,_y,_name,_value,_unit) \
  A_POSITIONAL_PRINT_BEGIN(_x,_y,_name); \
  m_module->print(_value, 1); \
  A_POSITIONAL_PRINT_END(_unit);

#define A_POSITIONAL_RELE_PRINT(_x,_y,_name,_rele) \
  A_POSITIONAL_PRINT_BEGIN(_x,_y,_name); \
  m_module->print(ST->_rele()->state()); \
  m_module->print("-"); \
  m_module->print(ST->_rele()->reason())

void CLCD20x4::viewDashboard()
{
  A_DLOG("Displaying dashboard");
  A_POSITIONAL_FLOAT_PRINT(0 ,0,"lb", ST->sensorTHLobby()->temperature() , "c");
  A_POSITIONAL_FLOAT_PRINT(11,0,"rm", ST->sensorTHRoom()->temperature()  , "c");
  A_POSITIONAL_FLOAT_PRINT(0 ,1,"st", ST->sensorTHStreet()->temperature(), "c");
  A_POSITIONAL_FLOAT_PRINT(11,1,"sr", ST->sensorTHServer()->temperature(), "c");
  A_POSITIONAL_INT_PRINT  (0 ,2,"mq2", ST->sensorMQ2Room()->value()      , "ppm");
  A_POSITIONAL_INT_PRINT  (0 ,3,"org", ST->sensorCCSRoom()->getOrganic() , /* */);
  A_POSITIONAL_INT_PRINT  (11,3,"CO2", ST->sensorCCSRoom()->getCO2()     , /* */);
}

#define A_DISPALY_AM2301_TEMP_HUMIDITY(_name,_sensor) \
  A_DLOG("Displaying " _name " (" #_sensor ")"); \
  m_module->setCursor(0, 0); m_module->print(_name); \
  A_POSITIONAL_FLOAT_PRINT(0 ,1,"Real", ST->_sensor()->temperature() , "c"); \
  A_POSITIONAL_FLOAT_PRINT(0 ,2,"Heat index", ST->_sensor()->heatIndex() , "c"); \
  A_POSITIONAL_FLOAT_PRINT(0 ,3,"Humidity", ST->_sensor()->humidity() , "%");

void CLCD20x4::viewRoomTemperatureHumidity()
{
  A_DISPALY_AM2301_TEMP_HUMIDITY("Room", sensorTHRoom);
}

void CLCD20x4::viewLobbyTemperatureHumidity()
{
  A_DISPALY_AM2301_TEMP_HUMIDITY("Lobby", sensorTHLobby);
}

void CLCD20x4::viewStreetTemperatureHumidity()
{
  A_DISPALY_AM2301_TEMP_HUMIDITY("Street", sensorTHStreet);
}

void CLCD20x4::viewServerTemperatureHumidity()
{
  A_DISPALY_AM2301_TEMP_HUMIDITY("Server", sensorTHServer);
}

void CLCD20x4::viewRoomAirStatus()
{
  m_module->setCursor(0, 0);
  m_module->print("Air control");
  A_POSITIONAL_INT_PRINT  (0 ,1,"MQ-2", ST->sensorMQ2Room()->value()      , "ppm");
  A_POSITIONAL_INT_PRINT  (0 ,2,"Organic", ST->sensorCCSRoom()->getOrganic() , "ppm");
  A_POSITIONAL_INT_PRINT  (0 ,3,"CO2", ST->sensorCCSRoom()->getCO2()     , "ppm");
}

void CLCD20x4::viewEffectorStatus()
{
  A_POSITIONAL_RELE_PRINT(0, 0, "Heater", releRoomHeater);
  A_POSITIONAL_RELE_PRINT(0, 1, "Cooler", releRoomCooler);
  A_POSITIONAL_RELE_PRINT(0, 2, "Exhaust", releExhaust);
}


}
