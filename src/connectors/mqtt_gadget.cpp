#include "mqtt_gadget.h"
#include <sstream>
#include <utility>

static const char *TAG = "MQTTGadget";

bool MQTTGadget::connect_mqtt() {
  mqttClient_->setServer(mqttServer_.getData(), mqtt_port_);

  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  mqttClient_->setCallback(std::bind(&MQTTGadget::callback, this, _1, _2, _3));

  logger_i(TAG, "Connecting to Broker...");
  uint8_t conn_count = 0;
  while (!mqttClient_->connected()) {
    bool connected;
    if (has_credentials_) {
      connected = mqttClient_->connect(client_name_.c_str(), username_.c_str(), password_.c_str());
    } else {
      connected = mqttClient_->connect(client_name_.c_str());
    }

    if (connected) {
      logger_i(TAG, "Established connection to mqtt broker");

      logger_i(TAG, "Subscribing to topics:");
      subscribe_to_topic("smarthome/*");

      mqttClient_->publish("smarthome/debug/out", "Controller Launched");
      return true;
    } else {
      if (conn_count > 5) {
        // TODO: Check code structure
        logger_e(TAG, "No Connection to Broker could be established.");
        break;
      }
      delay(1000);
      conn_count++;
    }
  }
  return false;
}

void MQTTGadget::callback(char *topic, const byte *payload, const unsigned int length) {
  std::stringstream local_topic;
  local_topic << topic;

  logger_d(TAG, "Callback called");

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
                                       doc["payload"]);
  addIncomingRequest(req);
}

void MQTTGadget::executeRequestSending(std::shared_ptr<Request> req) {
  std::string topic = req->getPath();
  std::string body = req->getBody();

  auto msg_len = body.size();

  logger_i(TAG, "Publishing on %s: %d bytes", topic.c_str(), msg_len);

//  bool status = mqttClient_->publish(topic.c_str(), body.c_str());
//  mqttClient_->endPublish();

  bool status = mqttClient_->beginPublish(topic.c_str(), msg_len, false);
  uint16_t k;
  for (
      k = 0;
      k < msg_len;
      k++) {
    status = status && mqttClient_->write(body[k]);
  }
  status = status && mqttClient_->endPublish();

  if (!status)
    logger_e(TAG, "Failed to publish Message to MQTT Broker");
}

MQTTGadget::MQTTGadget(const std::string &client_name,
                       std::string wifi_ssid,
                       std::string wifi_pw,
                       const IPContainer &mqtt_ip,
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

    logger_i(TAG, "Username: %s", username_.c_str());
    logger_i(TAG, "Password: %s", password_.c_str());

    connect_mqtt();
    request_gadget_is_ready_ = everything_ok;
  } else {
    logger_e(TAG, "Cannot initialize MQTT: WiFi not working.");
    request_gadget_is_ready_ = false;
  }
}

MQTTGadget::MQTTGadget(const std::string &client_name, std::string wifi_ssid, std::string wifi_pw,
                       const IPContainer &mqtt_ip,
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
    request_gadget_is_ready_ = everything_ok;
  } else {
    logger_e(TAG, "Cannot initialize MQTT: WiFi not working.");
    request_gadget_is_ready_ = false;
  }
}

bool
MQTTGadget::initMqttClient(const IPContainer &mqtt_ip, uint16_t mqtt_port) {

  logger_i(TAG, "Creating MQTT Gadget.");
  mqttClient_ = new PubSubClient(network_client_);
  bool everything_ok = true;

  // Check IP
  if (mqtt_ip == IPContainer(0, 0, 0, 0)) {
    everything_ok = false;
    logger_e(TAG, "'ip' is null");
  } else {
    logger_i(TAG, "IP: %s", mqtt_ip.toString().c_str());
  }

  // Check port
  if (mqtt_port != 0) {
    mqtt_port_ = mqtt_port;
    logger_i(TAG, "Port: %d", mqtt_port);
  } else {
    everything_ok = false;
    logger_e(TAG, "'port' is 0");
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
    logger_i(TAG, "Subscribed to %s", topic.c_str());
  } else {
    logger_e(TAG, "Failed to subscribed to %s", topic.c_str());
  }
  return status;
}
