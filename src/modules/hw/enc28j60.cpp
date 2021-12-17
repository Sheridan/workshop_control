#include "enc28j60.hpp"
#include "st.hpp"

namespace module
{
const uint8_t CENC28J60::m_mac[6] = { 0x74,0x69,0x69,0x08,0x16,0x32 };

CENC28J60::CENC28J60()
{
  if(Ethernet.begin(m_mac))
  {
    m_server = new EthernetServer(80);
    m_server->begin();
  }
  else
  {
    A_DLOG("Failed init ethernet");
    ST->mainLCD()->showMessage("Failed init ethernet");
    while(true);
  }
}

void CENC28J60::init()
{
  A_DLOG("Ethernet done");
  A_DLOG(Ethernet.localIP());
  delay(2000);
}

CENC28J60::~CENC28J60()
{
  delete m_server;
}

void CENC28J60::checkQuery()
{
  EthernetClient client = m_server->available();
  if(client)
  {
    ST->integratedDiode()->on();
    while(client.available() > 0)
    {
      String request = client.readString();
      A_DLOG(request);
      if(request.lastIndexOf("GET / ") == 0)
      {
        homePage(&client);
      }
      if(request.lastIndexOf("GET /metrics ") == 0)
      {
        prometheusPage(&client);
      }
    }
    ST->integratedDiode()->off();
  }
}

void CENC28J60::headers(EthernetClient *client, const char *contentType)
{
  client->println("HTTP/1.1 200 OK");
  client->print("Content-Type: ");
  client->println(contentType);
  // client->println("Connection: close");
  client->println("Arduino: true");
  client->print("\r\n");
}

void CENC28J60::homePage(EthernetClient *client)
{
  A_DLOG("Home page request");
  headers(client, "text/html");
  client->println("<h1>Home page</h1>");
  client->stop();
  // m_server->write("<h1>Home page</h1>");
  // helpers::CDecimalFloatSensors sensors;

  // BufferFiller bfill;
  // bfill = m_eth->tcpOffset();
  // bfill.emit_p(
  //   PSTR(
  //   "<html><head><meta charset='UTF-8'>"
  //   "<h1>Температура в прихожей: $D.$D &#176;C</h1>"
  //   "<h1>Температура в комнате: $D.$D &#176;C</h1>"
  //   "</html> "
  //   ),
  //   sensors.temperatureLobby()->decimal(), sensors.temperatureLobby()->fract(),
  //   sensors.temperatureRoom()->decimal(), sensors.temperatureRoom()->fract()
  // );
  // return bfill.position();
}
#define A_PROM_HELP(_name,_description,_type) client->print("# HELP " _name " " _description "\n# TYPE " _name " "  _type "\n");
#define A_PROM_METRIC(_name,_tags,_value) client->print(_name "{" _tags "} "); client->print(_value); client->print("\n");
void CENC28J60::prometheusPage(EthernetClient *client)
{
  A_DLOG("Prometheus request");
  headers(client, "text/plain");
  A_PROM_HELP("ctrl_temperature", "Temperature", "gauge");
  A_PROM_METRIC("ctrl_temperature","room=\"lobby\",type=\"real\"",ST->sensorTHLobby()->temperature());
  A_PROM_METRIC("ctrl_temperature","room=\"room\",type=\"real\"",ST->sensorTHRoom()->temperature());
  A_PROM_METRIC("ctrl_temperature","room=\"lobby\",type=\"heatindex\"",ST->sensorTHLobby()->heatIndex());
  A_PROM_METRIC("ctrl_temperature","room=\"room\",type=\"heatindex\"",ST->sensorTHRoom()->heatIndex());
  A_PROM_HELP("ctrl_humidity", "Humidity", "gauge");
  A_PROM_METRIC("ctrl_humidity","room=\"lobby\"",ST->sensorTHLobby()->humidity());
  A_PROM_METRIC("ctrl_humidity","room=\"room\"",ST->sensorTHRoom()->humidity());
  A_PROM_HELP("ctrl_air_quality", "Air quality", "gauge");
  A_PROM_METRIC("ctrl_air_quality","room=\"room\" type=\"MQ-2\"",ST->sensorMQ2Room()->value());
  A_PROM_METRIC("ctrl_air_quality","room=\"room\" type=\"organic\"",ST->sensorCCSRoom()->getOrganic());
  A_PROM_METRIC("ctrl_air_quality","room=\"room\" type=\"co2\"",ST->sensorCCSRoom()->getCO2());
  A_PROM_HELP("ctrl_effector_state", "Effector state", "gauge");
  A_PROM_METRIC("ctrl_effector_state","device=\"main_exhaust\" type=\"cooler\"",ST->releExhaust()->state());
  A_PROM_METRIC("ctrl_effector_state","device=\"room_heater\" type=\"heater\"",ST->releRoomHeater()->state());
  client->stop();
}

}
