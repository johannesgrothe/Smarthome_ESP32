#include "config.h"

Config::Config(std::string id,
               NetworkMode network_mode,
               std::vector<gadget_tuple> gadgets,
               uint8_t ir_recv_pin,
               uint8_t ir_send_pin,
               uint8_t radio_recv_pin,
               uint8_t radio_send_pin,
               std::shared_ptr<std::string> wifi_ssid,
               std::shared_ptr<std::string> wifi_pw,
               std::shared_ptr<IPAddress> mqtt_ip,
               std::shared_ptr<uint16_t> mqtt_port,
               std::shared_ptr<std::string> mqtt_username,
               std::shared_ptr<std::string> mqtt_password):
               id_(id),
               network_mode_(network_mode),
               gadgets_(gadgets),
               ir_recv_pin_(ir_recv_pin),
               ir_send_pin_(ir_send_pin),
               radio_recv_pin_(radio_recv_pin),
               radio_send_pin_(radio_recv_pin),
               wifi_ssid_(wifi_ssid),
               wifi_pw_(wifi_pw),
               mqtt_ip_(mqtt_ip),
               mqtt_port_(mqtt_port),
               mqtt_username_(mqtt_username),
               mqtt_password_(mqtt_password) {}

std::vector <uint8_t> Config::getGadgetPorts() const {
  std::vector<uint8_t> ports;
  for (auto gadget: gadgets_) {
    auto buf_ports = std::get<2>(gadget);
    for (auto port: buf_ports) {
      if (port != 0) {
        ports.push_back(port);
      }
    }
  }
  return ports;
}

std::vector<gadget_tuple> Config::getGadgets() const {
  return gadgets_;
}

std::vector<std::string> Config::getGadgetNames() const {
  std::vector<std::string> names;
  for (auto gadget: gadgets_) {
    std::string buf_name = std::get<3>(gadget);
    names.push_back(buf_name);
  }
  return names;
}

uint8_t Config::getIRRecvPin() const {
  return ir_recv_pin_;
}

uint8_t Config::getIRSendPin() const {
  return ir_send_pin_;
}

uint8_t Config::getRadioRecvPin() const {
  return radio_recv_pin_;
}

uint8_t Config::getRadioSendPin() const {
  return radio_send_pin_;
}

NetworkMode Config::getNetworkMode() const {
  return network_mode_;
}

std::string Config::getID() const {
  return id_;
}

std::shared_ptr<std::string> Config::getWifiSSID() const {
  return wifi_ssid_;
}

std::shared_ptr<std::string> Config::getWifiPW() const {
  return wifi_pw_;
}

std::shared_ptr<IPAddress> Config::getMqttIP() const {
  return mqtt_ip_;
}

std::shared_ptr<uint16_t> Config::getMqttPort() const {
  return mqtt_port_;
}

std::shared_ptr<std::string> Config::getMqttUsername() const {
  return mqtt_username_;
}

std::shared_ptr<std::string> Config::getMqttPassword() {
  return mqtt_password_;
}
