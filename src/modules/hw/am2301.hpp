#pragma once
#include "modules/csensor.hpp"
#include <DHT.h>

namespace module
{
class CAM2301 : public CSensor
{
  public:
    CAM2301(uint8_t pin);
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
    uint8_t m_pin;
    DHT *m_sensor;   
};
}