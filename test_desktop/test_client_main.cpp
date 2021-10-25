#include "catch.hpp"

#include "../src/client_main.h"

TEST_CASE("Test Client Main", "[Main]") {

  SystemConfig system_cfg("test_client",
                          NetworkMode::MQTT,
                          0,
                          1,
                          2,
                          3,
                          std::make_shared<std::string>("wifi_ssid"),
                          std::make_shared<std::string>("wifi_password"),
                          std::make_shared<IPContainer>(192, 168, 178, 214),
                          std::make_shared<uint16_t>(4),
                          std::make_shared<std::string>("username"),
                          std::make_shared<std::string>("password"));
  GadgetConfig gadget_cfg({});
  EventConfig event_cfg({});
  auto main = ClientMain(BootMode::Serial_Only, system_cfg, gadget_cfg, event_cfg);

  SECTION("Test Attributes") {
    CHECK(system_cfg.id == "test_client");
    CHECK(system_cfg.network_mode == NetworkMode::MQTT);
    CHECK(system_cfg.ir_recv_pin == 0);
    CHECK(system_cfg.ir_send_pin == 1);
    CHECK(system_cfg.radio_recv_pin == 2);
    CHECK(system_cfg.radio_send_pin == 3);

    CHECK(system_cfg.wifi_ssid != nullptr);
    CHECK(*system_cfg.wifi_ssid == "wifi_ssid");

    CHECK(system_cfg.wifi_pw != nullptr);
    CHECK(*system_cfg.wifi_pw == "wifi_password");

    CHECK(system_cfg.mqtt_ip != nullptr);
    CHECK(*system_cfg.mqtt_ip == IPContainer(192, 168, 178, 214));

    CHECK(system_cfg.mqtt_port != nullptr);
    CHECK(*system_cfg.mqtt_port == 4);

    CHECK(system_cfg.mqtt_username != nullptr);
    CHECK(*system_cfg.mqtt_username == "username");

    CHECK(system_cfg.mqtt_password != nullptr);
    CHECK(*system_cfg.mqtt_password == "password");
  }
}
