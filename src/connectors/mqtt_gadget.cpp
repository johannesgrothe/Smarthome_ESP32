#include "mqtt_gadget.h"
#include <sstream>
#include <utility>

bool MQTTGadget::connect_mqtt() {
  mqttClient_->setServer(mqttServer_, mqtt_port_);

  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  mqttClient_->setCallback(std::bind(&MQTTGadget::callback, this, _1, _2, _3));

  logger.sender("MqttGadget").level(LOG_TYPE::DATA) << "Connecting to Broker ";
  uint8_t conn_count = 0;
  while (!mqttClient_->connected()) {
    bool connected;
    if (has_credentials_) {
      connected = mqttClient_->connect(client_name_.c_str(), username_.c_str(), password_.c_str());
    } else {
      connected = mqttClient_->connect(client_name_.c_str());
    }

    if (connected) {
      logger << ("OK\n");

      logger.sender("MqttGadget").level(LOG_TYPE::DATA) << "Subscribing to topics:";
      ++ logger;
      for (const auto &list_path: broadcast_request_paths) {
        subscribe_to_topic(list_path);
      }

      for (const auto &list_path: system_request_paths) {
        subscribe_to_topic(list_path);
      }

      for (const auto &list_path: additional_request_paths) {
        subscribe_to_topic(list_path);
      }

      -- logger;

      mqttClient_->publish("smarthome/debug/out", "Controller Launched");
      return true;
    } else {
      if (conn_count > 5) {
        logger.sender("MqttGadget") << "Failed.\n";
        logger.sender("MqttGadget").level(LOG_TYPE::ERR) << "No Connection to Broker could be established.\n";
        break;
      }
      logger << ".";
      delay(1000);
      conn_count++;
    }
  }
  return false;
}

void MQTTGadget::callback(char *topic, const byte *payload, const unsigned int length) {
  std::stringstream local_topic;
  local_topic << topic;

  logger.sender("MqttGadget") << "Callback called\n";

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
  auto req = std::make_shared<Request>(req_path,
                                       doc["session_id"].as<int>(),
                                       doc["sender"].as<std::string>(),
                                       doc["receiver"].as<std::string>(),
                                       doc["payload"],
                                       std::bind(&RequestGadget::sendRequest, this, _1));
  addIncomingRequest(req);
}

void MQTTGadget::executeRequestSending(std::shared_ptr<Request> req) {
  std::string topic = req->getPath();
  std::string body = req->getBody();
  logger.sender("MqttGadget") << "Publishing on '" << topic << ": " << body;
  bool status = mqttClient_->publish(topic.c_str(), body.c_str());
  mqttClient_->endPublish();
  if (status)
    logger << "OK\n";
  else
    logger << "ERR\n";
}

MQTTGadget::MQTTGadget(const std::string &client_name,
                       std::string wifi_ssid,
                       std::string wifi_pw,
                       const IPAddress &mqtt_ip,
                       uint16_t mqtt_port,
                       const std::string &mqtt_username,
                       const std::string &mqtt_pw) :
    WiFiGadget(std::move(wifi_ssid), std::move(wifi_pw)),
    RequestGadget(RequestGadgetType::MQTT_G),
    mqttServer_(mqtt_ip),
    mqtt_port_(mqtt_port),
    username_(),
    password_(),
    has_credentials_(true),
    client_name_(client_name) {
  if (wifiIsInitialized()) {
    bool everything_ok = initMqttClient(mqttServer_, mqtt_port_);

    logger.sender("MqttGadget") << "Username: " << username_ << "\n";
    logger.sender("MqttGadget") << "Password: " << password_ << "\n";

    connect_mqtt();
    -- logger;
    request_gadget_is_ready_ = everything_ok;
  } else {
    logger.sender("MqttGadget").level(LOG_TYPE::ERR) << "Cannot initialize MQTT: WiFi not working.\n";
    request_gadget_is_ready_ = false;
  }
}

MQTTGadget::MQTTGadget(const string &client_name, std::string wifi_ssid, std::string wifi_pw, const IPAddress &mqtt_ip,
                       uint16_t mqtt_port) :
    WiFiGadget(std::move(wifi_ssid), std::move(wifi_pw)),
    RequestGadget(RequestGadgetType::MQTT_G),
    mqttServer_(mqtt_ip),
    mqtt_port_(mqtt_port),
    username_(),
    password_(),
    has_credentials_(false),
    client_name_(client_name) {
  if (wifiIsInitialized()) {
    bool everything_ok = initMqttClient(mqttServer_, mqtt_port_);

    connect_mqtt();
    -- logger;
    request_gadget_is_ready_ = everything_ok;
  } else {
    logger.sender("MqttGadget").level(LOG_TYPE::ERR) << "Cannot initialize MQTT: WiFi not working.\n";
    request_gadget_is_ready_ = false;
  }
}

bool
MQTTGadget::initMqttClient(const IPAddress &mqtt_ip, uint16_t mqtt_port) {

  logger.sender("MqttGadget") << "Creating MQTT Gadget.\n";
  ++ logger;
  mqttClient_ = new PubSubClient(network_client_);
  bool everything_ok = true;

  // Check IP
  if (mqtt_ip == IPAddress(0, 0, 0, 0)) {
    everything_ok = false;
    logger.sender("MqttGadget").level(LOG_TYPE::ERR) << "'ip' is null\n";
  } else {
    logger.sender("MqttGadget") << "IP: " << mqtt_ip.toString() << "\n";
  }

  // Check port
  if (mqtt_port != 0) {
    mqtt_port_ = mqtt_port;
    logger.sender("MqttGadget").level(LOG_TYPE::DATA) << "Port: " << mqtt_port_ << "\n";
  } else {
    everything_ok = false;
    logger.sender("MqttGadget").level(LOG_TYPE::ERR) << "'port' is 0\n";
  }

  return everything_ok;
}

void MQTTGadget::refresh_network() {
  if (!mqttClient_->connected()) {
    connect_mqtt();
  }
  mqttClient_->loop();
  sendQueuedItems();
}

bool MQTTGadget::subscribe_to_topic(const std::string &topic) {
  bool status = mqttClient_->subscribe(topic.c_str());
  if (status) {
    logger.level(LOG_TYPE::DATA) << "Subscribed to " << topic << "\n";
  } else {
    logger.level(LOG_TYPE::ERR) << "Failed to subscribe to " << topic << "\n";
  }
  return status;
}
