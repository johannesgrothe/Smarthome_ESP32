#ifndef __MQTT_Connector__
#define __MQTT_Connector__

#include "../system_settings.h"
#include "mqtt_gadget.h"
#include <cstring>

// Connector for MQTT Usage
class MQTT_Connector {
protected:

  MQTT_Gadget *mqttgadget;

  bool initialized_mqtt;

public:
  MQTT_Connector() :
    mqttgadget(nullptr),
    initialized_mqtt(false) {};

  void init_mqtt_con(MQTT_Gadget *new_mqtt_gadget) {
    initialized_mqtt = true;
    mqttgadget = new_mqtt_gadget;
  }

  bool decode_mqtt() {
    Serial.println("[WARN] decode_mqtt() not implemented");
    return false;
  }

  bool send_mqtt() {
    Serial.println("[WARN] decode_ir() not implemented");
    return false;
  }
};

#endif //__MQTT_Connector__
