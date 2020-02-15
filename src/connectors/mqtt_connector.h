#ifndef __MQTT_Connector__
#define __MQTT_Connector__

#include <Arduino.h>
#include "IPAddress.h"
#include "Client.h"
#include "WiFiClient.h"
#include <PubSubClient.h>
#include "ArduinoJson.h"
#include <cstring>
#include "code_connector.h"

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget {
protected:

  IPAddress *mqttServer{};

  WiFiClient *networkClient{};

  PubSubClient *mqttClient{};

  uint16_t port{};

  char *username{};

  char *password{};

  bool is_initialized;

public:
  MQTT_Gadget() :
    is_initialized(false) {
  };

  MQTT_Gadget(IPAddress *broker_ip, WiFiClient *network_client) :
    mqttServer(broker_ip),
    networkClient(network_client),
    is_initialized(true) {
    connect_mqtt();
  };

  MQTT_Gadget(JsonObject data, WiFiClient *network_client) {
    Serial.println("   [INFO] Creating MQTT-Gadget");
    networkClient = network_client;
    mqttClient = new PubSubClient(*networkClient);
    bool everything_ok = true;

    // Reads the IP from the JSON
    if (data["ip"] != nullptr) {
      char ip_str[16];
      strcpy(ip_str, data["ip"].as<const char *>());
      unsigned int ip_arr[4];
      uint8_t count = 0;
      char *part;
      part = strtok(ip_str, ".");
      ip_arr[count] = atoi(part);
      while (count < 3) {
        part = strtok(nullptr, ".");
        count++;
        ip_arr[count] = atoi(part);
      }
      mqttServer = new IPAddress(ip_arr[0], ip_arr[1], ip_arr[2], ip_arr[3]);
      Serial.printf("     => IP: %d.%d.%d.%d\n", ip_arr[0], ip_arr[1], ip_arr[2], ip_arr[3]);
    } else {
      everything_ok = false;
      Serial.println("     => [ERR] \"ip\" missing in config.");
    }

    // Reads the Port from the JSON
    if (data["port"] != nullptr) {
      port = data["port"].as<unsigned int>();
      Serial.printf("     => Port: %d\n", port);
    } else {
      everything_ok = false;
      Serial.println("     => [ERR] \"port\" missing in config.");
    }

    // Reads the Username from JSON
    if (data["username"] != nullptr) {
      port = data["username"].as<unsigned int>();
      Serial.printf("     => Username: %d\n", port);
    } else {
      // everything_ok = false;
      Serial.println("     => no \"username\" in config.");
    }

    // Reads the Password from JSON
    if (data["password"] != nullptr) {
      port = data["password"].as<unsigned int>();
      Serial.printf("     => Password: %d\n", port);
    } else {
      // everything_ok = false;
      Serial.println("     => no \"password\" in config.");
    }

    connect_mqtt();

    is_initialized = everything_ok;
  };

  bool connect_mqtt() {

    mqttClient->setServer(*mqttServer, port);

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    mqttClient->setCallback(std::bind(&MQTT_Gadget::callback, this, _1, _2, _3));

    Serial.print("     => Connecting to Broker ");
    uint8_t conn_count = 0;
    while (!mqttClient->connected()) {
      if (mqttClient->connect("esp32_test_client")) {
        Serial.println("OK");
        return true;
      } else {
        if (conn_count > 5) {
          Serial.println("Failed.");
          Serial.println("     => [ERR] No Connection to Broker could be established.");
          break;
        }
        Serial.print(". ");
        delay(1000);
        conn_count++;
      }
    }
    return false;
  }

  void callback(char *topic, byte *payload, unsigned int length) {
    char message[length + 1];
    for (uint32_t i = 0; i < length; i++) {
      message[i] = (char) payload[i];
    }
    message[length] = '\0';

    if ((strcmp(topic, "debug/in") == 0)) {
      Serial.printf("[DEBUG]: %s\n", message);
    } else if ((strcmp(topic, "homebridge/from/set") == 0)) {
      Serial.printf("[HOMEBRIDGE SET]: %s\n", message);
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, message);
//      forwardCommand(&doc);
    } else if ((strcmp(topic, "homebridge/from/response") == 0)) {
      Serial.printf("[RESPONSE]: %s\n", message);
    } else {
      Serial.printf("[MSG '%s']: %s\n", topic, message);
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

  void refresh() {
    if (!is_initialized) {
      return;
    }
  }
};


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
