#include "ccontroller.hpp"
#include "debug.hpp"
#include "st.hpp"

CController::CController() : helper::CDelayer(A_CONTROL_DELAY)
{}

CController::~CController()
{}

void CController::check()
{
  if(ready())
  {
    A_DLOG("Control check");
    controlExhaust();
    controlRoomHeater();
    controlRoomCooler();
  }
}

#define A_TURN_ON(_ctrl,_sensor,_operation,_targetvalue,_reason_text) \
  if(_sensor _operation _targetvalue) \
  { \
    ST->_ctrl()->on(_reason_text); \
    return; \
  } \
  delay(4);

void CController::controlExhaust()
{
  A_TURN_ON(releExhaust, ST->sensorTHRoom()->heatIndex(),   >, A_TARGET_ROOM_TEMPERATURE_STAGE1_HIGH, "temperature");
  A_TURN_ON(releExhaust, ST->sensorCCSRoom()->getCO2()    , >, A_TARGET_ROOM_CO2                    , "co2"        );
  A_TURN_ON(releExhaust, ST->sensorCCSRoom()->getOrganic(), >, A_TARGET_ROOM_ORGANIC                , "organic"    );
  A_TURN_ON(releExhaust, ST->sensorMQ2Room()->value()     , >, A_TARGET_ROOM_MQ2                    , "mq2"        );
  if(ST->sensorTHRoom()->heatIndex()   <= A_TARGET_ROOM_TEMPERATURE      &&
     ST->sensorCCSRoom()->getCO2()     <= A_TARGET_ROOM_CO2              &&
     ST->sensorCCSRoom()->getOrganic() <= A_TARGET_ROOM_ORGANIC          &&
     ST->sensorMQ2Room()->value()      <= A_TARGET_ROOM_MQ2)
  {
    ST->releExhaust()->off();
  }
}

void CController::controlRoomHeater()
{
  A_TURN_ON(releRoomHeater, ST->sensorTHRoom()->heatIndex(), <, A_TARGET_ROOM_TEMPERATURE_STAGE1_LOW, "temperature");
  if(ST->sensorTHRoom()->heatIndex() >= A_TARGET_ROOM_TEMPERATURE)
  {
    ST->releRoomHeater()->off();
  }
}

void CController::controlRoomCooler()
{
  A_TURN_ON(releRoomCooler, ST->releExhaust()->isOn()      ,  /**/, /**/                                 , "exhaust_on");
  A_TURN_ON(releRoomCooler, ST->releRoomHeater()->isOn()   ,  /**/, /**/                                 , "heater_on");
  A_TURN_ON(releRoomCooler, ST->sensorTHRoom()->heatIndex(),   >  , A_TARGET_ROOM_TEMPERATURE_STAGE0_HIGH, "temperature_high");
  A_TURN_ON(releRoomCooler, ST->sensorTHRoom()->heatIndex(),   <  , A_TARGET_ROOM_TEMPERATURE_STAGE0_LOW , "temperature_low");
  if(ST->sensorTHRoom()->heatIndex()   <= A_TARGET_ROOM_TEMPERATURE_STAGE0_HIGH &&
     ST->sensorTHRoom()->heatIndex()   >= A_TARGET_ROOM_TEMPERATURE_STAGE0_LOW)
  {
    ST->releRoomCooler()->off();
  }
}
