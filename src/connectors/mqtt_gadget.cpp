#include "mqtt_gadget.h"

bool MQTT_Gadget::connect_mqtt() {
  mqttClient_->setServer(*mqttServer_, mqtt_port_);

  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  mqttClient_->setCallback(std::bind(&MQTT_Gadget::callback, this, _1, _2, _3));

  logger.print(LOG_TYPE::DATA, "Connecting to Broker ");
  uint8_t conn_count = 0;
  while (!mqttClient_->connected()) {
    if (mqttClient_->connect("esp32_test_client")) {
      logger.addln("OK");
      mqttClient_->subscribe("debug/in");
      mqttClient_->subscribe("smarthome/from/sys/config/set");
      mqttClient_->subscribe("smarthome/from/sys/command");
      mqttClient_->subscribe("smarthome/from/sys/time");
      mqttClient_->subscribe("smarthome/from/gadget/set");
      mqttClient_->subscribe("smarthome/from/response");
      mqttClient_->subscribe("smarthome/from/code");
      mqttClient_->publish("debug/out", "Controller Launched");
      return true;
    } else {
      if (conn_count > 5) {
        logger.addln("Failed.");
        logger.println(LOG_TYPE::ERR, "No Connection to Broker could be established.");
        break;
      }
      logger.add(".");
      delay(1000);
      conn_count++;
    }
  }
  return false;
}

void MQTT_Gadget::callback(char *topic, const byte *payload, const unsigned int length) {
  char local_message[length + 1]{};
  for (unsigned int i = 0; i < length; i++) {
    local_message[i] = (char) payload[i];
  }
  using std::placeholders::_1;
  auto *req = new Request(topic, &local_message[0], std::bind(&Request_Gadget::sendRequest, this, _1));
  // TODO: caused Exception -> Backtrace: 0x4008ea58:0x3ffd5540 0x4008ec89:0x3ffd5560 0x4014034b:0x3ffd5580 0x40140392:0x3ffd55a0 0x4014043f:0x3ffd55c0 0x401404be:0x3ffd55e0 0x400d3569:0x3ffd5600 0x4014dd49:0x3ffd5680 0x400de1cd:0x3ffd56a0 0x400d1ec1:0x3ffd56f0 0x400d1778:0x3ffd5710 0x4008b1a1:0x3ffd5730
  addIncomingRequest(req);
}

void MQTT_Gadget::executeRequestSending(Request *req) {
  const char *topic = req->getPath();
  const char *body = req->getBody();
  logger.print("System / MQTT", "publishing on '");
  logger.
    add(topic);
  logger.add("'");
  bool status = true;
  uint16_t msg_len = strlen(body);
  status = status && mqttClient_->beginPublish(topic, msg_len, false);
  uint16_t k;
  for (
    k = 0;
    k < msg_len;
    k++) {
    status = status && mqttClient_->write(body[k]);
  }
  status = status && mqttClient_->endPublish();
  if (status)
    logger.addln("OK");
  else
    logger.addln("ERR");
}

MQTT_Gadget::MQTT_Gadget() :
  WiFiGadget(),
  Request_Gadget() {};

MQTT_Gadget::MQTT_Gadget(JsonObject data) :
  WiFiGadget(data),
  Request_Gadget(RequestGadgetType::MQTT_G, data) {
  if (data.isNull()) {
    logger.println(LOG_TYPE::ERR, "No valid MQTT configuration.");
    return;
  }
  if (wifiIsInitialized()) {
    logger.println("Creating MQTT Gadget");
    network_client_ = WiFiClient();
    logger.incIndent();
    mqttClient_ = new PubSubClient(network_client_);
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
      mqttServer_ = new IPAddress(ip_arr[0], ip_arr[1], ip_arr[2], ip_arr[3]);
      logger.print(LOG_TYPE::DATA, "IP: ");
      logger.add(ip_arr[0]);
      logger.add(".");
      logger.add(ip_arr[1]);
      logger.add(".");
      logger.add(ip_arr[2]);
      logger.add(".");
      logger.addln(ip_arr[3]);
    } else {
      everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'ip' missing in config.");
    }

    // Reads the Port from the JSON
    if (data["port"] != nullptr) {
      mqtt_port_ = data["port"].as<unsigned int>();
      logger.print(LOG_TYPE::DATA, "Port: ");
      logger.addln(mqtt_port_);
    } else {
      everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'port' missing in config.");
    }

    // Reads the Username from JSON
    if (data["username"] != nullptr) {
      strncpy(username_, data["username"].as<const char *>(), MQTT_USERNAME_MAX_LEN);
      logger.print(LOG_TYPE::DATA, "Username: ");
      logger.addln(username_);
    } else {
      // everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'username' missing in config.");
    }

    // Reads the Password from JSON
    if (data["password"] != nullptr) {
      strncpy(password_, data["password"].as<const char *>(), MQTT_USERNAME_MAX_LEN);

      logger.print(LOG_TYPE::DATA, "Password: ");
      logger.addln(username_);
    } else {
      // everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'password' missing in config.");
    }

    connect_mqtt();
    logger.decIndent();
    request_gadget_is_ready_ = everything_ok;
  } else {
    logger.println(LOG_TYPE::ERR, "Cannot initialize MQTT: WiFi not working.");
    request_gadget_is_ready_ = false;
  }
}

void MQTT_Gadget::refresh() {
  if (!request_gadget_is_ready_) {
    return;
  }
  if (!mqttClient_->connected()) {
    connect_mqtt();
  }
  mqttClient_->loop();
  sendQueuedItems();
}
