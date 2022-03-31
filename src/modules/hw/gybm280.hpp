#pragma once
#include "modules/csensor.hpp"
#include <Arduino.h>
// #include <Wire.h>
// #include <SPI.h>
#include <Adafruit_BME280.h>

namespace module
{
class CGYBM280 : public CSensor
{
  public:
    CGYBM280(const uint8_t &pin);
    ~CGYBM280();
    void init();

    float pressure();
    float temperature();
    float humidity();
  private:
    void readData();
    float m_lastPressure;
    float m_lastTemperature;
    float m_lastHumidity;
    uint8_t m_pin;
    Adafruit_BME280 *m_sensor;
};
}
