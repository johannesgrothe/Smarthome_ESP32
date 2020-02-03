#ifndef __MQTT_Connector__
#define __MQTT_Connector__

#include <Arduino.h>
#include "IPAddress.h"
#include "Client.h"
#include <PubSubClient.h>
#include "ArduinoJson.h"
#include <string.h>

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget {
protected:

  IPAddress *mqttServer;

  Client *networkClient;

  PubSubClient *mqttClient;

  uint16_t port;

  bool is_initialized;

public:
  MQTT_Gadget();

  MQTT_Gadget(IPAddress *broker_ip, Client *network_client) :
    mqttServer(broker_ip),
    networkClient(network_client) {
    connect_mqtt();
  };

  explicit MQTT_Gadget(JsonObject data) :
    mqttServer(),
    networkClient(),
    mqttClient() {
    Serial.println("   [INFO] Creating MQTT-Gadget");
    bool everything_ok = true;
    if (data["ip"] != nullptr) {
      char ip_str[16];
      strcpy(ip_str, data["ip"].as<const char*>());
      unsigned int ip_arr[4];
      uint8_t count = 0;
      char * part;
      part = strtok(ip_str, ".");
      ip_arr[count] = atoi(part);
      while (count < 3) {
        part = strtok(nullptr, ".");
        count ++;
        ip_arr[count] = atoi(part);
      }
      mqttServer = new IPAddress(ip_arr[0], ip_arr[1], ip_arr[2], ip_arr[3]);
      Serial.printf("     => IP: %d.%d.%d.%d\n", ip_arr[0], ip_arr[1], ip_arr[2], ip_arr[3]);
    } else {
      everything_ok = false;
      Serial.println("     => [ERR] \"ip\" nicht spezifiziert.");
    }
    if (data["port"] != nullptr) {
      port = data["port"].as<unsigned int>();
      Serial.printf("     => Port: %d\n", port);
    } else {
      everything_ok = true;
      Serial.println("     => [ERR] \"port\" nicht spezifiziert.");
    }

    connect_mqtt();

    is_initialized = everything_ok;
  };

  bool connect_mqtt() {

//    uint32_t ip = mqttServer->uint32_t();

    uint32_t ip = 19216817860;

    mqttClient->setServer(ip, port);

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    mqttClient->setCallback(std::bind(&MQTT_Gadget::callback, this, _1, _2, _3));

    while (!mqttClient->connected()) {
      Serial.print("   => Connecting to Broker ");
      if (mqttClient->connect("esp32_test_client")) {
        Serial.println("OK");
      } else {
        Serial.print(". ");
        delay(3000);
      }
    }
  }

  void callback(char *topic, byte *payload, unsigned int length) {
    char message[length + 1];
    for (uint32_t i = 0; i < length; i++) {
      message[i] = (char) payload[i];
    }
    message[length] = '\0';

    if ((strcmp(topic, "debug/in") == 0)) {
#ifdef LOG_MESSAGES
      Serial.printf("[DEBUG]: %s\n", message);
#endif
    } else if ((strcmp(topic, "homebridge/from/set") == 0)) {
#ifdef LOG_MESSAGES
      Serial.printf("[HOMEBRIDGE SET]: %s\n", message);
#endif
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, message);
//      forwardCommand(&doc);
    } else if ((strcmp(topic, "homebridge/from/response") == 0)) {
#ifdef LOG_MESSAGES
      Serial.printf("[RESPONSE]: %s\n", message);
#endif
    } else {
#ifdef LOG_MESSAGES
      Serial.printf("[MSG '%s']: %s\n", topic, message);
#endif
    }
  }

  bool publishMessage(char *topic, char *message) {
    bool status = true;
    uint16_t msg_len = strlen(message);
    status = status && mqttClient->beginPublish(topic, msg_len, false);
    uint16_t k;
    for (k = 0; k < msg_len; k++) {
      status = status && mqttClient->write(message[k]);
    }
    status = status && mqttClient->endPublish();
    return status;
  }

  bool isInitialized() {
    return is_initialized;
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
  }

  bool send_mqtt() {
    Serial.println("[WARN] decode_ir() not implemented");
    return false;
  }
};

#endif
