#ifndef __MQTT_Gadget__
#define __MQTT_Gadget__

#include "Client.h"
#include "WiFiClient.h"
#include <PubSubClient.h>
#include "request_gadget.h"
#include <cstring>

// Gadget to communicate with MQTT Endpoint
class MQTT_Gadget : public Request_Gadget {
//class MQTT_Gadget {
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
        mqttClient->subscribe("homebridge/from/set");
        mqttClient->subscribe("homebridge/from/response");
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
    char local_message[length + 1]{};
    for (unsigned int i = 0; i < length; i++) {
      local_message[i] = (char) payload[i];
    }
    if (strcmp(topic, "homebridge/from/response") == 0) {
      // TODO: if corrupt json arrives, program crashes

      bool resp_status = local_message[7] == 't';
      resp_status = resp_status && local_message[8] == 'r';
      resp_status = resp_status && local_message[9] == 'u';
      resp_status = resp_status && local_message[10] == 'e';

      int status_code = 0;
      if (resp_status) {
        status_code = 200;
      } else {
        status_code = 400;
      }
      setResponseRequest(topic, &local_message[0], status_code);
      has_request = true;
    } else {
      setRequest(topic, &local_message[0], REQ_MQTT);
      has_request = true;
    }
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
    Request_Gadget() {
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

  bool publishMessageAndWaitForAnswer(const char *topic, const char *message) {
    if (!request_gadget_is_ready) {
      return false;
    }
    byte count = 0;
    if (publishMessage(topic, message, true)){
      refresh();
      while (count < 10 && !has_response) {
        logger.add(".");
        refresh();
        count ++;
        delay(100);
      }
      if (hasResponse()) {
        logger.addln(getResponseStatusCode());
        return getResponseStatusCode() == 200;
      }
    }
    logger.addln("Error");
    return false;
  }

  bool publishMessage(const char *topic, const char *message, bool wait_for_answer = false) {
    if (!request_gadget_is_ready) {
      return false;
    }
    logger.print("System / MQTT", "publishing on '");
    logger.add(topic);
    if (wait_for_answer)
      logger.add("': ");
    else
      logger.addln();
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

  void
  sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
              const char *req_body) override {

  }

  void
  sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
              JsonObject req_body) override {

  }

  void sendAnswer(const char *req_body, int status_code) override {

  };

  void sendAnswer(JsonObject req_body, int status_code) override {

  };

  void refresh() override {
    if (!request_gadget_is_ready) {
      return;
    }
    if (!mqttClient->connected()) {
      connect_mqtt();
    }
    mqttClient->loop();
  }
};

#endif //__MQTT_Gadget__