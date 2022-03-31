#include "am2301.hpp"
#include "debug.hpp"

namespace module
{
CAM2301::CAM2301(const uint8_t &pin, const float &temperatureDelta) :
  m_pin(pin),
  m_temperatureDelta(temperatureDelta)
{
  m_lastTemperature = 0;
  m_lastHumidity    = 0;
  m_lastHeatIndex   = 0;
  m_sensor = new DHT(m_pin, AM2301);
}

void CAM2301::init()
{
  m_sensor->begin();
}

CAM2301::~CAM2301()
{
  delete m_sensor;
}

void CAM2301::readData()
{
  A_DLOG("Reading am2301");
  A_READ_SENSOR(m_sensor->readTemperature(false)  + m_temperatureDelta, float, m_lastTemperature, A_TEMPERATURE_HARD_MIN, A_TEMPERATURE_HARD_MAX);
  A_READ_SENSOR(m_sensor->computeHeatIndex(false) + m_temperatureDelta, float, m_lastHeatIndex  , A_TEMPERATURE_HARD_MIN, A_TEMPERATURE_HARD_MAX);
  A_READ_SENSOR(m_sensor->readHumidity()                              , float, m_lastHumidity   , A_HUMIDITY_HARD_MIN   , A_HUMIDITY_HARD_MAX);
}

float CAM2301::humidity()
{
  tryReadData();
  return m_lastHumidity;
}

float CAM2301::temperature()
{
  tryReadData();
  return m_lastTemperature;
}

float CAM2301::heatIndex()
{
  tryReadData();
  return m_lastHeatIndex;
}

}
