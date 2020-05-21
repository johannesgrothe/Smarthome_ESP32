#ifndef __MQTT_Gadget__
#define __MQTT_Gadget__

#include "Client.h"
#include "WiFiClient.h"
#include <PubSubClient.h>
#include "request_gadget.h"
#include <cstring>

static unsigned long getIdent(const char *json_str_input) {
  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, json_str_input);
  if (err == DeserializationError::Ok) {
    JsonObject json_obj = json_file.as<JsonObject>();
    if (json_obj["request_id"] != nullptr) {
      unsigned long ident = json_obj["request_id"].as<unsigned long>();
      return ident;
    }
  }
  return 0;
}

static bool getAck(const char *json_str_input) {
  DynamicJsonDocument json_file(2048);
  DeserializationError err = deserializeJson(json_file, json_str_input);
  if (err == DeserializationError::Ok) {
    JsonObject json_obj = json_file.as<JsonObject>();
    if (json_obj["ack"] != nullptr) {
      return json_obj["ack"].as<bool>();
    }
  }
  return false;
}


class MQTT_Gadget;

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget : public Request_Gadget {
protected:

  IPAddress *mqttServer{};

  WiFiClient networkClient{};

  PubSubClient *mqttClient{};

  uint16_t mqtt_port{};

  char username[MQTT_USERNAME_MAX_LEN]{};

  char password[MQTT_PW_MAX_LEN]{};

  bool connect_mqtt() {
    mqttClient->setServer(*mqttServer, mqtt_port);

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
        mqttClient->subscribe("smarthome/from/sys/config/set");
        mqttClient->subscribe("smarthome/from/sys/command");
        mqttClient->subscribe("smarthome/from/sys/time");
        mqttClient->subscribe("smarthome/from/gadget/set");
        mqttClient->subscribe("smarthome/from/response");
        mqttClient->subscribe("smarthome/from/code");
        mqttClient->publish("debug/out", "Controller Launched");
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
    char local_message[length + 1]{};
    for (unsigned int i = 0; i < length; i++) {
      local_message[i] = (char) payload[i];
    }
    using std::placeholders::_1;
    auto *req = new Request(topic, &local_message[0], std::bind(&Request_Gadget::sendRequest, this, _1));
    // TODO: caused Exception -> Backtrace: 0x4008ea58:0x3ffd5540 0x4008ec89:0x3ffd5560 0x4014034b:0x3ffd5580 0x40140392:0x3ffd55a0 0x4014043f:0x3ffd55c0 0x401404be:0x3ffd55e0 0x400d3569:0x3ffd5600 0x4014dd49:0x3ffd5680 0x400de1cd:0x3ffd56a0 0x400d1ec1:0x3ffd56f0 0x400d1778:0x3ffd5710 0x4008b1a1:0x3ffd5730
    addIncomingRequest(req);
  }

  void executeRequestSending(Request *req) override {
    const char *topic = req->getPath();
    const char *body = req->getBody();
    logger.print("System / MQTT", "publishing on '");
    logger.add(topic);
    logger.add("'");
    bool status = true;
    uint16_t msg_len = strlen(body);
    status = status && mqttClient->beginPublish(topic, msg_len, false);
    uint16_t k;
    for (k = 0; k < msg_len; k++) {
      status = status && mqttClient->write(body[k]);
    }
    status = status && mqttClient->endPublish();
    if (status)
      logger.addln("OK");
    else
      logger.addln("ERR");
  }

public:
  MQTT_Gadget() :
    Request_Gadget() {};

  MQTT_Gadget(IPAddress *broker_ip, WiFiClient *network_client) :
    Request_Gadget(),
    mqttServer(broker_ip) {
    connect_mqtt();
  };

  MQTT_Gadget(JsonObject data, WiFiClient *network_client) :
    Request_Gadget(MQTT_G, data) {
    logger.println("Creating MQTT Gadget");
    networkClient = WiFiClient();
    logger.incIndent();
    mqttClient = new PubSubClient(networkClient);
    bool everything_ok = true;
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
      mqtt_port = data["port"].as<unsigned int>();
      logger.print(LOG_DATA, "Port: ");
      logger.addln(mqtt_port);
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
    logger.decIndent();
    request_gadget_is_ready = everything_ok;
  };


  void refresh() override {
    if (!request_gadget_is_ready) {
      return;
    }
    if (!mqttClient->connected()) {
      connect_mqtt();
    }
    mqttClient->loop();
    sendQueuedItems();
  }
};

#endif //__MQTT_Gadget__