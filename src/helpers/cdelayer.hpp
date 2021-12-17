#pragma once
namespace helper
{
class CDelayer
{
public:
  CDelayer(const unsigned long &delay);
  ~CDelayer();
protected:
  bool ready();
private:
  unsigned long m_delay;
  unsigned long m_lastTime;

};


}
