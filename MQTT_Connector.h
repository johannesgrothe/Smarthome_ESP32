#ifndef __MQTT_Connector__
#define __MQTT_Connector__

#include <Arduino.h>

class MQTT_Connector
{  
  protected:

    bool decode_mqtt()
    {
      return false;
    };

    bool send_mqtt()
    {
      return false;
    };

  public:

    MQTT_Connector();
};

#endif