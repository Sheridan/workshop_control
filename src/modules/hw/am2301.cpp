#include "am2301.hpp"

namespace module
{
CAM2301::CAM2301(uint8_t pin) :
  m_pin(pin)
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
  m_lastTemperature = m_sensor->readTemperature();
  m_lastHumidity    = m_sensor->readHumidity();
  m_lastHeatIndex   = m_sensor->computeHeatIndex(false); 
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