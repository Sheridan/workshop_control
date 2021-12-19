#include "ccs811.hpp"
#include "st.hpp"

namespace module
{

CCCS811::CCCS811()
{
  m_lastOrganic = 0;
  m_lastCO2  = 0;
  m_module = new Adafruit_CCS811();
}

void CCCS811::init()
{
  if(!m_module->begin())
  {
    ST->mainLCD()->showMessage("Failed init CCS-811");
  }
  m_module->disableInterrupt();
  while(!m_module->available());
}

CCCS811::~CCCS811()
{
  delete m_module;
}

void CCCS811::readData()
{
  A_DLOG("Reading ccs-811");
  if(m_module->available())
  {
    m_module->setEnvironmentalData(ST->sensorTHRoom()->humidity(),
                                   ST->sensorTHRoom()->temperature());
    if(m_module->readData() == 0)
    {
      m_lastOrganic = m_module->getTVOC();
      m_lastCO2  = m_module->geteCO2();
    }
    else
    {
      m_lastOrganic = -1;
      m_lastCO2  = -1;
    }
  }
}

int CCCS811::getOrganic()
{
  tryReadData();
  return m_lastOrganic;
}
int CCCS811::getCO2()
{
  tryReadData();
  return m_lastCO2;
}


}
