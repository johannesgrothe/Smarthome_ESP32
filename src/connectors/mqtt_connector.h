#ifndef __MQTT_Connector__
#define __MQTT_Connector__

#include "IPAddress.h"
#include "Client.h"
#include "WiFiClient.h"
#include <PubSubClient.h>
#include "request_connector.h"

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget : public Request_Gadget {
//class MQTT_Gadget {
protected:

  IPAddress *mqttServer{};

  WiFiClient *networkClient{};

  PubSubClient *mqttClient{};

  uint16_t port{};

  char username[MQTT_USERNAME_MAX_LEN]{};

  char password[MQTT_PW_MAX_LEN]{};

public:
  MQTT_Gadget() :
    Request_Gadget() {
  };

  MQTT_Gadget(IPAddress *broker_ip, WiFiClient *network_client) :
    Request_Gadget(),
    mqttServer(broker_ip),
    networkClient(network_client) {
    connect_mqtt();
  };

  MQTT_Gadget(JsonObject data, WiFiClient *network_client) :
    Request_Gadget() {
    logger.println("Creating MQTT Gadget");
    logger.incIntent();
    networkClient = network_client;
    mqttClient = new PubSubClient(*networkClient);
    bool everything_ok = true;
//    setRequest("ddd", "ddd", REQ_MQTT);
    // Reads the IP from the JSON
    if (data["ip"] != nullptr) {
      char ip_str[15]{};
      strncpy(ip_str, data["ip"].as<const char *>(), 15);
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
      logger.print(LOG_DATA, "IP: ");
      logger.add(ip_arr[0]);
      logger.add(".");
      logger.add(ip_arr[1]);
      logger.add(".");
      logger.add(ip_arr[2]);
      logger.add(".");
      logger.addln(ip_arr[3]);
    } else {
      everything_ok = false;
      logger.println(LOG_ERR, "'ip' missing in config.");
    }

    // Reads the Port from the JSON
    if (data["port"] != nullptr) {
      port = data["port"].as<unsigned int>();
      logger.print(LOG_DATA, "Port: ");
      logger.addln(port);
    } else {
      everything_ok = false;
      logger.println(LOG_ERR, "'port' missing in config.");
    }

    // Reads the Username from JSON
    if (data["username"] != nullptr) {
      strncpy(username, data["username"].as<const char *>(), MQTT_USERNAME_MAX_LEN);
      logger.print(LOG_DATA, "Username: ");
      logger.addln(username);
    } else {
      // everything_ok = false;
      logger.println(LOG_ERR, "'username' missing in config.");
    }

    // Reads the Password from JSON
    if (data["password"] != nullptr) {
      strncpy(password, data["password"].as<const char *>(), MQTT_USERNAME_MAX_LEN);

      logger.print(LOG_DATA, "Password: ");
      logger.addln(username);
    } else {
      // everything_ok = false;
      logger.println(LOG_ERR, "'password' missing in config.");
    }

    connect_mqtt();
    logger.decIntent();
    is_initialized = everything_ok;
  };

  bool connect_mqtt() {
    mqttClient->setServer(*mqttServer, port);

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    mqttClient->setCallback(std::bind(&MQTT_Gadget::callback, this, _1, _2, _3));

    logger.print(LOG_DATA, "Connecting to Broker ");
    uint8_t conn_count = 0;
    while (!mqttClient->connected()) {
      if (mqttClient->connect("esp32_test_client")) {
        logger.addln("OK");
        mqttClient->subscribe("debug/in");
        mqttClient->publish("debug/out", "hallo welt");
        return true;
      } else {
        if (conn_count > 5) {
          logger.addln("Failed.");
          logger.println(LOG_ERR, "No Connection to Broker could be established.");
          break;
        }
        logger.add(".");
        delay(1000);
        conn_count++;
      }
    }
    return false;
  }

  void callback(char *topic, byte *payload, unsigned int length) {
    char local_message[length + 1];
    for (uint32_t i = 0; i < length; i++) {
      local_message[i] = (char) payload[i];
    }
    local_message[length] = '\0';

    setRequest(&local_message[0], topic, REQ_MQTT);

//    strncpy(path, topic, REQUEST_PATH_LEN_MAX);
//
//    strncpy(body, &local_message[0], REQUEST_BODY_LEN_MAX);

    //Logging
    logger.printname("MQTT", "[");
    logger.add(path);
    logger.add("] ");
    logger.addln(body);
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

  void refresh() {
    if (!is_initialized) {
      return;
    }
    mqttClient->loop();
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

// Connector for MQTT Usage
class Homebridge_Connector {
protected:

  MQTT_Gadget *mqttgadget;

  bool initialized_mqtt;

public:
  Homebridge_Connector() :
    mqttgadget(nullptr),
    initialized_mqtt(false) {};

  void init_mqtt_con(MQTT_Gadget *new_mqtt_gadget) {
    initialized_mqtt = true;
    mqttgadget = new_mqtt_gadget;
  }

  virtual void registerGadget() {
    logger.println(LOG_ERR, "registerGadget() is not implemented.");
  }

  virtual void decodeMQTTCommand() {
    logger.println(LOG_ERR, "decodeMQTTCommand() is not implemented.");
  }

  virtual void updateStatusOnServer() {
    logger.println(LOG_ERR, "updateStatusOnServer() is not implemented.");
  }
};

#endif //__MQTT_Connector__
