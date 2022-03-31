#include "gybm280.hpp"
#include "debug.hpp"

namespace module
{
  CGYBM280::CGYBM280(const uint8_t &pin)
    : m_pin(pin)
  {
    m_lastTemperature = 0;
    m_lastHumidity    = 0;
    m_lastPressure    = 0;
    m_sensor = new Adafruit_BME280(m_pin);
  }

  void CGYBM280::init()
  {
    m_sensor->begin();
  }

  CGYBM280::~CGYBM280()
  {
    delete m_sensor;
  }

  float CGYBM280::pressure()
  {
    tryReadData();
    return m_lastPressure;
  }

  float CGYBM280::temperature()
  {
    tryReadData();
    return m_lastTemperature;
  }

  float CGYBM280::humidity()
  {
    tryReadData();
    return m_lastHumidity;;
  }

  void CGYBM280::readData()
  {
    A_DLOG("Reading gy-bm 280");
    m_lastHumidity = m_sensor->readHumidity();
    m_lastTemperature = m_sensor->readTemperature();
    m_lastPressure   = m_sensor->readPressure();
  }
}
