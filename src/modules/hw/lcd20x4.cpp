#include "lcd20x4.hpp"
#include "st.hpp"

namespace module
{
CLCD20x4::CLCD20x4() : CModule(), helper::CDelayer(REFRESH_DELAY)
{
  m_module = new LiquidCrystal_I2C(0x27,20,4);
  m_prevStatusTime = 0;
}

void CLCD20x4::init()
{
  m_module->init();
  m_module->backlight();
  showMessage("Initialize...");
}

CLCD20x4::~CLCD20x4()
{
  reset();
  delete m_module;
}

void CLCD20x4::reset()
{
  m_module->clear();
  m_module->setCursor(0,0);
}

void CLCD20x4::setCursor(uint8_t x, uint8_t y)
{
  m_module->setCursor(x, y);
}

void CLCD20x4::showMessage(const char *msg)
{
  reset();
  print(msg);
}

void CLCD20x4::showStatus()
{
  if(ready())
  {
    setCursor(0,0);
    print("1:");
    print(ST->sensorTHLobby()->temperature());
    print("C,");
    print(ST->sensorTHLobby()->humidity());
    print("%");
    setCursor(0,1);
    print("2:");
    print(ST->sensorTHRoom()->temperature());
    print("C,");
    print(ST->sensorTHRoom()->humidity());
    print("%");
    setCursor(0,2);
    print("mq2:");
    print(ST->sensorMQ2Room()->value());
    setCursor(0,3);
    print("                    ");
    setCursor(0,3);
    print("org:");
    print(ST->sensorCCSRoom()->getOrganic());
    print(" co2:");
    print(ST->sensorCCSRoom()->getCO2());
  }
}

}
