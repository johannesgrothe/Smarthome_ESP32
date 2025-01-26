#include "config_decoder.h"

#include <sstream>

std::shared_ptr<EolConfig> ConfigDecoder::decodeEolConfig(DynamicJsonDocument config) {
    if (!JsonValidator::checkKeys(config, {
                                      "hw_variant",
                                      "hw_serial"
                                  })) {
        return nullptr;
    }
    HwVariant hw_variant = config["hw_variant"];
    std::string hw_serial = config["hw_serial"];

    return std::make_shared<EolConfig>(hw_variant, hw_serial);
}


std::shared_ptr<SystemConfig> ConfigDecoder::decodeSystemConfig(DynamicJsonDocument config) {
    if (!JsonValidator::checkKeys(config, {
                                      "id",
                                      "wifi_ssid",
                                      "wifi_pw",
                                      "mqtt_ip",
                                      "mqtt_port",
                                      "mqtt_user",
                                      "mqtt_pw"
                                  })) {
        return nullptr;
    }

    std::string id = config["id"];

    std::shared_ptr<std::string> wifi_ssid = nullptr;
    std::shared_ptr<std::string> wifi_pw = nullptr;

    std::shared_ptr<IPContainer> mqtt_ip = nullptr;
    std::shared_ptr<uint16_t> mqtt_port = nullptr;

    std::shared_ptr<std::string> mqtt_username = nullptr;
    std::shared_ptr<std::string> mqtt_pw = nullptr;

    if (config["wifi_ssid"]) {
        wifi_ssid = std::make_shared<std::string>(config["wifi_ssid"].as<std::string>());
    }

    if (config["wifi_pw"]) {
        wifi_pw = std::make_shared<std::string>(config["wifi_pw"].as<std::string>());
    }

    if (config["mqtt_ip"]) {
        uint8_t buf_ip[4];
        uint8_t counter = 0;
        std::stringstream ip_strm;
        int buf_val = 0;
        for (char c: config["mqtt_ip"].as<std::string>()) {
            if (c == '.') {
                ip_strm >> buf_val;
                buf_ip[counter] = static_cast<uint8_t>(buf_val);
                counter++;
                ip_strm = std::stringstream();
            } else {
                ip_strm << c;
            }
        }
        ip_strm >> buf_val;
        buf_ip[counter] = static_cast<uint8_t>(buf_val);
        mqtt_ip = std::make_shared<IPContainer>(buf_ip[0], buf_ip[1], buf_ip[2], buf_ip[3]);
    }

    if (config["mqtt_port"]) {
        mqtt_port = std::make_shared<uint16_t>(config["mqtt_port"].as<uint16_t>());
    }

    if (config["mqtt_user"]) {
        mqtt_username = std::make_shared<std::string>(config["mqtt_user"].as<std::string>());
    }

    if (config["mqtt_pw"]) {
        mqtt_pw = std::make_shared<std::string>(config["mqtt_pw"].as<std::string>());
    }

    return std::make_shared<SystemConfig>(id,
                                          wifi_ssid,
                                          wifi_pw,
                                          mqtt_ip,
                                          mqtt_port,
                                          mqtt_username,
                                          mqtt_pw);
}
