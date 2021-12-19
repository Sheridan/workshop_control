#include "ccontroller.hpp"
#include "debug.hpp"
#include "st.hpp"

#define A_TARGET_ROOM_TEMPERATURE 25.0
#define A_TARGET_ROOM_TEMPERATURE_GATE 1.5
#define A_TARGET_ROOM_TEMPERATURE_HIGH (A_TARGET_ROOM_TEMPERATURE + A_TARGET_ROOM_TEMPERATURE_GATE)
#define A_TARGET_ROOM_TEMPERATURE_LOW  (A_TARGET_ROOM_TEMPERATURE - A_TARGET_ROOM_TEMPERATURE_GATE)

#define A_TARGET_ROOM_CO2 6000
#define A_TARGET_ROOM_ORGANIC 3000
#define A_TARGET_ROOM_MQ2 600

CController::CController() : helper::CDelayer(A_CONTROL_DELAY)
{}

CController::~CController()
{}

void CController::check()
{
  if(ready())
  {
    controlExhaust();
    controlRoomHeater();
  }
}

#define A_TURN_ON(_ctrl,_sensor,_operation,_targetvalue,_reason_text) \
  if(_sensor _operation _targetvalue) \
  { \
    ST->_ctrl()->on(_reason_text); \
    return; \
  }

void CController::controlExhaust()
{
  A_TURN_ON(releExhaust, ST->sensorTHRoom()->temperature(), >, A_TARGET_ROOM_TEMPERATURE_HIGH, "temperature");
  A_TURN_ON(releExhaust, ST->sensorCCSRoom()->getCO2()    , >, A_TARGET_ROOM_CO2             , "co2"        );
  A_TURN_ON(releExhaust, ST->sensorCCSRoom()->getOrganic(), >, A_TARGET_ROOM_ORGANIC         , "organic"    );
  A_TURN_ON(releExhaust, ST->sensorMQ2Room()->value()     , >, A_TARGET_ROOM_MQ2             , "mq2"        );
  if(ST->sensorTHRoom()->temperature() <= A_TARGET_ROOM_TEMPERATURE      &&
     ST->sensorCCSRoom()->getCO2()     <= A_TARGET_ROOM_CO2              &&
     ST->sensorCCSRoom()->getOrganic() <= A_TARGET_ROOM_ORGANIC          &&
     ST->sensorMQ2Room()->value()      <= A_TARGET_ROOM_MQ2)
  {
    ST->releExhaust()->off();
  }
}

void CController::controlRoomHeater()
{
  A_TURN_ON(releRoomHeater, ST->sensorTHRoom()->temperature(), <, A_TARGET_ROOM_TEMPERATURE_LOW, "temperature");
  if(ST->sensorTHRoom()->temperature() >= A_TARGET_ROOM_TEMPERATURE)
  {
    ST->releRoomHeater()->off();
  }
}
