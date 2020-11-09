#include "mqtt_gadget.h"
#include <sstream>
#include <utility>

bool MQTT_Gadget::connect_mqtt() {
  mqttClient_->setServer(mqttServer_, mqtt_port_);

  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  mqttClient_->setCallback(std::bind(&MQTT_Gadget::callback, this, _1, _2, _3));

  logger.print(LOG_TYPE::DATA, "Connecting to Broker ");
  uint8_t conn_count = 0;
  while (!mqttClient_->connected()) {
    bool connected = false;
    if (has_credentials_) {
      connected = mqttClient_->connect(client_name_.c_str(), username_.c_str(), password_.c_str());
    } else {
      connected = mqttClient_->connect(client_name_.c_str());
    }

    if (connected) {
      logger.println("OK");

      logger.println(LOG_TYPE::DATA, "Subscribing to topics:");
      logger.incIndent();
      for (const auto& list_path: broadcast_request_paths) {
        subscibe_to_topic(list_path);
      }

      for (const auto& list_path: system_request_paths) {
        subscibe_to_topic(list_path);
      }

      for (const auto& list_path: additional_request_paths) {
        subscibe_to_topic(list_path);
      }

      logger.decIndent();

      mqttClient_->publish("smarthome/debug/out", "Controller Launched");
      return true;
    } else {
      if (conn_count > 5) {
        logger.println("Failed.");
        logger.println(LOG_TYPE::ERR, "No Connection to Broker could be established.");
        break;
      }
      logger.print(".");
      delay(1000);
      conn_count++;
    }
  }
  return false;
}

void MQTT_Gadget::callback(char *topic, const byte *payload, const unsigned int length) {
  std::stringstream local_topic;
  local_topic << topic;

  logger.println("Callback called");

  std::stringstream local_message;
  for (unsigned int i = 0; i < length; i++) {
    local_message << (char) payload[i];
  }

  auto req_path = local_topic.str();
  auto req_body = local_message.str();

  DynamicJsonDocument doc(2056);
  deserializeJson(doc, req_body);
  if (!doc.containsKey("session_id")) {
    return;
  }
  if (!doc.containsKey("sender")) {
    return;
  }
  if (!doc.containsKey("receiver")) {
    return;
  }
  if (!doc.containsKey("payload")) {
    return;
  }
  using std::placeholders::_1;
  auto *req = new Request(req_path,
                          doc["session_id"].as<int>(),
                          doc["sender"].as<std::string>(),
                          doc["receiver"].as<std::string>(),
                          doc["payload"],
                          std::bind(&Request_Gadget::sendRequest, this, _1));
  addIncomingRequest(req);
}

void MQTT_Gadget::executeRequestSending(Request *req) {
  std::string topic = req->getPath();
  std::string body = req->getBody();
  logger.print("System / MQTT", "publishing on '");
  logger.print(topic);
  logger.print("'");
  bool status = true;
  uint16_t msg_len = body.size();
  status = mqttClient_->beginPublish(topic.c_str(), msg_len, false);
  uint16_t k;
  for (
    k = 0;
    k < msg_len;
    k++) {
    status = status && mqttClient_->write(body[k]);
  }
  status = status && mqttClient_->endPublish();
  if (status)
    logger.println("OK");
  else
    logger.println("ERR");
}

MQTT_Gadget::MQTT_Gadget(const std::string& client_name,
                         std::string wifi_ssid,
                         std::string wifi_pw,
                         const IPAddress& mqtt_ip,
                         uint16_t mqtt_port,
                         const std::string& mqtt_username,
                         const std::string& mqtt_pw) :
  WiFiGadget(std::move(wifi_ssid), std::move(wifi_pw)),
  Request_Gadget(RequestGadgetType::MQTT_G),
  mqttServer_(mqtt_ip),
  mqtt_port_(mqtt_port),
  has_credentials_(true),
  client_name_(client_name) {
  if (wifiIsInitialized()) {
    logger.println("Creating MQTT Gadget");
    logger.incIndent();
    mqttClient_ = new PubSubClient(network_client_);
    bool everything_ok = true;

    // Check IP
    if (mqtt_ip == IPAddress(0, 0, 0, 0)) {
      everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'ip' is null");
    } else {
      logger.printfln("IP: %s", mqtt_ip.toString().c_str());
    }

    // Check port
    if (mqtt_port != 0) {
      mqtt_port_ = mqtt_port;
      logger.print(LOG_TYPE::DATA, "Port: ");
      logger.println(mqtt_port_);
    } else {
      everything_ok = false;
      logger.println(LOG_TYPE::ERR, "'port' is 0");
    }

    // Check username
    if (mqtt_username != "null") {
      username_ = mqtt_username;
      logger.print(LOG_TYPE::DATA, "Username: ");
      logger.println(username_);
    } else {
      // everything_ok = false;
      has_credentials_ = false;
      logger.println(LOG_TYPE::WARN, "no 'username' configured.");
    }

    // Check password
    if (mqtt_pw != "null") {
      password_ = mqtt_pw;
      logger.print(LOG_TYPE::DATA, "Password: ");
      logger.println(password_);
    } else {
      has_credentials_ = false;
      logger.println(LOG_TYPE::WARN, "no 'password' configured.");
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

bool MQTT_Gadget::subscibe_to_topic(const std::string& topic) {
  bool status = mqttClient_->subscribe(topic.c_str());
  if (status) {
    logger.printfln("Subscribed to %s", topic.c_str());
  } else {
    logger.printfln(LOG_TYPE::ERR, "Failed to subscribe to %s", topic.c_str());
  }
  return status;
}
