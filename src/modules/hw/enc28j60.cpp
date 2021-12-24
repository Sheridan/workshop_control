#include "enc28j60.hpp"
#include "st.hpp"
#include "debug.hpp"
#include "ccontroller.hpp"
void(* resetFunc) (void) = 0;

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
    while(!client.available()) { delay(4); }

    {
      String request = client.readString();
      // A_DLOG(request);
      if(request.lastIndexOf("GET / ") == 0)
      {
        homePage(&client);
      }
      if(request.lastIndexOf("GET /metrics ") == 0)
      {
        prometheusPage(&client);
      }
      if(request.lastIndexOf("GET /reset ") == 0)
      {
        reset(&client);
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

void CENC28J60::closeClient(EthernetClient *client)
{
  delay(8);
  client->stop();
}

void CENC28J60::homePage(EthernetClient *client)
{
  A_DLOG("Home page request");
  headers(client, "text/html");
  client->println("<h1>Home page</h1>");
  client->println("<a href='/metrics'>Metrics</h1><br/>");
  client->println("<a href='/reset'>Reset</h1>");
  closeClient(client);
}
#define A_PROM_HELP(_name,_description,_type) client->print("# HELP " _name " " _description "\n# TYPE " _name " "  _type "\n");
#define A_PROM_METRIC(_name,_tags,_value) client->print(_name "{" _tags "} "); client->print(_value); client->print("\n");
#define A_PROM_TAG(_name,_value) _name "=\"" _value "\""
void CENC28J60::prometheusPage(EthernetClient *client)
{
  A_DLOG("Prometheus request");
  headers(client, "text/plain");
  A_PROM_HELP("ctrl_temperature", "Temperature", "gauge");
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "lobby") "," A_PROM_TAG("type", "real")              ,ST->sensorTHLobby()->temperature());
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "room")  "," A_PROM_TAG("type", "real")              ,ST->sensorTHRoom()->temperature());
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "lobby") "," A_PROM_TAG("type", "heatindex")         ,ST->sensorTHLobby()->heatIndex());
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "room")  "," A_PROM_TAG("type", "heatindex")         ,ST->sensorTHRoom()->heatIndex());
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "room")  "," A_PROM_TAG("type", "target")            ,A_TARGET_ROOM_TEMPERATURE);
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "room")  "," A_PROM_TAG("type", "target_stage0_high"),A_TARGET_ROOM_TEMPERATURE_STAGE0_HIGH);
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "room")  "," A_PROM_TAG("type", "target_stage0_low") ,A_TARGET_ROOM_TEMPERATURE_STAGE0_LOW);
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "room")  "," A_PROM_TAG("type", "target_stage1_high"),A_TARGET_ROOM_TEMPERATURE_STAGE1_HIGH);
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "room")  "," A_PROM_TAG("type", "target_stage1_low") ,A_TARGET_ROOM_TEMPERATURE_STAGE1_LOW);
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "street")  "," A_PROM_TAG("type", "real")            ,ST->sensorTHStreet()->temperature());
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "server")  "," A_PROM_TAG("type", "real")            ,ST->sensorTHServer()->temperature());
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "street")  "," A_PROM_TAG("type", "heatindex")       ,ST->sensorTHStreet()->heatIndex());
  A_PROM_METRIC("ctrl_temperature",A_PROM_TAG("room", "server")  "," A_PROM_TAG("type", "heatindex")       ,ST->sensorTHServer()->heatIndex());

  A_PROM_HELP("ctrl_humidity", "Humidity", "gauge");
  A_PROM_METRIC("ctrl_humidity",A_PROM_TAG("room", "lobby"),ST->sensorTHLobby()->humidity());
  A_PROM_METRIC("ctrl_humidity",A_PROM_TAG("room", "room") ,ST->sensorTHRoom()->humidity());
  A_PROM_METRIC("ctrl_humidity",A_PROM_TAG("room", "street"),ST->sensorTHStreet()->humidity());
  A_PROM_METRIC("ctrl_humidity",A_PROM_TAG("room", "server") ,ST->sensorTHServer()->humidity());
  A_PROM_HELP("ctrl_air_quality", "Air quality", "gauge");
  A_PROM_METRIC("ctrl_air_quality",A_PROM_TAG("room", "room") "," A_PROM_TAG("type", "mq2")    ,ST->sensorMQ2Room()->value());
  A_PROM_METRIC("ctrl_air_quality",A_PROM_TAG("room", "room") "," A_PROM_TAG("type", "organic"),ST->sensorCCSRoom()->getOrganic());
  A_PROM_METRIC("ctrl_air_quality",A_PROM_TAG("room", "room") "," A_PROM_TAG("type", "co2")    ,ST->sensorCCSRoom()->getCO2());
  A_PROM_METRIC("ctrl_air_quality",A_PROM_TAG("room", "room") "," A_PROM_TAG("type", "target_mq2")    ,A_TARGET_ROOM_MQ2);
  A_PROM_METRIC("ctrl_air_quality",A_PROM_TAG("room", "room") "," A_PROM_TAG("type", "target_organic"),A_TARGET_ROOM_ORGANIC);
  A_PROM_METRIC("ctrl_air_quality",A_PROM_TAG("room", "room") "," A_PROM_TAG("type", "target_co2")    ,A_TARGET_ROOM_CO2);

  A_PROM_HELP("ctrl_effector_state", "Effector state", "gauge");
  A_PROM_METRIC("ctrl_effector_state",A_PROM_TAG("device","main_exhaust") "," A_PROM_TAG("type","cooler") "," A_PROM_TAG("state", +ST->releExhaust()->state()+)    "," A_PROM_TAG("reason", +ST->releExhaust()->reason()+), 1);
  A_PROM_METRIC("ctrl_effector_state",A_PROM_TAG("device","room_heater")  "," A_PROM_TAG("type","heater") "," A_PROM_TAG("state", +ST->releRoomHeater()->state()+) "," A_PROM_TAG("reason", +ST->releRoomHeater()->reason()+), 1);
  A_PROM_METRIC("ctrl_effector_state",A_PROM_TAG("device","room_cooler")  "," A_PROM_TAG("type","cooler") "," A_PROM_TAG("state", +ST->releRoomCooler()->state()+) "," A_PROM_TAG("reason", +ST->releRoomCooler()->reason()+), 1);
  closeClient(client);
}

void CENC28J60::reset(EthernetClient *client)
{
  A_DLOG("Reset request");
  headers(client, "text/html");
  client->println("<h1>Reseting...</h1>");
  client->flush();
  closeClient(client);
  while(client->connected()) { delay(1000); }
  resetFunc();
}

}
