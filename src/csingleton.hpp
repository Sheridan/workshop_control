#pragma once
#include "debug.hpp"
#include "modules/hw/am2301.hpp"
#include "modules/hw/lcd20x4.hpp"
#include "modules/hw/enc28j60.hpp"
#include "modules/hw/fc22.hpp"
#include "modules/hw/ccs811.hpp"
#include "modules/hw/diode.hpp"
#include "modules/hw/hw307.hpp"
#include "modules/hw/adkeyboard.hpp"
#include "modules/hw/gybm280.hpp"

#define A_SENSOR_LOBBY_TH_D_PIN 2
#define A_SENSOR_ROOM_TH_D_PIN 3
#define A_SENSOR_ROOM_TH_D_PIN 3
#define A_SENSOR_STREET_TH_D_PIN 4
#define A_SENSOR_SERVER_TH_D_PIN 5
#define A_SENSOR_ROOM_SMOKE_A_PIN 0
#define A_INTEGRATED_DIODE_D_PIN 13
#define A_RELE_EXHAUST_D_PIN 22
#define A_RELE_ROOMHEATER_D_PIN 24
#define A_RELE_ROOMCOOLER_D_PIN 26
#define A_ADKEYBOARD_A_PIN 1
#define A_ADKEYBOARD_GYBM_PIN 22

#define ST_TONULL(_name) m_##_name = nullptr;

//  mainLCD()->showMessage("Init " #_name); delay(1000);
#define ST_INIT(_name) \
  A_DLOG("Initialize " #_name ); \
  _name()->init();

#define ST_SIMPLE_DECLARE(_class,_name,...) \
  private: \
    _class *m_##_name; \
  public: \
    _class *_name() \
    { \
      if(!m_##_name) \
      { \
        m_##_name = new _class(__VA_ARGS__); \
      } \
      return m_##_name; \
    }

class CSingleTon
{
  public:
    static CSingleTon *instance();
    void init();
  private:
    CSingleTon();
    static CSingleTon* m_singleton;

  // ST_SIMPLE_DECLARE(module::CGYBM280   , sensorPressure , A_ADKEYBOARD_GYBM_PIN);
  ST_SIMPLE_DECLARE(module::CAdKeyboard, keyboard       , A_ADKEYBOARD_A_PIN);
  ST_SIMPLE_DECLARE(module::CHW307     , releRoomCooler , A_RELE_ROOMCOOLER_D_PIN);
  ST_SIMPLE_DECLARE(module::CHW307     , releRoomHeater , A_RELE_ROOMHEATER_D_PIN);
  ST_SIMPLE_DECLARE(module::CHW307     , releExhaust    , A_RELE_EXHAUST_D_PIN);
  ST_SIMPLE_DECLARE(module::CDiode     , integratedDiode, A_INTEGRATED_DIODE_D_PIN);
  ST_SIMPLE_DECLARE(module::CAM2301    , sensorTHLobby  , A_SENSOR_LOBBY_TH_D_PIN);
  ST_SIMPLE_DECLARE(module::CAM2301    , sensorTHRoom   , A_SENSOR_ROOM_TH_D_PIN);
  ST_SIMPLE_DECLARE(module::CAM2301    , sensorTHStreet , A_SENSOR_STREET_TH_D_PIN);
  ST_SIMPLE_DECLARE(module::CAM2301    , sensorTHServer , A_SENSOR_SERVER_TH_D_PIN);
  ST_SIMPLE_DECLARE(module::CFC22      , sensorMQ2Room  , A_SENSOR_ROOM_SMOKE_A_PIN);
  ST_SIMPLE_DECLARE(module::CCCS811    , sensorCCSRoom  , /* empty */);
  ST_SIMPLE_DECLARE(module::CLCD20x4   , mainLCD        , /* empty */);
  ST_SIMPLE_DECLARE(module::CENC28J60  , eth            , /* empty */);
};
