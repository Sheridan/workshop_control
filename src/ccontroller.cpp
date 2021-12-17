#include "ccontroller.hpp"
#include "debug.hpp"
#include "st.hpp"

#define A_TARGET_ROOM_TEMPERATURE 29.0
#define A_TARGET_ROOM_TEMPERATURE_GATE 0.5
#define A_TARGET_ROOM_TEMPERATURE_HIGH (A_TARGET_ROOM_TEMPERATURE + A_TARGET_ROOM_TEMPERATURE_GATE)
#define A_TARGET_ROOM_TEMPERATURE_LOW  (A_TARGET_ROOM_TEMPERATURE - A_TARGET_ROOM_TEMPERATURE_GATE)

#define A_TARGET_ROOM_CO2 2000
#define A_TARGET_ROOM_ORGANIC 300
#define A_TARGET_ROOM_MQ2 300

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

void CController::controlExhaust()
{
  A_DLOG("---");
  A_DLOG(ST->sensorTHRoom()->temperature());
  A_DLOG("->");
  A_DLOG(A_TARGET_ROOM_TEMPERATURE);
  A_DLOG(A_TARGET_ROOM_TEMPERATURE_HIGH);
  A_DLOG(A_TARGET_ROOM_TEMPERATURE_LOW);
  A_DLOG("---");
  if(ST->sensorTHRoom()->temperature() > A_TARGET_ROOM_TEMPERATURE_HIGH ||
     ST->sensorCCSRoom()->getCO2()     > A_TARGET_ROOM_CO2              ||
     ST->sensorCCSRoom()->getOrganic() > A_TARGET_ROOM_ORGANIC          ||
     ST->sensorMQ2Room()->value()      > A_TARGET_ROOM_MQ2)
  {
    ST->releExhaust()->on();
  }
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
  if(ST->sensorTHRoom()->temperature() < A_TARGET_ROOM_TEMPERATURE_LOW)
  {
    ST->releRoomHeater()->on();
  }
  if(ST->sensorTHRoom()->temperature() >= A_TARGET_ROOM_TEMPERATURE)
  {
    ST->releRoomHeater()->off();
  }
}
