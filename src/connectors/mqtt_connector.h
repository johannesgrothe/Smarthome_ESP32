#ifndef __MQTT_Connector__
#define __MQTT_Connector__

#include <Arduino.h>
#include "IPAddress.h"
#include "Client.h"
#include <PubSubClient.h>
#include "ArduinoJson.h"

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget {
protected:

  IPAddress *mqttServer;

  Client *networkClient;

  PubSubClient *mqttClient;

public:
  MQTT_Gadget();

  MQTT_Gadget(IPAddress *broker_ip, Client *network_client) :
    mqttServer(broker_ip),
    networkClient(network_client) {
  };

  bool publishMessage(char *topic, char *message) {
    bool status = true;
    uint16_t msg_len = strlen(message);
    status = status && mqttClient.beginPublish(topic, msg_len, false);
    uint16_t k;
    for (k = 0; k < msg_len; k++) {
      status = status && mqttClient.write(message[k]);
    }
    status = status && mqttClient.endPublish();
    return status;
  }
};


// Connector for MQTT Usage
class MQTT_Connector {
protected:

  MQTT_Gadget *mqttgadget;

public:
  MQTT_Connector();

  MQTT_Connector(MQTT_Gadget *mqtt_gadget) :
    mqttgadget(mqtt_gadget) {
  };

  void set_mqtt_gadget(MQTT_Gadget *mqtt_gadget) {
    mqttgadget = mqtt_gadget;
  }

  bool decode_mqtt() {
    Serial.println("[WARN] decode_mqtt() not implemented");
    return false;
  };

  bool send_mqtt() {
    Serial.println("[WARN] decode_ir() not implemented");
    return false;
  };
};

#endif