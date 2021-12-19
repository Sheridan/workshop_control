#pragma once
#include "modules/cmodule.hpp"
#include <UIPEthernet.h>

namespace module
{
class CENC28J60 : public CModule
{
public:
  CENC28J60();
  ~CENC28J60();
  void init();
  void checkQuery();

private:
  static const uint8_t m_mac[6];
  EthernetServer *m_server;
  void headers(EthernetClient *client, const char *contentType);
  void homePage(EthernetClient *client);
  void prometheusPage(EthernetClient *client);
  void reset(EthernetClient *client);


};


}
