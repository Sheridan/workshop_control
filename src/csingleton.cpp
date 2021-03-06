#include "csingleton.hpp"
CSingleTon* CSingleTon::m_singleton = nullptr;

CSingleTon::CSingleTon()
{
  // ST_TONULL(sensorPressure );
  ST_TONULL(keyboard       );
  ST_TONULL(releRoomCooler );
  ST_TONULL(releRoomHeater );
  ST_TONULL(releExhaust    );
  ST_TONULL(integratedDiode);
  ST_TONULL(sensorTHLobby  );
  ST_TONULL(sensorTHRoom   );
  ST_TONULL(sensorTHStreet );
  ST_TONULL(sensorTHServer );
  ST_TONULL(sensorMQ2Room  );
  ST_TONULL(sensorCCSRoom  );
  ST_TONULL(mainLCD        );
  ST_TONULL(eth            );
}

void CSingleTon::init()
{
  // ST_INIT(sensorPressure );
  ST_INIT(keyboard       );
  ST_INIT(releRoomCooler );
  ST_INIT(releRoomHeater );
  ST_INIT(releExhaust    );
  ST_INIT(integratedDiode);
  ST_INIT(mainLCD        );
  ST_INIT(eth            );
  ST_INIT(sensorTHLobby  );
  ST_INIT(sensorTHRoom   );
  ST_INIT(sensorTHStreet );
  ST_INIT(sensorTHServer );
  ST_INIT(sensorMQ2Room  );
  ST_INIT(sensorCCSRoom  );
}

CSingleTon *CSingleTon::instance()
{
  if(!m_singleton)
  {
    m_singleton = new CSingleTon();
  }
  return m_singleton;
}
