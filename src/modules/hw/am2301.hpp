#pragma once
#include "modules/csensor.hpp"
#include <DHT.h>

#define A_TEMPERATURE_HARD_MIN -30
#define A_TEMPERATURE_HARD_MAX 50

#define A_HUMIDITY_HARD_MIN 0
#define A_HUMIDITY_HARD_MAX 100

namespace module
{
class CAM2301 : public CSensor
{
  public:
    CAM2301(const uint8_t &pin, const float &temperatureDelta = 0);
    ~CAM2301();
    void init();

    float humidity();
    float temperature();
    float heatIndex();
  private:
    void readData();
    float m_lastHumidity;
    float m_lastTemperature;
    float m_lastHeatIndex;
    float m_temperatureDelta;
    uint8_t m_pin;
    DHT *m_sensor;
};
}
